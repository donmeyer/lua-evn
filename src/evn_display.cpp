// evn_display.cpp

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
#include <EVN.h>

#include "lua.hpp"

#include "lua_tools.h"

#include "evn_display.h"


static int new_object( lua_State *L );



static int begin( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int splashEVN( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    obj->splashEVN();
    return 0;
}


static int rotate( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    obj->rotate();
    return 0;
}


static int clear( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    obj->clear();
    return 0;
}


static int clearLine( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    int row = methodArgInt( L, 1 );
    obj->clearLine( row );
    return 0;
}


static int writeData( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    int row = methodArgInt( L, 1 );
    const char *data = methodArgString( L, 2 );
    obj->writeData( row, data );
    return 0;
}


static int writeLabel( lua_State *L )
{
    EVNDisplay *obj = (EVNDisplay*)luaL_checkudata( L, 1, "EVNDisplay" );
    int row = methodArgInt( L, 1 );
    const char *label = methodArgString( L, 2 );
    obj->writeLabel( row, label );
    return 0;
}




//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "splashEVN", splashEVN },
    { "rotate", rotate },
    { "clear", clear },
    { "clearLine", clearLine },
    { "writeData", writeData },
    { "writeLabel", writeLabel },
    { "print", writeLabel },        // alias

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_display( lua_State *L )
{
    //
    // Objects
    //

    // Create metatable
    luaL_newmetatable( L, "EVNDisplay" );

    // metatable.__index = metatable
    lua_pushvalue( L, -1 );        // dup mt
    lua_setfield( L, -2, "__index" );

    luaL_setfuncs( L, methods, 0 );

    lua_pop( L, 1 );


    //
    // Class
    //

    // The object table
    lua_pushstring( L, "Display" );

    lua_newtable( L );
    luaL_setfuncs( L, funcs, 0 );

    // Class constants

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    bool flip_180deg = functionArgBool( L, 2, false );

    void *rp = lua_newuserdata( L, sizeof(EVNDisplay) );
    // ud --

    EVNDisplay *p = new(rp) EVNDisplay( port, flip_180deg );

    // Add metatable
    luaL_getmetatable( L, "EVNDisplay" );
    lua_setmetatable( L, -2 );

    return 1;
}
