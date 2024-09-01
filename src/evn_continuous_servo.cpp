// evn_continuous_servo_.cpp

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

#include "evn_continuous_servo.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNContinuousServo
#define EVN_CLASS_NAME      "EVNContinuousServo"
#define LUA_CLASS_NAME      "ContinuousServo"



static int begin( lua_State *L )
{
    EVNContinuousServo *obj = (EVNContinuousServo*)luaL_checkudata( L, 1, "EVNContinuousServo" );
    obj->begin();
    return 0;
}


static int write( lua_State *L )
{
    EVNContinuousServo *obj = (EVNContinuousServo*)luaL_checkudata( L, 1, "EVNContinuousServo" );
    float duty_cycle_pct = methodArgFloat( L, 1 );
    obj->write( duty_cycle_pct );
    return 0;
}


static int writeMicroseconds( lua_State *L )
{
    EVNContinuousServo *obj = (EVNContinuousServo*)luaL_checkudata( L, 1, "EVNContinuousServo" );
    int pulse_us = methodArgInt( L, 1 );
    obj->writeMicroseconds( pulse_us );
    return 0;
}


//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "write", write },
    { "writeMicroseconds", writeMicroseconds },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_continuous_servo( lua_State *L )
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
    bool servo_dir = functionArgBool( L, 2, DIRECT );
    int min_pulse_us = functionArgInt( L, 3, 600 );
    int max_pulse_us = functionArgInt( L, 4, 2400 );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNContinuousServo *p = new(rp) EVNContinuousServo( port, servo_dir, min_pulse_us, max_pulse_us );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
