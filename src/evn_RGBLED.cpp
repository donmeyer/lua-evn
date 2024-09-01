// evn_RGBLED.cpp

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

#include "evn_RGBLED.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNRGBLED
#define EVN_CLASS_NAME      "EVNRGBLED"
#define LUA_CLASS_NAME      "RGBLED"



static int begin( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int setInvert( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    bool enable = methodArgBool( L, 1 );
    obj->setInvert( enable );
    return 0;
}


static int getInvert( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    lua_pushinteger( L, obj->getInvert() );
    return 1;
}


static int setLEDCount( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int led_count = methodArgInt( L, 1 );
    obj->setLEDCount( led_count );
    return 0;
}


static int getLEDCount( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    lua_pushinteger( L, obj->getLEDCount() );
    return 1;
}


static int writeOne( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int led = methodArgInt( L, 1 );
    int r = methodArgInt( L, 2, 0 );
    int g = methodArgInt( L, 3, 0 );
    int b = methodArgInt( L, 4, 0 );
    bool show = methodArgBool( L, 5, true );
    obj->writeOne( led, r, g, b, show );
    return 0;
}


static int clearOne( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int led = methodArgInt( L, 1 );
    bool show = methodArgBool( L, 2, true );
    obj->clearOne( led, show );
    return 0;
}


static int writeLine( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int start_led = methodArgInt( L, 1 );
    int end_led = methodArgInt( L, 2 );
    int r = methodArgInt( L, 3, 0 );
    int g = methodArgInt( L, 4, 0 );
    int b = methodArgInt( L, 5, 0 );
    bool show = methodArgBool( L, 6, true );
    obj->writeLine( start_led, end_led, r, g, b, show );
    return 0;
}


static int clearLine( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int start_led = methodArgInt( L, 1 );
    int end_led = methodArgInt( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->clearLine( start_led, end_led, show );
    return 0;
}


static int writeAll( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    int r = methodArgInt( L, 1, 0 );
    int g = methodArgInt( L, 2, 0 );
    int b = methodArgInt( L, 3, 0 );
    bool show = methodArgBool( L, 4, true );
    obj->writeAll( r, g, b, show );
    return 0;
}


static int clearAll( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    bool show = methodArgBool( L, 1, true );
    obj->clearAll( show );
    return 0;
}


static int update( lua_State *L )
{
    EVNRGBLED *obj = (EVNRGBLED*)luaL_checkudata( L, 1, "EVNRGBLED" );
    obj->update();
    return 0;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "setInvert", setInvert },
    { "getInvert", getInvert },
    { "setLEDCount", setLEDCount },
    { "getLEDCount", getLEDCount },
    { "writeOne", writeOne },
    { "clearOne", clearOne },
    { "writeLine", writeLine },
    { "clearLine", clearLine },
    { "writeAll", writeAll },
    { "clearAll", clearAll },
    { "update", update },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_RGBLED( lua_State *L )
{
    //
    // Objects
    //

    // Create metatable
    luaL_newmetatable( L, EVN_CLASS_NAME );

    // metatable.__index = metatable
    lua_pushvalue( L, -1 );        // dup mt
    lua_setfield( L, -2, "__index" );

    luaL_setfuncs( L, methods, 0 );

    lua_pop( L, 1 );


    //
    // Class
    //

    // The object table
    lua_pushstring( L, LUA_CLASS_NAME );

    lua_newtable( L );
    luaL_setfuncs( L, funcs, 0 );

    // Class constants

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    int led_count = functionArgInt( L, 2, 8 );
    bool invert = functionArgBool( L, 3, false );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNRGBLED *p = new(rp) EVNRGBLED( port, led_count, invert );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
