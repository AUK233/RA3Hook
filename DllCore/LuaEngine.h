#pragma once

#include <string>

namespace RA3::LuaEngine {

void __cdecl pushLuaExecuteFunction(void* pLua, uintptr_t pFunction, int n);
void __cdecl pushLuaGetFunction(void* pLua, const char* name);
//
const char* __cdecl Lua_tostring(void* pLua, int index);
std::wstring __fastcall Lua_toWString(void* pLua, int index);
double __cdecl Lua_ToNumber(void* pLua, int index);
void __cdecl Lua_pushnil(void* pLua);
void __cdecl Lua_pushnumber(void* pLua, double value);
void __cdecl Lua_pushstring(void* pLua, const char* text);
void __cdecl Lua_pushvalue(void* pLua, int value);
// in 0040A3D0, original is __cdecl
int __fastcall Lua_GetParameterCount(void* pLua);
// in 0040A5D0, original is __cdecl
uint32_t __fastcall Lua_GetTableNewMemberOrNull(void* pLua, int index);

// should be 005781E2, but now use 004DEE52 to return the value.
GameObject* __fastcall GetGameObjectPointerFromObjectID(uint32_t id);
//
GameObject* __fastcall GetGameObjectPointer(void* pLua, int index);

//
void __fastcall InitializeLuaEngineSteam(uintptr_t hmodEXE);
void __fastcall InitializeLuaEngineOrigin(uintptr_t hmodEXE);
void __fastcall HookLuaEngine();
void __fastcall GetCFACampaignFlagINIPath(LPCWSTR in);
void __fastcall HookGetProfileDataINI();
// lua scripts
int __cdecl SetCFACampaignFlag(void* pLua);
int __cdecl CheckCFACampaignFlag(void* pLua);
int __cdecl CheckSkirmishRule(void* pLua);
int __cdecl GetThisPlayer(void* pLua);
// object scripts
int __cdecl CurDrawableCurrentObjectSuicided(void* pLua);
//
void __fastcall HookLuaScriptEngineCPP(void* pLua);
void __fastcall HookLuaScriptEngineASM();
//
void __fastcall HookObjectScriptEngineCPP(void* pLua);
void __fastcall HookObjectScriptEngineASM();

}
