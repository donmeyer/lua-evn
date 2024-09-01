// evn_servo.cpp

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

#include "evn_servo.h"


static int new_object( lua_State *L );



static int begin( lua_State *L )
{
    EVNServo *obj = (EVNServo*)luaL_checkudata( L, 1, "EVNServo" );
    obj->begin();
    return 0;
}


static int write( lua_State *L )
{
    EVNServo *obj = (EVNServo*)luaL_checkudata( L, 1, "EVNServo" );
    float position = methodArgFloat( L, 1 );
    int wait_time_ms = methodArgInt( L, 2, 0 );
    float dps = methodArgFloat( L, 3, 0 );
    obj->write( position, wait_time_ms, dps );
    return 0;
}


static int writeMicroseconds( lua_State *L )
{
    EVNServo *obj = (EVNServo*)luaL_checkudata( L, 1, "EVNServo" );
    int pulse_us = methodArgInt( L, 1 );
    int wait_time_ms = methodArgInt( L, 2, 0 );
    obj->writeMicroseconds( pulse_us, wait_time_ms );
    return 0;
}


static int getRange( lua_State *L )
{
    EVNServo *obj = (EVNServo*)luaL_checkudata( L, 1, "EVNServo" );
    lua_pushinteger( L, obj->getRange() );
    return 1;
}


static int getMaxDPS( lua_State *L )
{
    EVNServo *obj = (EVNServo*)luaL_checkudata( L, 1, "EVNServo" );
    lua_pushnumber( L, obj->getMaxDPS() );
    return 1;
}


//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "write", write },
    { "writeMicroseconds", writeMicroseconds },
    { "getRange", getRange },
    { "getMaxDPS", getMaxDPS },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_servo( lua_State *L )
{
    //
    // Servo Objects
    //

    // Create metatable
    luaL_newmetatable( L, "EVNServo" );

    // metatable.__index = metatable
    lua_pushvalue( L, -1 );        // dup mt
    lua_setfield( L, -2, "__index" );

    luaL_setfuncs( L, methods, 0 );

    lua_pop( L, 1 );


    //
    // Servo class
    //

    // The object table
    lua_pushstring( L, "Servo" );

    lua_newtable( L );
    luaL_setfuncs( L, funcs, 0 );

    // Class constants
    addIntegerConstant( L, "DIRECT", DIRECT );
    addIntegerConstant( L, "REVERSE", REVERSE );

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    bool servo_dir = functionArgBool( L, 2, DIRECT );
    int range = functionArgInt( L, 3, 270 );
    float start_position = functionArgFloat( L, 4, 135 );
    int min_pulse_us = functionArgInt( L, 5, 600 );
    int max_pulse_us = functionArgInt( L, 6, 2400 );
    float max_dps = functionArgFloat( L, 7, 500 );

    void *rp = lua_newuserdata( L, sizeof(EVNServo) );
    // ud --

    EVNServo *p = new(rp) EVNServo( port, servo_dir, range, start_position, min_pulse_us, max_pulse_us, max_dps );

    // Add metatable
    luaL_getmetatable( L, "EVNServo" );
    lua_setmetatable( L, -2 );

    return 1;
}
