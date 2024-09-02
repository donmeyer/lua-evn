// evn_imu_sensor.cpp

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

#include "evn_imu_sensor.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNIMUSensor
#define EVN_CLASS_NAME      "EVNIMUSensor"
#define LUA_CLASS_NAME      "IMUSensor"



static int begin( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool calibrate_gyro = methodArgBool( L, 1, true );
    lua_pushboolean( L, obj->begin( calibrate_gyro ) );
    return 1;
}


static int setAccelRange( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    EVNIMUSensor::accel_range range = (EVNIMUSensor::accel_range)methodArgInt( L, 1 );
    obj->setAccelRange( range );
    return 0;
}


static int setGyroRange( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    EVNIMUSensor::gyro_range range = (EVNIMUSensor::gyro_range)methodArgInt( L, 1 );
    obj->setGyroRange( range );
    return 0;
}


static int setDataRate( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    EVNIMUSensor::data_rate data_rate = (EVNIMUSensor::data_rate)methodArgInt( L, 1 );
    obj->setDataRate( data_rate );
    return 0;
}


static int setCalibrationGyro( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    float gx_offset = methodArgFloat( L, 1 );
    float gy_offset = methodArgFloat( L, 2 );
    float gz_offset = methodArgFloat( L, 3 );
    obj->setCalibrationGyro( gx_offset, gy_offset, gz_offset );
    return 0;
}


static int setCalibrationAccel( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    float ax_low = methodArgFloat( L, 1 );
    float ax_high = methodArgFloat( L, 2 );
    float ay_low = methodArgFloat( L, 3 );
    float ay_high = methodArgFloat( L, 4 );
    float az_low = methodArgFloat( L, 5 );
    float az_high = methodArgFloat( L, 6 );
    obj->setCalibrationAccel( ax_low, ax_high, ay_low, ay_high, az_low, az_high );
    return 0;
}


static int read( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->read( blocking ) );
    return 1;
}


static int readYaw( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readYaw( blocking ) );
    return 1;
}


static int readRoll( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRoll( blocking ) );
    return 1;
}


static int readPitch( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readPitch( blocking ) );
    return 1;
}


static int readYawRadians( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readYawRadians( blocking ) );
    return 1;
}


static int readRollRadians( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRollRadians( blocking ) );
    return 1;
}


static int readPitchRadians( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readPitchRadians( blocking ) );
    return 1;
}


static int readAccelX( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readAccelX( blocking ) );
    return 1;
}


static int readAccelY( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readAccelY( blocking ) );
    return 1;
}


static int readAccelZ( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readAccelZ( blocking ) );
    return 1;
}


static int readGyroX( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readGyroX( blocking ) );
    return 1;
}


static int readGyroY( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readGyroY( blocking ) );
    return 1;
}


static int readGyroZ( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readGyroZ( blocking ) );
    return 1;
}


static int linkCompass( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    EVNCompassSensor *compass = (EVNCompassSensor*) luaL_checkudata( L, 1, "EVNCompassSensor" );
    obj->linkCompass( compass );
    return 0;
}


static int setTopAxis( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    int axis = methodArgInt( L, 1 );
    obj->setTopAxis( axis );
    return 0;
}


static int setFrontAxis( lua_State *L )
{
    EVNIMUSensor *obj = (EVNIMUSensor*)luaL_checkudata( L, 1, "EVNIMUSensor" );
    int axis = methodArgInt( L, 1 );
    obj->setFrontAxis( axis );
    return 0;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "setAccelRange", setAccelRange },
    { "setGyroRange", setGyroRange },
    { "setDataRate", setDataRate },
    { "setCalibrationGyro", setCalibrationGyro },
    { "setCalibrationAccel", setCalibrationAccel },
    { "read", read },
    { "readYaw", readYaw },
    { "readRoll", readRoll },
    { "readPitch", readPitch },
    { "readYawRadians", readYawRadians },
    { "readRollRadians", readRollRadians },
    { "readPitchRadians", readPitchRadians },
    { "readAccelX", readAccelX },
    { "readAccelY", readAccelY },
    { "readAccelZ", readAccelZ },
    { "readGyroX", readGyroX },
    { "readGyroY", readGyroY },
    { "readGyroZ", readGyroZ },
    { "linkCompass", linkCompass },
    { "setTopAxis", setTopAxis },
    { "setFrontAxis", setFrontAxis },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_imu_sensor( lua_State *L )
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
    addIntegerConstant( L, "DPS_250", (int)EVNIMUSensor::gyro_range::DPS_250 );
    addIntegerConstant( L, "DPS_500", (int)EVNIMUSensor::gyro_range::DPS_500 );
    addIntegerConstant( L, "DPS_1000", (int)EVNIMUSensor::gyro_range::DPS_1000 );
    addIntegerConstant( L, "DPS_2000", (int)EVNIMUSensor::gyro_range::DPS_2000 );

    addIntegerConstant( L, "G_2", (int)EVNIMUSensor::accel_range::G_2 );
    addIntegerConstant( L, "G_4", (int)EVNIMUSensor::accel_range::G_4 );
    addIntegerConstant( L, "G_8", (int)EVNIMUSensor::accel_range::G_8 );
    addIntegerConstant( L, "G_16", (int)EVNIMUSensor::accel_range::G_16 );

    addIntegerConstant( L, "HZ_184", (int)EVNIMUSensor::data_rate::HZ_184 );
    addIntegerConstant( L, "HZ_92", (int)EVNIMUSensor::data_rate::HZ_92 );
    addIntegerConstant( L, "HZ_41", (int)EVNIMUSensor::data_rate::HZ_41 );
    addIntegerConstant( L, "HZ_20", (int)EVNIMUSensor::data_rate::HZ_20 );
    addIntegerConstant( L, "HZ_10", (int)EVNIMUSensor::data_rate::HZ_10 );
    addIntegerConstant( L, "HZ_5", (int)EVNIMUSensor::data_rate::HZ_5 );

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    float gx_offset = functionArgFloat( L, 2, 0 );
    float gy_offset = functionArgFloat( L, 3, 0 );
    float gz_offset = functionArgFloat( L, 4, 0 );
    float ax_low = functionArgFloat( L, 5, 0 );
    float ax_high = functionArgFloat( L, 6, 0 );
    float ay_low = functionArgFloat( L, 7, 0 );
    float ay_high = functionArgFloat( L, 8, 0 );
    float az_low = functionArgFloat( L, 9, 0 );
    float az_high = functionArgFloat( L, 10, 0 );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNIMUSensor *p = new(rp) EVNIMUSensor( port, gx_offset, gy_offset, gz_offset, ax_low, ax_high, ay_low, ay_high, az_low, az_high );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
