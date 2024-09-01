// evn_matrixled.cpp

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

#include "evn_matrixled.h"


static int new_object( lua_State *L );

#define EVN_CLASS           EVNMatrixLED
#define EVN_CLASS_NAME      "EVNMatrixLED"
#define LUA_CLASS_NAME      "MatrixLED"


static int begin( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushboolean( L, obj->begin() );
    return 1;
}


static int setDisplayMode( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    EVN_HT16K33::mode mode = (EVN_HT16K33::mode)methodArgInt( L, 1 );
    obj->setDisplayMode( mode );
    return 0;
}


static int setBrightness( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int brightness = methodArgInt( L, 1 );
    obj->setBrightness( brightness );
    return 0;
}


static int getBrightness( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushinteger( L, obj->getBrightness() );
    return 1;
}


static int getMode( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushinteger( L, obj->getMode() );
    return 1;
}


static int writeRaw( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int led = methodArgInt( L, 1 );
    bool on = methodArgBool( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->writeRaw( led, on, show );
    return 0;
}


static int clearAll( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    bool show = methodArgBool( L, 1, true );
    obj->clearAll( show );
    return 0;
}


static int writeAll( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    bool show = methodArgBool( L, 1, true );
    obj->writeAll( show );
    return 0;
}


static int update( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    obj->update();
    return 0;
}



static int setInvertX( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    bool enable = methodArgBool( L, 1 );
    obj->setInvertX( enable );
    return 0;
}


static int setInvertY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    bool enable = methodArgBool( L, 1 );
    obj->setInvertY( enable );
    return 0;
}


static int setSwapXY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    bool enable = methodArgBool( L, 1 );
    obj->setSwapXY( enable );
    return 0;
}


static int getInvertX( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushboolean( L, obj->getInvertX() );
    return 1;
}


static int getInvertY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushboolean( L, obj->getInvertY() );
    return 1;
}


static int getSwapXY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    lua_pushboolean( L, obj->getSwapXY() );
    return 1;
}


static int writeOne( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int x = methodArgInt( L, 1 );
    int y = methodArgInt( L, 2 );
    bool on = methodArgBool( L, 3, true );
    bool show = methodArgBool( L, 4, true );
    obj->writeOne( x, y, on, show );
    return 0;
}


static int clearOne( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int x = methodArgInt( L, 1 );
    int y = methodArgInt( L, 2 );
    bool show = methodArgBool( L, 3, true );
    obj->clearOne( x, y, show );
    return 0;
}


static int writeHLine( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int y = methodArgInt( L, 1 );
    int start_x = methodArgInt( L, 2 );
    int end_x = methodArgInt( L, 3 );
    bool on = methodArgBool( L, 4, true );
    bool show = methodArgBool( L, 5, true );
    obj->writeHLine( y, start_x, end_x, on, show );
    return 0;
}


static int clearHLine( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int y = methodArgInt( L, 1 );
    int start_x = methodArgInt( L, 2 );
    int end_x = methodArgInt( L, 3 );
    bool show = methodArgBool( L, 4, true );
    obj->clearHLine( y, start_x, end_x, show );
    return 0;
}


static int writeVLine( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int x = methodArgInt( L, 1 );
    int start_y = methodArgInt( L, 2 );
    int end_y = methodArgInt( L, 3 );
    bool on = methodArgBool( L, 4, true );
    bool show = methodArgBool( L, 5, true );
    obj->writeVLine( x, start_y, end_y, on, show );
    return 0;
}


static int clearVLine( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int y = methodArgInt( L, 1 );
    int start_x = methodArgInt( L, 2 );
    int end_x = methodArgInt( L, 3 );
    bool show = methodArgBool( L, 4, true );
    obj->clearVLine( y, start_x, end_x, show );
    return 0;
}


static int writeY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int y = methodArgInt( L, 1 );
    bool on = methodArgBool( L, 2, true );
    bool show = methodArgBool( L, 3, true );
    obj->writeY( y, on, show );
    return 0;
}


static int writeX( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int x = methodArgInt( L, 1 );
    bool on = methodArgBool( L, 2, true );
    bool show = methodArgBool( L, 3, true );
    obj->writeX( x, on, show );
    return 0;
}


static int clearY( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int y = methodArgInt( L, 1 );
    bool show = methodArgBool( L, 2, true );
    obj->clearY( y, show );
    return 0;
}


static int clearX( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int x = methodArgInt( L, 1 );
    bool show = methodArgBool( L, 2, true );
    obj->clearX( x, show );
    return 0;
}


static int writeRectangle( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int start_x = methodArgInt( L, 1 );
    int end_x = methodArgInt( L, 2 );
    int start_y = methodArgInt( L, 3 );
    int end_y = methodArgInt( L, 4 );
    bool on = methodArgBool( L, 5, true );
    bool show = methodArgBool( L, 6, true );
    obj->writeRectangle( start_x, end_x, start_y, end_y, on, show );
    return 0;
}


static int clearRectangle( lua_State *L )
{
    EVNMatrixLED *obj = (EVNMatrixLED*)luaL_checkudata( L, 1, "EVNMatrixLED" );
    int start_x = methodArgInt( L, 1 );
    int end_x = methodArgInt( L, 2 );
    int start_y = methodArgInt( L, 3 );
    int end_y = methodArgInt( L, 4 );
    bool show = methodArgBool( L, 5, true );
    obj->clearRectangle( start_x, end_x, start_y, end_y, show );
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
    { "setInvertX", setInvertX },
    { "setInvertY", setInvertY },
    { "setSwapXY", setSwapXY },
    { "getInvertX", getInvertX },
    { "getInvertY", getInvertY },
    { "getSwapXY", getSwapXY },
    { "writeOne", writeOne },
    { "clearOne", clearOne },
    { "writeHLine", writeHLine },
    { "clearHLine", clearHLine },
    { "writeVLine", writeVLine },
    { "clearVLine", clearVLine },
    { "writeY", writeY },
    { "writeX", writeX },
    { "clearY", clearY },
    { "clearX", clearX },
    { "writeRectangle", writeRectangle },
    { "clearRectangle", clearRectangle },

    { NULL, NULL }
};


// Class methods
static const luaL_Reg funcs[] = {
    { "new", new_object },

    { NULL, NULL }
};


void init_evn_matrixled( lua_State *L )
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

    EVNMatrixLED *p = new(rp) EVNMatrixLED( port );

    // Add metatable
    luaL_getmetatable( L, EVN_CLASS_NAME );
    lua_setmetatable( L, -2 );

    return 1;
}
