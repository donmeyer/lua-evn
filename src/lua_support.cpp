// lua_support.cpp

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
#include "lua_tools.h"
#include "lua_support.h"

#include "lib_platform.h"
#include "lib_arduino.h"

#if defined (ARDUINO_EVN_ALPHA)
#include "lib_evn_board.h"
#endif



#define DEBUG_ENABLED   1


bool execLoopEnabled = true;
bool housekeepingLoopEnabled = true;


// Set true when the core0 Lua setup() function has been called.
static bool setupFuncCalled = false;

static struct lua_State *luaState;

/**
 * these libs are loaded by lua.c and are readily available to any Lua
 * program
 */
static const luaL_Reg loadedlibs[] = {
  {LUA_GNAME, luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  // {LUA_IOLIBNAME, luaopen_io},
  // {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};


static void debug( const char *format, ... );

static bool findFunction( lua_State *L, const char *name );

static int callFunction( lua_State *L, int numArgs, int numRets );
static void callEvent( lua_State *L, const char *name, const void *data, int size );

static int loadScript( lua_State *L, const char *name );
static int callScript( lua_State *L );

static int errorHandler( lua_State *L );


static void registerSearcher( lua_State *L );
static int arduinoSearcher( lua_State *L );
static void registerPreloads( lua_State* const L );



void setupLua()
{
    luaState = luaL_newstate();

    struct lua_State *L = luaState;


    // Open most of the standard libraries.
    // We don't open 'os' or 'io'.
    debug( "Opening most of standard libs" );

    // "require" functions from 'loadedlibs' and set results to global table
    const luaL_Reg *lib;
    for (lib = loadedlibs; lib->func; lib++)
    {
        luaL_requiref( L, lib->name, lib->func, 1 );
        lua_pop( L, 1 );  // remove lib
    }

    registerSearcher( L );

    registerPreloads( L );

    // Clear the Lua stack
    lua_settop( L, 0 );

    // This will load and run the initial Lua source file.
    debug( "Loading the 'main.lua' script" );
    loadLuaFile( L, "main" );

    // The init event gets called the first time we load the script only.
    if( findFunction( L, "setup" ) )
    {
        debug( "Calling the Lua script 'setup' function");
        callFunction( L, 0, 0 );
        setupFuncCalled = true;
    }

    debug( "lua_Integer=%d  int=%d   long=%d   size_t=%d", sizeof(lua_Integer), sizeof(int), sizeof(long), sizeof(size_t) );
    debug( "lua_Number=%d   float=%d   double=%d", sizeof(lua_Number), sizeof(float), sizeof(double) );

    debug( "Lua setup 0 complete." );
}


void setupLua1()
{
    struct lua_State *L = luaState;

    // The init event gets called the first time we load the script only.
    if( findFunction( L, "setup1" ) )
    {
        debug( "Calling the Lua script 'setup1' function");
        callFunction( L, 0, 0 );
    }

    debug( "Lua setup 1 complete." );
}


// Deal with the special case of no filesystem. (or at least, no `main.lua`)
void checkLuaSetupFunction( lua_State *L )
{
  if( setupFuncCalled == false )
  {
      if( findFunction( L, "setup" ) )
      {
          debug( "Calling the Lua script 'setup' function");
          callFunction( L, 0, 0 );
          setupFuncCalled = true;
      }
  }
}


void runLua()
{
    struct lua_State *L = luaState;

    handleShell( L );

    if( lua_gettop( L ) > 5 )
    {
        debug( "Stack %d, should never be this high", lua_gettop( L ) );
    }

    if( execLoopEnabled )
    {
        if( findFunction( L, "exec_loop" ) )
        {
            if( callFunction( L, 0, 0 ) != 0 )
            {
                // If this function fails, stop trying to call it!
                execLoopEnabled = false;
            }
        }
    }

    if( housekeepingLoopEnabled )
    {
        if( findFunction( L, "housekeeping_loop" ) )
        {
            if( callFunction( L, 0, 0 ) != 0 )
            {
                // If this function fails, stop trying to call it!
                housekeepingLoopEnabled = false;
            }
        }
    }
}


void loadLuaFile( lua_State *L, const char *modname )
{
    debug( "Loading module '%s'", modname );

    int rc = loadScript( L, modname );
    if( rc == 0 )
    {
        rc = callScript( L );
        if( rc )
        {
            // If callScript fails, it pushes an error string onto the stack.
            lua_writestringerror( "%s\n", lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
        }
    }
    else
    {
        // If loadScript fails, it pushes an error string onto the stack.
        lua_writestringerror( "%s\n", lua_tostring( L, -1 ) );
        lua_pop( L, 1 );
    }
}


//=============================================================================================
//=============================================================================================
//=============================================================================================

/**
 * Load a file from disk.
 *
 * Pushes the loaded script as a function on top of the Lua stack and returns zero if it succeeds.
 * If this fails, it returns non-zero and leaves an error string on the Lua stack.
 */
static int loadScript( lua_State *L, const char *name )
{
    // Look for the file "<name>.lua"
    char fname[64];
    snprintf( fname, sizeof(fname), "/%s.lua", name );

#if defined (ARDUINO_ARCH_RP2040)
    File file = SDFS.open( fname, "r" );
#else
    File file = SD.open( fname );
#endif
    if( ! file )
    {
        lua_pushfstring( L, "no file '%s'", fname );
        return -1;
    }

    // Set up a buffer to load the file into.
    luaL_Buffer b;
    luaL_buffinit( L, &b );

    size_t len;

    do
    {
        // Get pointer to buffer space.
        uint8_t *buf = (uint8_t*) luaL_prepbuffer( &b );

        // Read a segment of the file into the buffer.
        len = file.read( buf, LUAL_BUFFERSIZE );
        luaL_addsize( &b, len );
    }
    while( len > 0 );

    file.close();

    // Push the buffer contents onto the stack as a string.
    luaL_pushresult( &b );

    // Get the pointer to the string, and its length.
    const char *p = lua_tolstring( L, -1, &len );

    // Load the buffer.
    int ret = luaL_loadbuffer( L, p, len, name );
    lua_remove( L, -2 );    // Remove the chunk string from the stack.
    if( ret != LUA_OK )
    {
        // Compile error
        lua_pushfstring( L, "Compile error: %s\n", lua_tostring( L, -1 ) );
        lua_remove( L, -2 );
        return -1;
    }

    return 0;
}


/**
 * Call the function on the top of the Lua stack.
 *
 * If this fails, it returns non-zero and leaves an error string on the Lua stack.
 */
static int callScript( lua_State *L )
{
    int rc = lua_pcall( L, 0, LUA_MULTRET, 0 );
    if( rc )
    {
        lua_pushfstring( L, "Run script failed: %s\n", lua_tostring( L, -1 ) );
    }

    return rc;
}


/**
 * Call a function on the stack. This also expects any arguments to be there as well.
 *
 * stack:  [func] [args...] <-- Top
 */
static int callFunction( lua_State *L, int numArgs, int numRets )
{
        // Tuck our error handler *under* the function on the stack
        lua_pushcfunction( L, errorHandler );
        lua_insert( L, -(numArgs+2) );

        // Stack is now: ... [error-func]  [func-to-call] [args...] <-- Top

        // The last argument to pcall is the index of the error handler
        int err = lua_pcall( L, numArgs, 0, -(numArgs+2) );
        if( err )
        {
            lua_writestringerror( "Error: %s\n", lua_tostring( L, -1 ) );
            lua_pop( L, 1 );
            return -1;
        }

        lua_pop( L, 1 );        // error handler

        return 0;
}




/**
 * This version will check for aliases and call them as well.
 */
static void callEvent( lua_State *L, const char *name, const void *data, int size )
{
//     printf( "call event %s\n", name );
    if( findFunction( L, "event_handler" ) )
    {
        // Function exists
        lua_pushstring( L, name);

        if( data )
        {
            lua_pushlstring( L, (const char*)data, size );
            // Stack is now: ... [error-func]  [func-to-call]  [data]  <-- Top
        }

        int argc = data ? 2 : 1;

        callFunction( L, argc, 0 );
    }
}



/**
 * Find function and leave it the top of the Lua stack.
 *
 * @param name The name of the function
 * @return true if the function was found
 */
static bool findFunction( lua_State *L, const char *name )
{
    lua_getglobal( L, name );

    if( ! lua_isfunction( L, -1 ) )
    {
        // Not a function
        lua_pop( L, 1 );
        return false;
    }

    return true;
}


/**
 * This can be used to get an error with the Lua location when doing a lua_pcall()
 */
static int errorHandler( lua_State *L )
{
#if 0
    dumpstack( L, "Error handler stack" );

    printf( "errfunc says: %s", lua_tostring(L,-1) );

    luaL_where( L, 0 );
    const char *where = lua_tostring( L, -1 );
    printf( "Where 0 `%s`\n", where );

    luaL_where( L, 1 );
    where = lua_tostring( L, -1 );
    printf( "Where 1 `%s`\n", where );

    luaL_where( L, 2 );
    where = lua_tostring( L, -1 );
    printf( "Where 2 `%s`\n", where );
#endif

    luaL_where( L, 2 );
    const char *where = lua_tostring( L, -1 );

    lua_pushfstring( L, "%s%s", where, lua_tostring(L,-2) );

    return 1;
}


static void registerPreloads( lua_State* const L )
{
    // Global table: package
    lua_getglobal( L, "package" );

    // The list of preloads
    lua_getfield( L, -1, "preload" );

    //
    // Add Lua EVN libraries
    //

    // Arduino
    lua_pushstring( L, "arduino" );
    lua_pushcfunction( L, luaopen_arduino );
    lua_settable( L, -3 );

#if defined (ARDUINO_EVN_ALPHA)
    // EVN Alpha Libraries
    lua_pushstring( L, "evn" );
    lua_pushcfunction( L, luaopen_evn_board );
    lua_settable( L, -3 );
#endif

    // Lua Platform
    lua_pushstring( L, "luaplatform" );
    lua_pushcfunction( L, luaopen_platform );
    lua_settable( L, -3 );

    // Clean up the stack.
    lua_pop(L, 2);
}


/**
 * Register our own searcher function that will allow modules to be required from the
 * Arduino SD filesystem.
 */
static void registerSearcher( lua_State *L )
{
    // Global table: package
    lua_getglobal( L, "package" );

    // The list of searchers from the package table
    lua_getfield( L, -1, "searchers" );

    // There are 4 predefined searchers (at least in 5.4.7).
    // Remove all but the "preload" searcher before adding ours.
    lua_pushnil( L );
    lua_rawseti( L, -2, 4 );

    lua_pushnil( L );
    lua_rawseti( L, -2, 3 );

    lua_pushnil( L );
    lua_rawseti( L, -2, 2 );

    // Add our searcher in the second index.
    lua_pushcfunction( L, arduinoSearcher );
    lua_rawseti( L, -2, 2 );

    // Clean up the stack.
    lua_pop( L, 2 );
}


/**
 * Search the Arduino SD filesystem for a module to load via `require()`.
 */
static int arduinoSearcher( lua_State *L )
{
    const char* const modname = lua_tostring(L, 1);
    debug( "Searching for module: %s", modname );

    (void) loadScript( L, modname );
    return 1;
}


static void debug( const char *format, ... )
{
#if DEBUG_ENABLED
    static char buf[256];

    va_list	args;
    va_start( args, format );

    vsnprintf( buf, sizeof(buf), format, args );
    LUA_SERIAL.println( buf );

    va_end( args );
#endif
}


//
// In the Lua config header, the Lua write string defines are re-defined to use these
// functions.
//
// This allows routing Lua print() and error messages to the port of our choice.
// (e.g. Serial)
//
// Don't call these directly!
// Call:
//   lua_writestring
//     lua_writestringerror
//

extern "C" void dtm_writestring( const char *p, int len )
{
    LUA_SERIAL.write( p, len );
}


extern "C" void dtm_writestringerror( const char *s, const char *p )
{
    static char buf[512];
    snprintf( buf, sizeof(buf), s, p );
    LUA_SERIAL.print( buf );
}
