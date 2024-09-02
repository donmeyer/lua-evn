// evn_compass_sensor.cpp

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

#include "evn_compass_sensor.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNCompassSensor
#define EVN_CLASS_NAME      "EVNCompassSensor"
#define LUA_CLASS_NAME      "CompassSensor"



static int begin( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int isQMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    lua_pushboolean( L, obj->isQMC() );
    return 1;
}


static int isHMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    lua_pushboolean( L, obj->isHMC() );
    return 1;
}


static int setCalibration( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    float hard_x = methodArgFloat( L, 1, 0 );
    float hard_y = methodArgFloat( L, 2, 0 );
    float hard_z = methodArgFloat( L, 3, 0 );
    float soft_x_0 = methodArgFloat( L, 4, 1 );
    float soft_x_1 = methodArgFloat( L, 5, 0 );
    float soft_x_2 = methodArgFloat( L, 6, 0 );
    float soft_y_0 = methodArgFloat( L, 7, 0 );
    float soft_y_1 = methodArgFloat( L, 8, 1 );
    float soft_y_2 = methodArgFloat( L, 9, 0 );
    float soft_z_0 = methodArgFloat( L, 10, 0 );
    float soft_z_1 = methodArgFloat( L, 11, 0 );
    float soft_z_2 = methodArgFloat( L, 12, 1 );
    obj->setCalibration( hard_x, hard_y, hard_z, soft_x_0, soft_x_1, soft_x_2, soft_y_0, soft_y_1, soft_y_2, soft_z_0, soft_z_1, soft_z_2 );
    return 0;
}


static int setModeHMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::hmc_mode mode = (EVNCompassSensor::hmc_mode)methodArgInt( L, 1 );
    obj->setModeHMC( mode );
    return 0;
}


static int setModeQMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::qmc_mode mode = (EVNCompassSensor::qmc_mode)methodArgInt( L, 1 );
    obj->setModeQMC( mode );
    return 0;
}


static int setDataRateHMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::hmc_data_rate data_rate = (EVNCompassSensor::hmc_data_rate)methodArgInt( L, 1 );
    obj->setDataRateHMC( data_rate );
    return 0;
}


static int setDataRateQMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::qmc_data_rate data_rate = (EVNCompassSensor::qmc_data_rate)methodArgInt( L, 1 );
    obj->setDataRateQMC( data_rate );
    return 0;
}


static int setRangeHMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::hmc_range range = (EVNCompassSensor::hmc_range)methodArgInt( L, 1 );
    obj->setRangeHMC( range );
    return 0;
}


static int setRangeQMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::qmc_range range = (EVNCompassSensor::qmc_range)methodArgInt( L, 1 );
    obj->setRangeQMC( range );
    return 0;
}


static int setSamplesHMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::hmc_samples samples = (EVNCompassSensor::hmc_samples)methodArgInt( L, 1 );
    obj->setSamplesHMC( samples );
    return 0;
}


static int setSamplesQMC( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    EVNCompassSensor::qmc_samples samples = (EVNCompassSensor::qmc_samples)methodArgInt( L, 1 );
    obj->setSamplesQMC( samples );
    return 0;
}


static int isCalibrated( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    lua_pushboolean( L, obj->isCalibrated() );
    return 1;
}


static int readRawX( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRawX( blocking ) );
    return 1;
}


static int readRawY( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRawY( blocking ) );
    return 1;
}


static int readRawZ( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readRawZ( blocking ) );
    return 1;
}


static int readCalX( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readCalX( blocking ) );
    return 1;
}


static int readCalY( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readCalY( blocking ) );
    return 1;
}


static int readCalZ( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->readCalZ( blocking ) );
    return 1;
}


static int read( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    bool blocking = methodArgBool( L, 1, true );
    lua_pushnumber( L, obj->read( blocking ) );
    return 1;
}


static int setNorth( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    obj->setNorth();
    return 0;
}


static int setHeading( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    int heading = methodArgInt( L, 1 );
    obj->setHeading( heading );
    return 0;
}


static int setTopAxis( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    int axis = methodArgInt( L, 1 );
    obj->setTopAxis( axis );
    return 0;
}


