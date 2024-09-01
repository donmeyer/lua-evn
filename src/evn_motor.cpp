// evn_motor.cpp

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

#include "evn_motor.h"



static int new_object( lua_State *L );



static int begin( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    obj->begin();
    return 0;
}


static int getPosition( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_pushnumber( L, obj->getPosition() );
    return 1;
}


static int getHeading( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_pushnumber( L, obj->getHeading() );
    return 1;
}


static int resetPosition( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    obj->resetPosition();
    return 0;
}


static int getSpeed( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_pushnumber( L, obj->getSpeed() );
    return 1;
}


static int runPWM( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float duty_cycle_pct = methodArgFloat( L, 1 );
    obj->runPWM( duty_cycle_pct );
    return 0;
}


static int runSpeed( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float dps = methodArgFloat( L, 1 );
    obj->runSpeed( dps );
    return 0;
}


static int runPosition( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float dps = methodArgFloat( L, 1 );
    float position = methodArgFloat( L, 2 );
    static int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->runPosition( dps, position, stop_action, wait );
    return 0;
}


static int runAngle( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float dps = methodArgFloat( L, 1 );
    float degrees = methodArgFloat( L, 2 );
    static int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->runAngle( dps, degrees, stop_action, wait );
    return 0;
}


static int runHeading( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float dps = methodArgFloat( L, 1 );
    float heading = methodArgFloat( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->runHeading( dps, heading, stop_action, wait );
    return 0;
}


static int runTime( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float dps = methodArgFloat( L, 1 );
    lua_Number time_ms = methodArgNumber( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->runTime( dps, time_ms, stop_action, wait );
    return 0;
}


int stop( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    obj->stop();
    return 0;
}


static int coast( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    obj->coast();
    return 0;
}


static int hold( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    obj->hold();
    return 0;
}


static int completed( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_pushboolean( L, obj->completed() );
    return 1;
}


static int stalled( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_pushboolean( L, obj->stalled() );
    return 1;
}


static int setPID( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float p = methodArgFloat( L, 1 );
    float i = methodArgFloat( L, 2 );
    float d = methodArgFloat( L, 3 );
    obj->setPID( p, i, d );
    return 0;
}


static int setAccel( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float accel_dps_sq = methodArgFloat( L, 1 );
    obj->setAccel( accel_dps_sq );
    return 0;
}


static int setDecel( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float decel_dps_sq = methodArgFloat( L, 1 );
    obj->setDecel( decel_dps_sq );
    return 0;
}


static int setMaxRPM( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    float max_rpm = methodArgFloat( L, 1 );
    obj->setMaxRPM( max_rpm );
    return 0;
}


static int setPPR( lua_State *L )
{
    EVNMotor *obj = (EVNMotor*)luaL_checkudata( L, 1, "EVNMotor" );
    lua_Number ppr = methodArgNumber( L, 1 );
    obj->setPPR( ppr );
    return 0;
}


//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "getPosition", getPosition },
    { "getHeading", getHeading },
    { "resetPosition", resetPosition },
    { "getSpeed", getSpeed },
    { "runPWM", runPWM },
    { "runSpeed", runSpeed },
    { "runPosition", runPosition },
    { "runAngle", runAngle },
    { "runHeading", runHeading },
    { "runTime", runTime },
    { "stop", stop },
    { "coast", coast },
    { "hold", hold },
    { "completed", completed },
    { "stalled", stalled },
    { "setPID", setPID },
    { "setAccel", setAccel },
    { "setDecel", setDecel },
    { "setMaxRPM", setMaxRPM },
    { "setPPR", setPPR },

    { NULL, NULL }
};

// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_motor( lua_State *L )
{
    //
    // Motor Objects
    //

    // Create metatable
    luaL_newmetatable( L, "EVNMotor" );

    // metatable.__index = metatable
    lua_pushvalue( L, -1 );        // dup mt
    lua_setfield( L, -2, "__index" );

    luaL_setfuncs( L, methods, 0 );

    lua_pop( L, 1 );

    //
    // Motor class
    //

    // The object table
    lua_pushstring( L, "Motor" );

    lua_newtable( L );
    luaL_setfuncs( L, funcs, 0 );

    // Class constants
    addIntegerConstant( L, "STOP_BRAKE", STOP_BRAKE );
    addIntegerConstant( L, "STOP_COAST", STOP_COAST );
    addIntegerConstant( L, "STOP_HOLD", STOP_HOLD );

    addIntegerConstant( L, "EV3_LARGE", EV3_LARGE );
    addIntegerConstant( L, "EV3_MED", EV3_MED );
    addIntegerConstant( L, "NXT_LARGE", NXT_LARGE );
    addIntegerConstant( L, "CUSTOM_MOTOR", CUSTOM_MOTOR );

    addIntegerConstant( L, "DIRECT", DIRECT );
    addIntegerConstant( L, "REVERSE", REVERSE );

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int motor_type = EV3_LARGE;
    int motor_dir = DIRECT;
    int enc_dir = DIRECT;
    int port;

    const int nargs = lua_gettop( L );
    switch( nargs )
    {
        case 4:
            enc_dir = luaL_checkinteger( L, 4 );
            // fall-through

        case 3:
            motor_dir = luaL_checkinteger( L, 3 );
            // fall-through

        case 2:
            motor_type = luaL_checkinteger( L, 2 );
            // fall-through

        case 1:
            port = luaL_checkinteger( L, 1 );
            break;

        default:
            return luaL_error( L, "Invalid number of arguments" );
            break;
    }


    void *rp = lua_newuserdata( L, sizeof(EVNMotor) );
    // ud --

    EVNMotor *p = new(rp) EVNMotor( port, motor_type, motor_dir, enc_dir );

    // Add metatable
    luaL_getmetatable( L, "EVNMotor" );
    lua_setmetatable( L, -2 );

    return 1;
}
