#pragma once

#include <string>

namespace RA3::LuaEngine {

const char* __cdecl Lua_tostring(void* pLua, int index);
std::wstring __fastcall Lua_toWString(void* pLua, int index);
void __cdecl Lua_pushnil(void* pLua);
void __cdecl Lua_pushnumber(void* pLua, double value);
void __cdecl Lua_pushstring(void* pLua, const char* text);
//
void __fastcall InitializeLuaEngineSteam(uintptr_t hmodEXE);
void __fastcall InitializeLuaEngineOrigin(uintptr_t hmodEXE);
void __fastcall HookLuaEngine();
void __fastcall GetCFACampaignFlagINIPath(LPCWSTR in);
void __fastcall HookGetProfileDataINI();
// lua scripts
int SetCFACampaignFlag(void* pLua);
int CheckCFACampaignFlag(void* pLua);
int CheckRuleEnhancedMap(void* pLua);
// object scripts
uintptr_t __fastcall GetCurrentGameObjectPtrASM();
int CurDrawableCurrentObjectSuicided(void* pLua);
//
void __fastcall HookLuaScriptEngineASM();
void __fastcall HookObjectScriptEngineASM();

}
