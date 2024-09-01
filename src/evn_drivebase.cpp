// evn_drivebase.cpp

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

#include "evn_drivebase.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNDrivebase
#define EVN_CLASS_NAME      "EVNDrivebase"
#define LUA_CLASS_NAME      "Drivebase"



static int begin( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    obj->begin();
    return 0;
}


static int drivePct( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed_outer_pct = methodArgFloat( L, 1 );
    float turn_rate_pct = methodArgFloat( L, 2 );
    obj->drivePct( speed_outer_pct, turn_rate_pct );
    return 0;
}


static int drive( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float turn_rate = methodArgFloat( L, 2 );
    obj->drive( speed, turn_rate );
    return 0;
}


static int driveTurnRate( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float turn_rate = methodArgFloat( L, 2 );
    obj->driveTurnRate( speed, turn_rate );
    return 0;
}


static int driveRadius( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float radius = methodArgFloat( L, 2 );
    obj->driveRadius( speed, radius );
    return 0;
}


static int straight( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float distance = methodArgFloat( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->straight( speed, distance, stop_action, wait );
    return 0;
}


static int curve( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float radius = methodArgFloat( L, 2 );
    float angle = methodArgFloat( L, 3 );
    int stop_action = methodArgInt( L, 4, STOP_BRAKE );
    bool wait = methodArgBool( L, 5, true );
    obj->curve( speed, radius, angle, stop_action, wait );
    return 0;
}


static int curveRadius( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float radius = methodArgFloat( L, 2 );
    float angle = methodArgFloat( L, 3 );
    int stop_action = methodArgInt( L, 4, STOP_BRAKE );
    bool wait = methodArgBool( L, 5, true );
    obj->curveRadius( speed, radius, angle, stop_action, wait );
    return 0;
}


static int curveTurnRate( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float turn_rate = methodArgFloat( L, 2 );
    float angle = methodArgFloat( L, 3 );
    int stop_action = methodArgInt( L, 4, STOP_BRAKE );
    bool wait = methodArgBool( L, 5, true );
    obj->curveTurnRate( speed, turn_rate, angle, stop_action, wait );
    return 0;
}


static int turn( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float turn_rate = methodArgFloat( L, 1 );
    float degrees = methodArgFloat( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->turn( turn_rate, degrees, stop_action, wait );
    return 0;
}


static int turnDegrees( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float turn_rate = methodArgFloat( L, 1 );
    float degrees = methodArgFloat( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->turnDegrees( turn_rate, degrees, stop_action, wait );
    return 0;
}


static int turnHeading( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float turn_rate = methodArgFloat( L, 1 );
    float heading = methodArgFloat( L, 2 );
    int stop_action = methodArgInt( L, 3, STOP_BRAKE );
    bool wait = methodArgBool( L, 4, true );
    obj->turnHeading( turn_rate, heading, stop_action, wait );
    return 0;
}


static int driveToXY( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed = methodArgFloat( L, 1 );
    float turn_rate = methodArgFloat( L, 2 );
    float x = methodArgFloat( L, 3 );
    float y = methodArgFloat( L, 4 );
    int stop_action = methodArgInt( L, 5, STOP_BRAKE );
    bool restore_initial_heading = methodArgBool( L, 6, true );
    obj->driveToXY( speed, turn_rate, x, y, stop_action, restore_initial_heading );
    return 0;
}


static int stop( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    obj->stop();
    return 0;
}


static int coast( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    obj->coast();
    return 0;
}


static int hold( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    obj->hold();
    return 0;
}


static int completed( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushboolean( L, obj->completed() );
    return 1;
}


static int setSpeedPID( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float kp = methodArgFloat( L, 1 );
    float ki = methodArgFloat( L, 2 );
    float kd = methodArgFloat( L, 3 );
    obj->setSpeedPID( kp, ki, kd );
    return 0;
}


static int setTurnRatePID( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float kp = methodArgFloat( L, 1 );
    float ki = methodArgFloat( L, 2 );
    float kd = methodArgFloat( L, 3 );
    obj->setTurnRatePID( kp, ki, kd );
    return 0;
}


static int setSpeedAccel( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed_accel = methodArgFloat( L, 1 );
    obj->setSpeedAccel( speed_accel );
    return 0;
}


static int setSpeedDecel( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float speed_decel = methodArgFloat( L, 1 );
    obj->setSpeedDecel( speed_decel );
    return 0;
}


static int setTurnRateAccel( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float turn_rate_accel = methodArgFloat( L, 1 );
    obj->setTurnRateAccel( turn_rate_accel );
    return 0;
}


static int setTurnRateDecel( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float turn_rate_decel = methodArgFloat( L, 1 );
    obj->setTurnRateDecel( turn_rate_decel );
    return 0;
}


static int getDistance( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushnumber( L, obj->getDistance() );
    return 1;
}


static int getAngle( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushnumber( L, obj->getAngle() );
    return 1;
}


static int getHeading( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushnumber( L, obj->getHeading() );
    return 1;
}


static int getX( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushnumber( L, obj->getX() );
    return 1;
}


static int getY( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    lua_pushnumber( L, obj->getY() );
    return 1;
}


static int resetXY( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    obj->resetXY();
    return 0;
}


static int getDistanceToPoint( lua_State *L )
{
    EVNDrivebase *obj = (EVNDrivebase*)luaL_checkudata( L, 1, "EVNDrivebase" );
    float x = methodArgFloat( L, 1 );
    float y = methodArgFloat( L, 2 );
    lua_pushnumber( L, obj->getDistanceToPoint( x, y ) );
    return 1;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "drivePct", drivePct },
    { "drive", drive },
    { "driveTurnRate", driveTurnRate },
    { "driveRadius", driveRadius },
    { "straight", straight },
    { "curve", curve },
    { "curveRadius", curveRadius },
    { "curveTurnRate", curveTurnRate },
    { "turn", turn },
    { "turnDegrees", turnDegrees },
    { "turnHeading", turnHeading },
    { "driveToXY", driveToXY },
    { "stop", stop },
    { "coast", coast },
    { "hold", hold },
    { "completed", completed },
    { "setSpeedPID", setSpeedPID },
    { "setTurnRatePID", setTurnRatePID },
    { "setSpeedAccel", setSpeedAccel },
    { "setSpeedDecel", setSpeedDecel },
    { "setTurnRateAccel", setTurnRateAccel },
    { "setTurnRateDecel", setTurnRateDecel },
    { "getDistance", getDistance },
    { "getAngle", getAngle },
    { "getHeading", getHeading },
    { "getX", getX },
    { "getY", getY },
    { "resetXY", resetXY },
    { "getDistanceToPoint", getDistanceToPoint },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_drivebase( lua_State *L )
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
    float wheel_dia = functionArgFloat( L, 1 );
    float axle_track = functionArgFloat( L, 2 );
    EVNMotor *motor_left = (EVNMotor*)luaL_checkudata( L, 3, "EVNMotor" );
    EVNMotor *motor_right = (EVNMotor*)luaL_checkudata( L, 4, "EVNMotor" );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNDrivebase *p = new(rp) EVNDrivebase( wheel_dia, axle_track, motor_left, motor_right );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