static int setFrontAxis( lua_State *L )
{
    EVNCompassSensor *obj = (EVNCompassSensor*)luaL_checkudata( L, 1, "EVNCompassSensor" );
    int axis = methodArgInt( L, 1 );
    obj->setFrontAxis( axis );
    return 0;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "isQMC", isQMC },
    { "isHMC", isHMC },
    { "setCalibration", setCalibration },
    { "setModeHMC", setModeHMC },
    { "setModeQMC", setModeQMC },
    { "setDataRateHMC", setDataRateHMC },
    { "setDataRateQMC", setDataRateQMC },
    { "setRangeHMC", setRangeHMC },
    { "setRangeQMC", setRangeQMC },
    { "setSamplesHMC", setSamplesHMC },
    { "setSamplesQMC", setSamplesQMC },
    { "isCalibrated", isCalibrated },
    { "readRawX", readRawX },
    { "readRawY", readRawY },
    { "readRawZ", readRawZ },
    { "readCalX", readCalX },
    { "readCalY", readCalY },
    { "readCalZ", readCalZ },
    { "read", read },
    { "setNorth", setNorth },
    { "setHeading", setHeading },
    { "setTopAxis", setTopAxis },
    { "setFrontAxis", setFrontAxis },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_compass_sensor( lua_State *L )
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
    addIntegerConstant( L, "HMC_CONTINUOUS", (int)EVNCompassSensor::hmc_mode::CONTINUOUS );
    addIntegerConstant( L, "HMC_STANDBY", (int)EVNCompassSensor::hmc_mode::STANDBY );

    addIntegerConstant( L, "HMC_HZ_75", (int)EVNCompassSensor::hmc_data_rate::HZ_75 );
    addIntegerConstant( L, "HMC_HZ_30", (int)EVNCompassSensor::hmc_data_rate::HZ_30 );
    addIntegerConstant( L, "HMC_HZ_15", (int)EVNCompassSensor::hmc_data_rate::HZ_15 );
    addIntegerConstant( L, "HMC_HZ_7_5", (int)EVNCompassSensor::hmc_data_rate::HZ_7_5 );
    addIntegerConstant( L, "HMC_HZ_3", (int)EVNCompassSensor::hmc_data_rate::HZ_3 );
    addIntegerConstant( L, "HMC_HZ_1_5", (int)EVNCompassSensor::hmc_data_rate::HZ_1_5 );
    addIntegerConstant( L, "HMC_HZ_0_75", (int)EVNCompassSensor::hmc_data_rate::HZ_0_75 );

    addIntegerConstant( L, "HMC_GA_8_1", (int)EVNCompassSensor::hmc_range::GA_8_1 );
    addIntegerConstant( L, "HMC_GA_5_6", (int)EVNCompassSensor::hmc_range::GA_5_6 );
    addIntegerConstant( L, "HMC_GA_4_7", (int)EVNCompassSensor::hmc_range::GA_4_7 );
    addIntegerConstant( L, "HMC_GA_4", (int)EVNCompassSensor::hmc_range::GA_4 );
    addIntegerConstant( L, "HMC_GA_2_5", (int)EVNCompassSensor::hmc_range::GA_2_5 );
    addIntegerConstant( L, "HMC_GA_1_9", (int)EVNCompassSensor::hmc_range::GA_1_9 );
    addIntegerConstant( L, "HMC_GA_1_3", (int)EVNCompassSensor::hmc_range::GA_1_3 );
    addIntegerConstant( L, "HMC_GA_0_88", (int)EVNCompassSensor::hmc_range::GA_0_88 );

    addIntegerConstant( L, "HMC_X1", (int)EVNCompassSensor::hmc_samples::X1 );
    addIntegerConstant( L, "HMC_X2", (int)EVNCompassSensor::hmc_samples::X2 );
    addIntegerConstant( L, "HMC_X4", (int)EVNCompassSensor::hmc_samples::X4 );
    addIntegerConstant( L, "HMC_X8", (int)EVNCompassSensor::hmc_samples::X8 );


    addIntegerConstant( L, "QMC_CONTINUOUS", (int)EVNCompassSensor::qmc_mode::CONTINUOUS );
    addIntegerConstant( L, "QMC_STANDBY", (int)EVNCompassSensor::qmc_mode::STANDBY );

    addIntegerConstant( L, "QMC_HZ_10", (int)EVNCompassSensor::qmc_data_rate::HZ_10 );
    addIntegerConstant( L, "QMC_HZ_50", (int)EVNCompassSensor::qmc_data_rate::HZ_50 );
    addIntegerConstant( L, "QMC_HZ_100", (int)EVNCompassSensor::qmc_data_rate::HZ_100 );
    addIntegerConstant( L, "QMC_HZ_200", (int)EVNCompassSensor::qmc_data_rate::HZ_200 );

    addIntegerConstant( L, "QMC_GA_8", (int)EVNCompassSensor::qmc_range::GA_8 );
    addIntegerConstant( L, "QMC_GA_2", (int)EVNCompassSensor::qmc_range::GA_2 );

    addIntegerConstant( L, "QMC_X64", (int)EVNCompassSensor::qmc_samples::X64 );
    addIntegerConstant( L, "QMC_X128", (int)EVNCompassSensor::qmc_samples::X128 );
    addIntegerConstant( L, "QMC_X256", (int)EVNCompassSensor::qmc_samples::X256 );
    addIntegerConstant( L, "QMC_X512", (int)EVNCompassSensor::qmc_samples::X512 );


    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );
    float hard_x = functionArgFloat( L, 2, 0 );
    float hard_y = functionArgFloat( L, 3, 0 );
    float hard_z = functionArgFloat( L, 4, 0 );
    float soft_x_0 = functionArgFloat( L, 5, 1 );
    float soft_x_1 = functionArgFloat( L, 6, 0 );
    float soft_x_2 = functionArgFloat( L, 7, 0 );
    float soft_y_0 = functionArgFloat( L, 8, 0 );
    float soft_y_1 = functionArgFloat( L, 9, 1 );
    float soft_y_2 = functionArgFloat( L, 10, 0 );
    float soft_z_0 = functionArgFloat( L, 11, 0 );
    float soft_z_1 = functionArgFloat( L, 12, 0 );
    float soft_z_2 = functionArgFloat( L, 13, 1 );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNCompassSensor *p = new(rp) EVNCompassSensor( port, hard_x, hard_y, hard_z, soft_x_0, soft_x_1, soft_x_2, soft_y_0, soft_y_1, soft_y_2, soft_z_0, soft_z_1, soft_z_2 );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
