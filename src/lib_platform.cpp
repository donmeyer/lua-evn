// lib_platform.cpp

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

#include "lua.hpp"

#include "lua_tools.h"
#include "lib_platform.h"
#include "lua_support.h"



static int execEnabled( lua_State *L )
{
    lua_pushboolean( L, execLoopEnabled );
    return 1;
}


static int housekeepingEnabled( lua_State *L )
{
    lua_pushboolean( L, housekeepingLoopEnabled );
    return 1;
}


static int setExecEnabled( lua_State *L )
{
    bool en = functionArgBool( L, 1 );
    execLoopEnabled = en;
    return 0;
}


static int setHousekeepingEnabled( lua_State *L )
{
    bool en = functionArgBool( L, 1 );
    housekeepingLoopEnabled = en;
    return 0;
}



//------------------------------------------------------------------

static const luaL_Reg funcs[] = {
    { "execEnabled", execEnabled },
    { "housekeepingEnabled", housekeepingEnabled },

    { "setExecEnabled", setExecEnabled },
    { "setHousekeepingEnabled", setHousekeepingEnabled },

    { NULL, NULL }
};


// This will be called by the Lua process to initialize the library.
int luaopen_platform( lua_State *L )
{
    luaL_newlib( L, funcs );

    return 1;
}
