// lib_arduino.cpp

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
#include "lua.hpp"

#include "lua_tools.h"
#include "lib_arduino.h"



//----------------------------------------------------------
// Time
//----------------------------------------------------------

static int funcMillis( lua_State *L )
{
    // The return type is an unsigned long.
    // Since Lua does not have unsigned integers, we return a Lua Number, which is a double
    // as that can represent an unsigned 32-bit value.
    lua_pushnumber( L, millis() );
    return 1;
}


static int funcMicros( lua_State *L )
{
    // The return type is an unsigned long.
    // Since Lua does not have unsigned integers, we return a Lua Number, which is a double
    // as that can represent an unsigned 32-bit value.
    lua_pushnumber( L, micros() );
    return 1;
}


static int funcDelay( lua_State *L )
{
    lua_Number t = luaL_checknumber( L, 1 );

    if( t < 0 )
    {
        return luaL_error( L, "Delay time cannot be negative" );
    }

    delay( t );

    return 0;
}


static int funcDelayMicroseconds( lua_State *L )
{
    lua_Number t = luaL_checknumber( L, 1 );

    if( t < 0 )
    {
        return luaL_error( L, "Delay time cannot be negative" );
    }

    delayMicroseconds( t );

    return 0;
}




//----------------------------------------------------------
// Digital Pins
//----------------------------------------------------------

static int funcPinMode( lua_State *L )
{
    int pin = luaL_checkinteger( L, 1 );
    lua_Integer mode = luaL_checkinteger( L, 2 );

    pinMode( pin, mode );

    return 0;
}


static int funcDigitalWrite( lua_State *L )
{
    int pin = luaL_checkinteger( L, 1 );
    int state = luaL_checkinteger( L, 2 );

    digitalWrite( pin, state );

    return 0;
}


static int funcDigitalRead( lua_State *L )
{
    int pin = luaL_checkinteger( L, 1 );

    int v = digitalRead( pin );

    lua_pushinteger( L, v );

    return 1;
}


//----------------------------------------------------------
// Analog Pins
//----------------------------------------------------------

static int funcAnalogRead( lua_State *L )
{
    int pin = luaL_checkinteger( L, 1 );

    int v = analogRead( pin );

    lua_pushinteger( L, v );

    return 1;
}


static int funcAnalogReadResolution( lua_State *L )
{
    int bits = luaL_checkinteger( L, 1 );

    if( bits < 1 || bits > 32 )
    {
        return luaL_error( L, "Invalid %d bit analog read resolution. Must be 1-32", bits );
    }

    analogReadResolution( bits );

    return 0;
}


#if 0
static int funcAnalogReference( lua_State *L )
{
    int type = luaL_checkinteger( L, 1 );

    analogReference( type );

    return 0;
}
#endif

static int funcAnalogWrite( lua_State *L )
{
    int pin = luaL_checkinteger( L, 1 );
    int value = luaL_checkinteger( L, 2 );

    if( value < 1 || value > 32 )
    {
        return luaL_error( L, "Invalid analog write value of %d. Must be 0-255", value );
    }

    analogWrite( pin, value );

    return 0;
}


#if defined (ARDUINO_ARCH_RP2040)
static int funcAnalogWriteResolution( lua_State *L )
{
    int bits = luaL_checkinteger( L, 1 );

    if( bits < 1 || bits > 32 )
    {
        return luaL_error( L, "Invalid %d bit analog write resolution. Must be 1-32", bits );
    }

    analogWriteResolution( bits );

    return 0;
}
#endif


//----------------------------------------------------------
// Advanced IO
//----------------------------------------------------------


//----------------------------------------------------------
// SPI
//----------------------------------------------------------


//----------------------------------------------------------
// Wire
//----------------------------------------------------------




//------------------------------------------------------------------

static const luaL_Reg funcs[] = {
    // Time
    { "millis", funcMillis },
    { "micros", funcMicros },
    { "delay", funcDelay },
    { "delayMicroseconds", funcDelayMicroseconds },

    // Digital Pins
    { "pinMode", funcPinMode },
    { "digitalWrite", funcDigitalWrite },
    { "digitalRead", funcDigitalRead },

    // Analog Pins
    { "analogRead", funcAnalogRead },
    { "analogReadResolution", funcAnalogReadResolution },
    { "analogWritw", funcAnalogWrite },
    // { "analogReference", funcAnalogReference },      Not in ESP32 or RP2040
#if defined (ARDUINO_ARCH_RP2040)
    { "analogWriteResolution", funcAnalogWriteResolution },
#endif

    { NULL, NULL }
};


// This will be called by the Lua process to initialize the library.
int luaopen_arduino( lua_State *L )
{
    luaL_newlib( L, funcs );

    addIntegerConstant( L, "HIGH", HIGH );
    addIntegerConstant( L, "LOW", LOW );

    addIntegerConstant( L, "INPUT", INPUT );
    addIntegerConstant( L, "INPUT_PULLUP", INPUT_PULLUP );
    addIntegerConstant( L, "OUTPUT", OUTPUT );

    addIntegerConstant( L, "LED_BUILTIN", LED_BUILTIN );

#if defined (ARDUINO_ARCH_RP2040)
    addIntegerConstant( L, "A0", A0 );
    addIntegerConstant( L, "A1", A1 );
    addIntegerConstant( L, "A2", A2 );
    addIntegerConstant( L, "A3", A3 );
#elif defined (ESP32)
    addIntegerConstant( L, "A0", A0 );
    addIntegerConstant( L, "A1", A1 );
    addIntegerConstant( L, "A2", A2 );
    addIntegerConstant( L, "A3", A3 );
    addIntegerConstant( L, "A4", A4 );
    addIntegerConstant( L, "A5", A5 );
    addIntegerConstant( L, "A6", A6 );
    addIntegerConstant( L, "A7", A7 );
    addIntegerConstant( L, "A8", A8 );
    addIntegerConstant( L, "A9", A9 );
    addIntegerConstant( L, "A10", A10 );
    addIntegerConstant( L, "A11", A11 );
    addIntegerConstant( L, "A12", A12 );
    addIntegerConstant( L, "A13", A13 );
#else
    addIntegerConstant( L, "A0", A0 );
    addIntegerConstant( L, "A1", A1 );
    addIntegerConstant( L, "A2", A2 );
    addIntegerConstant( L, "A3", A3 );
    addIntegerConstant( L, "A4", A4 );
    addIntegerConstant( L, "A5", A5 );
    addIntegerConstant( L, "A6", A6 );
    addIntegerConstant( L, "A7", A7 );
    addIntegerConstant( L, "A8", A8 );
    addIntegerConstant( L, "A9", A9 );
    addIntegerConstant( L, "A10", A10 );
    addIntegerConstant( L, "A11", A11 );
    addIntegerConstant( L, "A12", A12 );
    addIntegerConstant( L, "A13", A13 );
    addIntegerConstant( L, "A14", A14 );
#endif

#if ! defined (ARDUINO_ARCH_RP2040)
    addIntegerConstant( L, "DEFAULT", DEFAULT );
    addIntegerConstant( L, "EXTERNAL", EXTERNAL );
#endif

    return 1;
}
