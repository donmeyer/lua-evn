// evn_distance_sensor.cpp

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

#include "evn_distance_sensor.h"


static int new_object( lua_State *L );



static int begin( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int setSignalRateLimit( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    float limit = methodArgFloat( L, 1 );
    obj->setSignalRateLimit( limit );
    return 0;
}


static int setPulsePeriodPreRange( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    int period = methodArgInt( L, 1 );
    obj->setPulsePeriodPreRange( period );
    return 0;
}


static int setPulsePeriodFinalRange( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    int period = methodArgInt( L, 1 );
    obj->setPulsePeriodFinalRange( period );
    return 0;
}


static int setTimingBudget( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    int timing_budget_ms = methodArgInt( L, 1 );
    obj->setTimingBudget( timing_budget_ms );
    return 0;
}


static int getTimingBudget( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    lua_pushinteger( L, obj->getTimingBudget() );
    return 1;
}


static int read( lua_State *L )
{
    EVNDistanceSensor *obj = (EVNDistanceSensor*)luaL_checkudata( L, 1, "EVNDistanceSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->read( blocking ) );
    return 1;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "setSignalRateLimit", setSignalRateLimit },
    { "setPulsePeriodPreRange", setPulsePeriodPreRange },
    { "setPulsePeriodFinalRange", setPulsePeriodFinalRange },
    { "setTimingBudget", setTimingBudget },
    { "getTimingBudget", getTimingBudget },
    { "read", read },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_distance_sensor( lua_State *L )
{
    //
    // Objects
    //

    // Create metatable
    luaL_newmetatable( L, "EVNDistanceSensor" );

    // metatable.__index = metatable
    lua_pushvalue( L, -1 );        // dup mt
    lua_setfield( L, -2, "__index" );

    luaL_setfuncs( L, methods, 0 );

    lua_pop( L, 1 );


    //
    // Class
    //

    // The object table
    lua_pushstring( L, "DistanceSensor" );

    lua_newtable( L );
    luaL_setfuncs( L, funcs, 0 );

    // Class constants

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    int timing_budget_ms = functionArgInt( L, 2, 33 );

    void *rp = lua_newuserdata( L, sizeof(EVNDistanceSensor) );
    // ud --

    EVNDistanceSensor *p = new(rp) EVNDistanceSensor( port, timing_budget_ms );

    // Add metatable
    luaL_getmetatable( L, "EVNDistanceSensor" );
    lua_setmetatable( L, -2 );

    return 1;
}
