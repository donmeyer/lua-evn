// evn_colour_sensor.cpp

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

#include "evn_colour_sensor.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNColourSensor
#define EVN_CLASS_NAME      "EVNColourSensor"
#define LUA_CLASS_NAME      "ColourSensor"



static int begin( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int setGain( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    EVNColourSensor::gain gain = (EVNColourSensor::gain)methodArgInt( L, 1 );
    obj->setGain( gain );
    return 0;
}


static int setIntegrationCycles( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    int integration_cycles = methodArgInt( L, 1 );
    obj->setIntegrationCycles( integration_cycles );
    return 0;
}


static int setRedRange( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    int low = methodArgInt( L, 1 );
    int high = methodArgInt( L, 2 );
    obj->setRedRange( low, high );
    return 0;
}


static int setGreenRange( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    int low = methodArgInt( L, 1 );
    int high = methodArgInt( L, 2 );
    obj->setGreenRange( low, high );
    return 0;
}


static int setBlueRange( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    int low = methodArgInt( L, 1 );
    int high = methodArgInt( L, 2 );
    obj->setBlueRange( low, high );
    return 0;
}


static int setClearRange( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    int low = methodArgInt( L, 1 );
    int high = methodArgInt( L, 2 );
    obj->setClearRange( low, high );
    return 0;
}


static int read( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->read( blocking ) );
    return 1;
}


static int readRed( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->readRed( blocking ) );
    return 1;
}


static int readGreen( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->readGreen( blocking ) );
    return 1;
}


static int readBlue( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->readBlue( blocking ) );
    return 1;
}


static int readClear( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushinteger( L, obj->readClear( blocking ) );
    return 1;
}


static int readRedNorm( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRedNorm( blocking ) );
    return 1;
}


static int readGreenNorm( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readGreenNorm( blocking ) );
    return 1;
}


static int readBlueNorm( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readBlueNorm( blocking ) );
    return 1;
}


static int readClearNorm( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readClearNorm( blocking ) );
    return 1;
}


static int readClearPCT( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readClearPCT( blocking ) );
    return 1;
}


static int readRedPCT( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRedPCT( blocking ) );
    return 1;
}


static int readGreenPCT( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readGreenPCT( blocking ) );
    return 1;
}


static int readBluePCT( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readBluePCT( blocking ) );
    return 1;
}


static int readHueHSV( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readHueHSV( blocking ) );
    return 1;
}


static int readSaturationHSV( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readSaturationHSV( blocking ) );
    return 1;
}


static int readValueHSV( lua_State *L )
{
    EVNColourSensor *obj = (EVNColourSensor*)luaL_checkudata( L, 1, "EVNColourSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readValueHSV( blocking ) );
    return 1;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "setGain", setGain },
    { "setIntegrationCycles", setIntegrationCycles },
    { "setRedRange", setRedRange },
    { "setGreenRange", setGreenRange },
    { "setBlueRange", setBlueRange },
    { "setClearRange", setClearRange },
    { "read", read },
    { "readRed", readRed },
    { "readGreen", readGreen },
    { "readBlue", readBlue },
    { "readClear", readClear },
    { "readRedNorm", readRedNorm },
    { "readGreenNorm", readGreenNorm },
    { "readBlueNorm", readBlueNorm },
    { "readClearNorm", readClearNorm },
    { "readClearPCT", readClearPCT },
    { "readRedPCT", readRedPCT },
    { "readGreenPCT", readGreenPCT },
    { "readBluePCT", readBluePCT },
    { "readHueHSV", readHueHSV },
    { "readSaturationHSV", readSaturationHSV },
    { "readValueHSV", readValueHSV },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_colour_sensor( lua_State *L )
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
    addIntegerConstant( L, "X1", (int)EVNColourSensor::gain::X1 );
    addIntegerConstant( L, "X4", (int)EVNColourSensor::gain::X4);
    addIntegerConstant( L, "X16", (int)EVNColourSensor::gain::X16 );
    addIntegerConstant( L, "X64", (int)EVNColourSensor::gain::X64 );

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    int integration_cycles = functionArgInt( L, 2, 1 );
    EVNColourSensor::gain gain = (EVNColourSensor::gain)functionArgInt( L, 3, (int)EVNColourSensor::gain::X16 );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNColourSensor *p = new(rp) EVNColourSensor( port, integration_cycles, gain );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
