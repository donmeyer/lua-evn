// luatools.cpp
//
//

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

#include <lua.hpp>

#include "lua_tools.h"



/**
 * If the table on the top of the Lua stack has the given field
 * and check to see if it is the right type.
 * If the field is missing or the wrong type, throw a Lua error.
 */
void requireField( lua_State *L, const char *key, int type )
{
    if( hasField( L, key, type ) == false )
    {
        luaL_error( L, "Table field '%s' missing", key );
    }
}


/**
 * If the table on the top of the Lua stack has the given field
 * and check to see if it is the right type.
 * If so, return true.
 * If no field return false.
 * If has a field but it is the wrong type, throw an error.
 */
bool hasField( lua_State *L, const char *key, int type )
{
    lua_pushstring( L, key );
    lua_gettable( L, -2 );
    if( lua_isnil( L, -1 ) )
    {
        // That table member does not exist. Not necessarily an error.
        lua_pop( L, 1 );
        return false;
    }

    if( type != lua_type( L, -1 ) )
    {
        // Has the field, but wrong type
        luaL_error( L, "Wrong type for table field '%s'", key );
        return false;
    }

    return true;
}


void addIntegerConstant( lua_State *L, const char *name, lua_Integer value )
{
    lua_pushstring( L, name );
    lua_pushnumber( L, value );
    lua_settable( L, -3 );
}


#if 0
void dumpstack( lua_State *L, const char *message )
{
  int i;
  int top=lua_gettop(L);
  printf("dumpstack -- %s\n",message);
  for (i=1; i<=top; i++) {
    printf("%d\t%s\t",i,luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
  printf("dumpstack -- END\n");
}
#endif


//==================================================================================
// Function argument parsing
//==================================================================================

// functionArgBool( L, arg-num, default-value )     // Optional
// functionArgBool( L, arg-num )                    // Required

bool functionArgBool( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    luaL_checktype( L, arg, LUA_TBOOLEAN );
    return( lua_toboolean( L, arg ) );
}


bool functionArgBool( lua_State *L, int arg, bool def )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return def;
    }

    luaL_checktype( L, arg, LUA_TBOOLEAN );
    return( lua_toboolean( L, arg ) );
}


int functionArgInt( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checkinteger( L, arg ) );
}


int functionArgInt( lua_State *L, int arg, int def )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checkinteger( L, arg ) );
}


float functionArgFloat( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checknumber( L, arg ) );
}


float functionArgFloat( lua_State *L, int arg, float def )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checknumber( L, arg ) );
}


lua_Number functionArgNumber( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checknumber( L, arg ) );
}


lua_Number functionArgNumber( lua_State *L, int arg, lua_Number def )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checknumber( L, arg ) );
}


const char *functionArgString( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        luaL_error( L, "Invalid number of arguments" );
    }

    const char *s = lua_tostring( L, arg );
    if( s == NULL )
    {
        luaL_error( L, "Invalid argument, must be a string or a number" );
    }

    return s;
}


const char *functionArgString( lua_State *L, int arg, const char *def )
{
    const int nargs = lua_gettop( L );
    if( nargs < arg )
    {
        return def;
    }

    const char *s = lua_tostring( L, arg );
    if( s == NULL )
    {
        luaL_error( L, "Invalid argument, must be a string or a number" );
    }

    return s;
}



//==================================================================================
// Class method argument parsing
//==================================================================================

// Method argument numbers take into account the 'self' item at the bottom of
// the Lua stack. i.e. argument 1 is actually at stack index 2.
//
// methodArgBool( L, arg-num, default-value )     // Optional
// methodArgBool( L, arg-num )                    // Required

bool methodArgBool( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    luaL_checktype( L, arg + 1, LUA_TBOOLEAN );
    return( lua_toboolean( L, arg + 1 ) );
}


bool methodArgBool( lua_State *L, int arg, bool def )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return def;
    }

    luaL_checktype( L, arg + 1, LUA_TBOOLEAN );
    return( lua_toboolean( L, arg + 1 ) );
}


int methodArgInt( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checkinteger( L, arg + 1 ) );
}


int methodArgInt( lua_State *L, int arg, int def )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checkinteger( L, arg + 1 ) );
}


float methodArgFloat( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checknumber( L, arg + 1 ) );
}


float methodArgFloat( lua_State *L, int arg, float def )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checknumber( L, arg + 1 ) );
}


lua_Number methodArgNumber( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return luaL_error( L, "Invalid number of arguments" );
    }

    return( luaL_checknumber( L, arg + 1 ) );
}


lua_Number methodArgNumber( lua_State *L, int arg, lua_Number def )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return def;
    }

    return( luaL_checknumber( L, arg + 1 ) );
}


const char *methodArgString( lua_State *L, int arg )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        luaL_error( L, "Invalid number of arguments" );
    }

    const char *s = lua_tostring( L, arg + 1 );
    if( s == NULL )
    {
        luaL_error( L, "Invalid argument, must be a string or a number" );
    }

    return s;
}


const char *methodArgString( lua_State *L, int arg, const char *def )
{
    const int nargs = lua_gettop( L ) - 1;
    if( nargs < arg )
    {
        return def;
    }

    const char *s = lua_tostring( L, arg + 1 );
    if( s == NULL )
    {
        luaL_error( L, "Invalid argument, must be a string or a number" );
    }

    return s;
}
