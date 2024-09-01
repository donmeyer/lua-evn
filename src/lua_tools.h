// lua_tools.h

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

//
// These are helper functions that are meant to be called from Lua functions implemented in 'C'.
//

#ifndef LUA_TOOLS_H
#define LUA_TOOLS_H  1

#include "lua.hpp"




void requireField( lua_State *L, const char *key, int type );

bool hasField( lua_State *L, const char *key, int type );

void addIntegerConstant( lua_State *L, const char *name, lua_Integer value );

// void dumpstack( lua_State *L, const char *message);


bool functionArgBool( lua_State *L, int arg );
bool functionArgBool( lua_State *L, int arg, bool def );

int functionArgInt( lua_State *L, int arg );
int functionArgInt( lua_State *L, int arg, int def );

float functionArgFloat( lua_State *L, int arg );
float functionArgFloat( lua_State *L, int arg, float def );

lua_Number functionArgNumber( lua_State *L, int arg );
lua_Number functionArgNumber( lua_State *L, int arg, lua_Number def );

const char *functionArgString( lua_State *L, int arg );
const char *functionArgString( lua_State *L, int arg, const char *def );



bool methodArgBool( lua_State *L, int arg );
bool methodArgBool( lua_State *L, int arg, bool def );

int methodArgInt( lua_State *L, int arg );
int methodArgInt( lua_State *L, int arg, int def );

float methodArgFloat( lua_State *L, int arg );
float methodArgFloat( lua_State *L, int arg, float def );

lua_Number methodArgNumber( lua_State *L, int arg );
lua_Number methodArgNumber( lua_State *L, int arg, lua_Number def );

const char *methodArgString( lua_State *L, int arg );
const char *methodArgString( lua_State *L, int arg, const char *def );

#endif
