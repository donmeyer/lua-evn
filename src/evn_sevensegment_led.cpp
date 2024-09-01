// evn_sevensegment_led.cpp

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

#include "evn_sevensegment_led.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNSevenSegmentLED
#define EVN_CLASS_NAME      "EVNSevenSegmentLED"
#define LUA_CLASS_NAME      "SevenSegmentLED"



static int begin( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int setDisplayMode( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    EVN_HT16K33::mode mode = (EVN_HT16K33::mode)methodArgInt( L, 1 );
    obj->setDisplayMode( mode );
    return 0;
}


static int setBrightness( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int brightness = methodArgInt( L, 1 );
    obj->setBrightness( brightness );
    return 0;
}


static int getBrightness( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    lua_pushinteger( L, obj->getBrightness() );
    return 1;
}


static int getMode( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    lua_pushinteger( L, obj->getMode() );
    return 1;
}


static int writeRaw( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int led = methodArgInt( L, 1 );
    bool on = methodArgBool( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->writeRaw( led, on, show );
    return 0;
}


static int clearAll( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    bool show = methodArgBool( L, 1, true );
    obj->clearAll( show );
    return 0;
}


static int writeAll( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    bool show = methodArgBool( L, 1, true );
    obj->writeAll( show );
    return 0;
}


static int update( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    obj->update();
    return 0;
}


static int writeDigit( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int position = methodArgInt( L, 1 );
    int digit = methodArgInt( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->writeDigit( position, digit, show );
    return 0;
}


static int writeLetter( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int position = methodArgInt( L, 1 );
    const char *letter = methodArgString( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->writeLetter( position, *letter, show );
    return 0;
}


static int clearPosition( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int position = methodArgInt( L, 1 );
    bool clear_point = methodArgBool( L, 2, true );
    bool show = methodArgBool( L, 3, true );
    obj->clearPosition( position, clear_point, show );
    return 0;
}


static int writePoint( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int position = methodArgInt( L, 1 );
    bool on = methodArgBool( L, 2, true );
    bool show = methodArgBool( L, 3, true );
    obj->writePoint( position, on, show );
    return 0;
}


static int clearPoint( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    int position = methodArgInt( L, 1 );
    bool show = methodArgBool( L, 2, true );
    obj->clearPoint( position, show );
    return 0;
}


static int writeColon( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    bool on = methodArgBool( L, 1, true );
    bool show = methodArgBool( L, 2, true );
    obj->writeColon( on, show );
    return 0;
}


static int clearColon( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    bool show = methodArgBool( L, 1, true );
    obj->clearColon( show );
    return 0;
}


static int writeNumber( lua_State *L )
{
    EVNSevenSegmentLED *obj = (EVNSevenSegmentLED*)luaL_checkudata( L, 1, "EVNSevenSegmentLED" );
    lua_Number number = methodArgNumber( L, 1 );
    bool show = methodArgBool( L, 2, true );
    obj->writeNumber( number, show );
    return 0;
}



//==============================================================================================================

// Object methods
static const luaL_Reg methods[] = {
    { "begin", begin },
    { "setDisplayMode", setDisplayMode },
    { "setBrightness", setBrightness },
    { "getBrightness", getBrightness },
    { "getMode", getMode },
    { "writeRaw", writeRaw },
    { "clearAll", clearAll },
    { "writeAll", writeAll },
    { "update", update },
    { "writeDigit", writeDigit },
    { "writeLetter", writeLetter },
    { "clearPosition", clearPosition },
    { "writePoint", writePoint },
    { "clearPoint", clearPoint },
    { "writeColon", writeColon },
    { "clearColon", clearColon },
    { "writeNumber", writeNumber },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_sevensegment_led( lua_State *L )
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
    addIntegerConstant( L, "OFF", EVN_HT16K33::OFF );
    addIntegerConstant( L, "ON", EVN_HT16K33::ON );
    addIntegerConstant( L, "BLINK_HZ_2", EVN_HT16K33::BLINK_HZ_2 );
    addIntegerConstant( L, "BLINK_HZ_1", EVN_HT16K33::BLINK_HZ_1 );
    addIntegerConstant( L, "BLINK_HZ_0_5", EVN_HT16K33::BLINK_HZ_0_5 );

    lua_settable( L, -3 );
}


static int new_object( lua_State *L )
{
    int port = functionArgInt( L, 1 );

    void *rp = lua_newuserdata( L, sizeof(EVN_CLASS) );
    // ud --

    EVNSevenSegmentLED *p = new(rp) EVNSevenSegmentLED( port );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
