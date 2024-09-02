// lib_evn_board.cpp

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

#include "lib_evn_board.h"

#include "evn_motor.h"
#include "evn_servo.h"
#include "evn_continuous_servo.h"
#include "evn_drivebase.h"

#include "evn_distance_sensor.h"
#include "evn_colour_sensor.h"
#include "evn_compass_sensor.h"
#include "evn_imu_sensor.h"

#include "evn_display.h"
#include "evn_matrixled.h"
#include "evn_sevensegment_led.h"
#include "evn_RGBLED.h"



static EVNAlpha *board;



static int buttonRead( lua_State *L )
{
    lua_pushboolean( L, board->buttonRead() );

    return 1;
}


static int ledWrite( lua_State *L )
{
    luaL_checktype( L, 1, LUA_TBOOLEAN );

    bool state = lua_toboolean( L, 1 );

    board->ledWrite( state );

    return 0;
}


//-----------------------------------------
// I2C
//-----------------------------------------

static int setPort( lua_State *L )
{
    int port = luaL_checkinteger( L, 1 );

    board->setPort( port );

    return 0;
}


static int getPort( lua_State *L )
{
    lua_pushinteger( L, board->getPort() );
    return 1;
}


static int getWirePort( lua_State *L )
{
    lua_pushinteger( L, board->getWirePort() );
    return 1;
}


static int getWire1Port( lua_State *L )
{
    lua_pushinteger( L, board->getWire1Port() );
    return 1;
}


static int printPorts( lua_State *L )
{
    board->printPorts();
    return 0;
}


//-----------------------------------------
// Battery Voltage
//-----------------------------------------

static int getBatteryVoltage( lua_State *L )
{
    int v;

    // Note that this code could be simpler if we used the default argument values here,
    // but that would require keeping those defaults in sync between this code and
    // the EVN library. Calling the method with each possible argument list is clunkier,
    // but more proper.

    int n = lua_gettop( L );
    if( n >= 2 )
    {
        // Both args
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        int threshold = luaL_checkinteger( L, 2 );

        v = board->getBatteryVoltage( flash, threshold );
    }
    else if( n >= 1 )
    {
        // One arg
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        v = board->getBatteryVoltage( flash );
    }
    else
    {
        // No args
        v = board->getBatteryVoltage();
    }

    lua_pushinteger( L, v );
    return 1;
}


static int getCell1Voltage( lua_State *L )
{
    int v;

    // Note that this code could be simpler if we used the default argument values here,
    // but that would require keeping those defaults in sync between this code and
    // the EVN library. Calling the method with each possible argument list is clunkier,
    // but more proper.

    int n = lua_gettop( L );
    if( n >= 2 )
    {
        // Both args
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        int threshold = luaL_checkinteger( L, 2 );

        v = board->getCell1Voltage( flash, threshold );
    }
    else if( n >= 1 )
    {
        // One arg
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        v = board->getCell1Voltage( flash );
    }
    else
    {
        // No args
        v = board->getCell1Voltage();
    }

    lua_pushinteger( L, v );
    return 1;
}


static int getCell2Voltage( lua_State *L )
{
    int v;

    // Note that this code could be simpler if we used the default argument values here,
    // but that would require keeping those defaults in sync between this code and
    // the EVN library. Calling the method with each possible argument list is clunkier,
    // but more proper.

    int n = lua_gettop( L );
    if( n >= 2 )
    {
        // Both args
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        int threshold = luaL_checkinteger( L, 2 );

        v = board->getCell2Voltage( flash, threshold );
    }
    else if( n >= 1 )
    {
        // One arg
        luaL_checktype( L, 1, LUA_TBOOLEAN );
        bool flash = lua_toboolean( L, 1 );

        v = board->getCell2Voltage( flash );
    }
    else
    {
        // No args
        v = board->getCell2Voltage();
    }

    lua_pushinteger( L, v );
    return 1;
}


//-----------------------------------------
// Set Functions
//-----------------------------------------

static int setMode( lua_State *L )
{
    int mode = luaL_checkinteger( L, 1 );

    board->setMode( mode );

    return 0;
}


static int setLinkLED( lua_State *L )
{
    luaL_checktype( L, 1, LUA_TBOOLEAN );
    bool enable = lua_toboolean( L, 1 );

    board->setLinkLED( enable );

    return 0;
}


static int setLinkMovement( lua_State *L )
{
    luaL_checktype( L, 1, LUA_TBOOLEAN );
    bool enable = lua_toboolean( L, 1 );

    board->setLinkMovement( enable );

    return 0;
}


static int setButtonInvert( lua_State *L )
{
    luaL_checktype( L, 1, LUA_TBOOLEAN );
    bool enable = lua_toboolean( L, 1 );

    board->setButtonInvert( enable );

    return 0;
}



//-----------------------------------------
// Get Functions
//-----------------------------------------

static int getMode( lua_State *L )
{
    lua_pushinteger( L, board->getMode() );
    return 1;
}


static int getLinkLED( lua_State *L )
{
    lua_pushboolean( L, board->getLinkLED() );
    return 1;
}


static int getLinkMovement( lua_State *L )
{
    lua_pushboolean( L, board->getLinkMovement() );
    return 1;
}


static int getButtonInvert( lua_State *L )
{
    lua_pushboolean( L, board->getButtonInvert() );
    return 1;
}


//===========================================================================================


static const luaL_Reg funcs[] = {
    { "buttonRead", buttonRead },
    { "ledWrite", ledWrite },

    { "setPort", setPort },
    { "getPort", getPort },
    { "getWirePort", getWirePort },
    { "getWire1Port", getWire1Port },
    { "printPorts", printPorts },

    { "getBatteryVoltage", getBatteryVoltage },
    { "getCell1Voltage", getCell1Voltage },
    { "getCell2Voltage", getCell2Voltage },

    { "setMode", setMode },
    { "setLinkLED", setLinkLED },
    { "setLinkMovement", setLinkMovement },
    { "setButtonInvert", setButtonInvert },

    { "getMode", getMode },
    { "getLinkLED", getLinkLED },
    { "getLinkMovement", getLinkMovement },
    { "getButtonInvert", getButtonInvert },

    { NULL, NULL }
};


/**
 * Set the EVNBoard pointer to the board object created
 * in the main 'ino' file.
 *
 * This MUST be done before calling any of this library's functions.
 */
void lib_evn_set_board( EVNAlpha *_board )
{
    board = _board;
}


// This will be called by the Lua process to initialize the library.
int luaopen_evn_board( lua_State *L )
{
    luaL_newlib( L, funcs );

    init_evn_motor( L );
    init_evn_servo( L );
    init_evn_continuous_servo( L );
    init_evn_drivebase( L );

    init_evn_distance_sensor( L );
    init_evn_colour_sensor( L );
    init_evn_compass_sensor( L );
    init_evn_imu_sensor( L );

    init_evn_display( L );
    init_evn_matrixled( L );
    init_evn_RGBLED( L );
    init_evn_sevensegment_led( L );

    addIntegerConstant( L, "BUTTON_TOGGLE", BUTTON_TOGGLE );
    addIntegerConstant( L, "BUTTON_PUSHBUTTON", BUTTON_PUSHBUTTON );
    addIntegerConstant( L, "BUTTON_DISABLE", BUTTON_DISABLE );

    return 1;
}
