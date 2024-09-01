// shell.cpp

/*
 * Copyright 2024 Donald T. Meyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#if defined (ARDUINO_ARCH_RP2040)
#include <SDFS.h>
#else
#include <SD.h>
#endif

#include "lua.hpp"

#include "shell.h"
#include "lua_support.h"



static luaL_Buffer downloadBuf;

static bool needPrompt = true;


enum class ShellMode {
    Interactive,
    Multiline,

    DownloadReady,
    DownloadUnderway
};

static ShellMode shellMode = ShellMode::Interactive;

static char downloadModname[80];

static unsigned long lastCharTime = 0;


static void handleShellChar( lua_State *L, char c );
static void processCommand( lua_State *L, const char *line );

static void finishDownload( lua_State *L );

static void processControl( lua_State *L, const char *cmd );
static void processInteractiveLine( lua_State *L, const char *line );
static void processMultiline( lua_State *L, const char *line );

static boolean messageHasEOF( lua_State *L );
static void executeChunk( lua_State *L );

static void shellPrint( const char *format, ... );



void handleShell( lua_State *L )
{
    // Display the shell prompt if needed.
    if( needPrompt )
    {
        if( shellMode == ShellMode::Multiline )
        {
            shellPrint( ">>" );
        }
        else
        {
            shellPrint( ">" );
        }

        needPrompt = false;
    }

    // Check to see if an ongoing download is complete.
    if( shellMode == ShellMode::DownloadUnderway )
    {
        // Check to see if the download is complete.
        if( millis() >= (lastCharTime + 1000) )
        {
            // Timed out, assume all done.
            shellPrint( "Download complete\n" );

            finishDownload( L );

            shellMode = ShellMode::Interactive;

            needPrompt = true;
        }
    }

    // Get all pending characters from the terminal.
    while( LUA_SERIAL.available() )
    {
        int c = LUA_SERIAL.read();

        // If ready to download, prep the buffer and set the mode to Underway.
        if( shellMode == ShellMode::DownloadReady )
        {
            luaL_buffinit( L, &downloadBuf );

            shellMode = ShellMode::DownloadUnderway;
        }

        if( shellMode == ShellMode::DownloadUnderway )
        {
            lastCharTime = millis();
            luaL_addchar( &downloadBuf, c );
        }
        else
        {
            handleShellChar( L, c );
        }
    }
}


static void finishDownload( lua_State *L )
{
    // Push buffer result onto stack
    luaL_pushresult( &downloadBuf );

    // Get the pointer to the string, and its length.
    size_t len;
    const char *p = lua_tolstring( L, -1, &len );

    if( downloadModname[0] != '\0' )
    {
        // Download has a module name given.

        // Write to file
        char fname[64];
        snprintf( fname, sizeof(fname), "/%s.lua", downloadModname );

        shellPrint( "Writing file '%s'\n", fname );

#if defined (ARDUINO_ARCH_RP2040)
        File file = SDFS.open( fname, "w" );
#else
        SD.remove( fname );
        File file = SD.open( fname, FILE_WRITE );
#endif
        if( file )
        {
            size_t nr = file.write( (uint8_t*)p, len );
            file.close();
        }
        else
        {
            lua_pushfstring( L, "Unable to open file '%s'", fname );
        }

        lua_pop( L, 1 );    // Pop string from stack

        // Load and run the file
        loadLuaFile( L, downloadModname );
    }
    else
    {
        // No module name.
        shellPrint( "Loading anonymous Lua chunk\n" );

        // Execute the chunk of Lua code.
        int err = luaL_loadbuffer( L, p, len, "download" );
        if( err )
        {
            lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
        }
        else
        {
            err = lua_pcall( L, 0, LUA_MULTRET, 0 );
            if( err )
            {
                lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
            }
            else
            {
                // Deal with the special case of no filesystem. (or at least, no `main.lua`) by
                // trying to call the Lua `setup()` function if it has never been called previously.
                checkLuaSetupFunction( L );
            }
        }

        lua_pop( L, 1 );
    }
}


static void handleShellChar( lua_State *L, char c )
{
    static char commandBuf[300];
    static int commandLen = 0;

    if( c == '\r' )
    {
        // Have full line
        shellPrint( "\r\n" );

        commandBuf[commandLen] = '\0';

        if( shellMode == ShellMode::Multiline )
        {
            processMultiline( L, commandBuf );

            needPrompt = true;
        }
        else
        {
            processCommand( L, commandBuf );
        }

        commandLen = 0;
    }
    else if( c == '\b' )
    {
        // Backspace
        if( commandLen > 0 )
        {
            commandLen--;
            shellPrint( "\b \b" );
        }
        else
        {
            shellPrint( "\a" );
        }
    }
    else
    {
        commandBuf[commandLen++] = c;
        LUA_SERIAL.write( c );  // echo
    }
}


static void processCommand( lua_State *L, const char *line )
{
    const char *p = line;

    if( *p == '*' )
    {
        // Download a module
        shellMode = ShellMode::DownloadReady;

        // Module name is on command line.
        p++;

        // Skip whitespace
        while( *p == ' ' )
        {
            p++;
        }

        if( strlen(p) > 0 )
        {
            strcpy( downloadModname, p );
        }
        else
        {
            downloadModname[0] = '\0';
        }

        shellPrint( "Waiting for download of '%s'...\n", downloadModname );
    }
    else if( *p == '@' )
    {
        // Reload a module
        // Module name is in command buffer.
        p++;

        // Skip whitespace
        while( *p == ' ' )
        {
            p++;
        }

        strcpy( downloadModname, p );

        shellPrint( "Reloading '%s'\n", downloadModname );

        // Load and run the file
        loadLuaFile( L, downloadModname );

        shellPrint( "Reload complete\n" );
    }
    else if( *p == ':' )
    {
        p++;
        processControl( L, p );

        needPrompt = true;
    }
    else
    {
        // Is the the first line of a file download?
        if( strncmp( p, "--", 2 ) == 0 )
        {
            // Looks like a Lua comment line. Is it followed by a filename?
            p += 2;

            // Skip whitespace
            while( *p == ' ' )
            {
                p++;
            }

            // Look for the dot Lua.
            char *pos = strstr( p, ".lua" );
            if( pos )
            {
                // Has a Lua file extension
                int len = pos - p;
                strncpy( downloadModname, p, len );
                downloadModname[len] = '\0';

                // Init the chunk buffer
                luaL_buffinit( L, &downloadBuf );

                // Add the first line.
                luaL_addstring( &downloadBuf, line );
                luaL_addchar( &downloadBuf, '\n' );

                lastCharTime = millis();

                shellMode = ShellMode::DownloadUnderway;

                shellPrint( "Downloading module '%s'\n", downloadModname );
            }
        }

        if( shellMode == ShellMode::Interactive )
        {
            // Execute the interactive line of Lua code.
            processInteractiveLine( L, line );

            needPrompt = true;
        }
    }
}


static void processControl( lua_State *L, const char *cmd )
{
    switch( cmd[0] )
    {
        case '+':
            switch( cmd[1] )
            {
                case 'e':
                    execLoopEnabled = true;
                    shellPrint( "Exec loop enabled\n" );
                    break;

                case 'h':
                    housekeepingLoopEnabled = true;
                    shellPrint( "Housekeeping loop enabled\n" );
                    break;

                default:
                    shellPrint( "Invalid command '%s'", cmd );
                    break;
            }
            break;

        case '-':
            switch( cmd[1] )
            {
                case 'e':
                    execLoopEnabled = false;
                    shellPrint( "Exec loop disabled\n" );
                    break;

                case 'h':
                    housekeepingLoopEnabled = false;
                    shellPrint( "Housekeeping loop disabled\n" );
                    break;

                default:
                    shellPrint( "Invalid command '%s'\n", cmd );
                    break;
            }
            break;
    }
}


static void processMultiline( lua_State *L, const char *line )
{
    if( *line == '\0' )
    {
        // Empty line, abort the multi-line chunk in progress.
        lua_pop( L, 1 );
        shellMode = ShellMode::Interactive;
        return;
    }

    // Concat the line onto the exiting chunk string
    lua_pushliteral( L, "\n" );  /* add newline... */
    lua_pushstring( L, line );
    lua_concat( L, 3 );  /* join them */

    const char *p = lua_tostring( L , -1 );

    int err = luaL_loadstring( L, p );
    if( err == LUA_OK )
    {
        lua_remove( L, -2 );    // Remove the concat string

        executeChunk( L );

        shellMode = ShellMode::Interactive;
    }
    else if( err == LUA_ERRSYNTAX )
    {
        if( messageHasEOF( L ) )
        {
            // Keep going
            lua_pop( L, 1 );
        }
        else
        {
            lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
            lua_pop( L, 2 );    // Pop err string and the multi-line string

            shellMode = ShellMode::Interactive;
        }
    }
    else
    {
        lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
        lua_pop( L, 2 );    // Pop err string and the multi-line string

        shellMode = ShellMode::Interactive;
    }
}


