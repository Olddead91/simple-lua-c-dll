//
// ������ ������� ���������� �� C++ ��� ������ �� �� LUA
// http://quik2dde.ru/viewtopic.php?id=18
//

#include <windows.h>
#include <process.h>

// � ������ ������ ������� �� Lua-���� �� ������� DLL
// ���������� ���������� ��� ��������� �� ����������� ������������ ������ Lua
#define LUA_LIB
#define LUA_BUILD_AS_DLL

// ���������� ������������ ����� �� ������������ Lua
// ���������� ���� � ������ Lua5.1 ��� Lua5.3 ����� � ���������� ������� (������ � ����������� �� ���������� �������� � Solution Configurations)
extern "C" {
#include "lauxlib.h"
#include "lua.h"
}

// ����������� ����� ����� ��� DLL
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

// ���������� ����� �������, ���������� �� Lua

static int forLua_GetCurrentThreadId(lua_State *L) {
	// ���������� ���� ��������, ���������� �� Win API �������
    lua_pushinteger(L, GetCurrentThreadId());
	return(1);
}

static int forLua_MultTwoNumbers(lua_State *L) {
	// �������� ������ � ������ ��������� ������ ������� �� ����� � ��������� ������� �� �����
    double d1 = luaL_checknumber(L, 1);
    double d2 = luaL_checknumber(L, 2);

	// �������� � ���� ��������� ���������
    lua_pushnumber(L, d1 * d2);

    return(1);  // ��� ������� ���������� ���� ��������
}
	
static int forLua_MultAllNumbers(lua_State *L) {
	const int n = lua_gettop(L);  // ���������� ���������� ����������
	double res = 1;
	bool isNumberFound = false;
	for (int i = 1; i <= n; ++i)
		if (lua_type(L, i) == LUA_TNUMBER)
		{
			res *= lua_tonumber(L, i);
			isNumberFound = true;
		}

    if (isNumberFound)
		lua_pushnumber(L, res);
	else
		lua_pushnil(L);

    return(1);
}
	
// ������ ������������� � dll �������
static struct luaL_Reg ls_lib[] = {
    {"GetCurrentThreadId", forLua_GetCurrentThreadId},
    {"MultTwoNumbers", forLua_MultTwoNumbers},
    {"MultAllNumbers", forLua_MultAllNumbers},
    {NULL, NULL}
};

extern "C" LUALIB_API int luaopen_luacdll(lua_State *L) {
	// ��� ������� ���������� � ������ ������ require() � Lua-����
	// ������������ ������������� � dll �������, ����� ��� ����� �������� ��� Lua
	#if LUA_VERSION_NUM >= 502
		lua_newtable(L);
		luaL_setfuncs(L, ls_lib, 0);
		lua_setglobal(L, "luacdll"); 
	#else
		luaL_openlib(L, "luacdll", ls_lib, 0);
	#endif

	return 1;
}