static boolean messageHasEOF( lua_State *L )
{
    size_t len;
    const char *msg = lua_tolstring( L, -1, &len );
    if( len >= 5 )
    {
        if( strcmp( msg + len - 5, "<eof>" ) == 0 )
        {
            // Error message ends with <eof>, so this is to be continued.
            return true;
        }
    }

    return false;
}


static void executeChunk( lua_State *L )
{
    int err = lua_pcall( L, 0, LUA_MULTRET, 0 );

    if( err == LUA_OK )
    {
        int rets = lua_gettop( L );
        if( rets > 0 )
        {
            // Print the returned results by calling the Lua print() function.
            luaL_checkstack( L, LUA_MINSTACK, "Too many results to print" );
            lua_getglobal( L, "print" );
            lua_insert( L, 1 );
            if( lua_pcall( L, rets, 0, 0 ) != LUA_OK )
            {
                const char *msg = lua_pushfstring( L, "Error calling 'print' (%s)", lua_tostring( L, -1 ) );
                lua_writestringerror( "%s\n", msg );
            }
        }
    }
    else
    {
        // Print the error
        lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
        lua_pop( L, 1 );
    }
}


static void processInteractiveLine( lua_State *L, const char *line )
{
    // First, try with a return prepended.
    lua_pushstring( L, "return " );
    lua_pushstring( L, line );
    lua_concat( L, 2 );  // join the strings

    const char *p = lua_tostring( L, -1 );

    int err = luaL_loadstring( L, p );

    lua_remove( L, -2 );

    if( err == LUA_OK )
    {
        // Return worked
        // Execute the chunk
        executeChunk( L );
    }
    else
    {
        // Return failed, try it as-is
        lua_pop( L, 1 );    // pop the error message
        err = luaL_loadstring( L, line );

        if( err == LUA_OK )
        {
            // Execute the chunk
            executeChunk( L );
        }
        else if( err == LUA_ERRSYNTAX )
        {
            // See if this is the beginning of a multi-line chunk.
            // (search for <eof> in the error string.)
            if( messageHasEOF( L ) )
            {
                // Start the chunk buffer with the line contents.
                lua_pop( L, 1 );    // pop the error message
                lua_pushstring( L, line );

                shellMode = ShellMode::Multiline;
            }
            else
            {
                lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
                lua_pop( L, 1 );
            }
        }
        else
        {
            lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
        }
    }
}


static void shellPrint( const char *format, ... )
{
    static char buf[256];

    va_list	args;
    va_start( args, format );

    vsnprintf( buf, sizeof(buf), format, args );
    LUA_SERIAL.print( buf );

    va_end( args );
}
