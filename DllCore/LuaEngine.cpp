#include "pch.h"
#include <string>
#include <codecvt>
#include <filesystem>
#pragma warning(disable:4996);

#include "utiliy.h"
#include "commonStruct.hpp"
#include "LuaEngine.h"

extern inputSettingINFO inputSetting;
extern uintptr_t _F_CallKillGameObject;
extern uintptr_t _F_RuleSettingsCE3A74;

struct CFACampaignINI {
	std::wstring wstr;
	UINT init = 0;
} CampaignINIPath;
std::wstring CampaignINIDefault;

namespace RA3::LuaEngine {

	uintptr_t _F_luaScriptEngine = 0;
	uintptr_t _F_loadConfigINIPath = 0;
	uintptr_t _Ret_loadConfigINIPath = 0;
	//
	uintptr_t _F_readLuaScript = 0;
	uintptr_t _Ret_readLuaScript = 0;
	uintptr_t _p_ExecuteAction = 0;
	uintptr_t _F_readObjectScript = 0;
	uintptr_t _Ret_readObjectScript = 0;

	uintptr_t _F_pushLuaExecuteFunction = 0;
	__declspec(naked) void __cdecl pushLuaExecuteFunction(void* pLua, uintptr_t pFunction, int n) {
		__asm {
			jmp _F_pushLuaExecuteFunction
		}
	}

	uintptr_t _F_pushLuaGetFunction = 0;
	__declspec(naked) void __cdecl pushLuaGetFunction(void* pLua, const char* name) {
		__asm {
			jmp _F_pushLuaGetFunction
		}
	}

	uintptr_t _p_Lua_tostring = 0;
	__declspec(naked) const char* __cdecl Lua_tostring(void* pLua, int index) {
		__asm {
			jmp _p_Lua_tostring
		}
	}

	std::wstring __fastcall Lua_toWString(void* pLua, int index) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(Lua_tostring(pLua, index));
	}

	uintptr_t _p_Lua_ToNumber = 0x40A700;
	__declspec(naked) double __cdecl Lua_ToNumber(void* pLua, int index)
	{
		__asm {
			jmp _p_Lua_ToNumber
		}
	}

	uintptr_t _p_Lua_pushnil = 0;
	__declspec(naked) void __cdecl Lua_pushnil(void* pLua) {
		__asm {
			jmp _p_Lua_pushnil
		}
	}

	uintptr_t _p_Lua_pushnumber = 0;
	__declspec(naked) void __cdecl Lua_pushnumber(void* pLua, double value) {
		__asm {
			jmp _p_Lua_pushnumber
		}
	}

	uintptr_t _p_Lua_pushstring = 0;
	__declspec(naked) void __cdecl Lua_pushstring(void* pLua, const char* text) {
		__asm {
			jmp _p_Lua_pushstring
		}
	}


	uintptr_t _p_Lua_pushvalue = 0;
	__declspec(naked) void __cdecl Lua_pushvalue(void* pLua, int value) {
		__asm {
			jmp _p_Lua_pushvalue
		}
	}

	__declspec(naked) int __fastcall Lua_GetParameterCount(void* pLua)
	{
		// in 0040A3D0
		__asm {
			mov eax, [ecx]
			sub eax, [ecx + 0x10]
			sar eax, 4
			ret
		}
	}

	__declspec(naked) uint32_t __fastcall Lua_GetTableNewMemberOrNull(void* pLua, int index)
	{
		__asm {
			mov eax, edx
			test eax, eax
			jl ofsA5ED
			mov edx, [ecx + 0x10]
			shl eax, 4
			lea eax, [edx + eax - 0x10]
			cmp eax, [ecx]
			jb ofsA5F2
		ofsA5EA:
			xor eax, eax
			ret
		ofsA5ED:
			shl eax, 4
			add eax, [ecx]
		ofsA5F2:
			test eax, eax
			je ofsA5EA
			cmp dword ptr[eax], 4
			jne ofsA5EA
			mov edx, [eax + 0x8]
			mov eax, [edx + 0xC]
			ret
		}
	}

	uintptr_t _Ret_004DEE52 = 0x4DEE52;
	__declspec(naked) GameObject* __fastcall GetGameObjectPointerFromObjectID(uint32_t id)
	{
		__asm {
			sub esp, 0xC
			test ecx, ecx
			je return0
			push esi
			push edi
			mov eax, ecx
			jmp _Ret_004DEE52
		return0 :
			xor eax, eax
			add esp, 0xC
			ret
		}
	}

	GameObject* __fastcall GetGameObjectPointer(void* pLua, int index)
	{
		// 005781B4
		int argc = Lua_GetParameterCount(pLua);
		if (argc < index) {
			return nullptr;
		}

		uint32_t id = Lua_GetTableNewMemberOrNull(pLua, index);
		if (!id) {
			return nullptr;
		}

		GameObject* pGO = GetGameObjectPointerFromObjectID(id);
		return pGO;
	}

	void __fastcall InitializeLuaEngineSteam(uintptr_t hmodEXE) {
		_p_Lua_pushvalue = hmodEXE + 0xA4D0;
		_p_Lua_tostring = hmodEXE + 0xA790;
		_p_Lua_pushnil = hmodEXE + 0xA8B0;
		_p_Lua_pushnumber = hmodEXE + 0xA910;
		_p_Lua_pushstring = hmodEXE + 0xA980;

		_F_luaScriptEngine = hmodEXE + 0x8DD17C;
		_F_loadConfigINIPath = hmodEXE + 0x2CBB44;
		_Ret_loadConfigINIPath = hmodEXE + 0x2CBB44 + 6;

		_F_pushLuaExecuteFunction = hmodEXE + 0x6F70;
		_F_pushLuaGetFunction = hmodEXE + 0xACA0;
		_F_readLuaScript = hmodEXE + 0x1B0BC9;
		_Ret_readLuaScript = hmodEXE + 0x1B0BC9 + 5;

		_p_ExecuteAction = hmodEXE + 0x182C50;
		_F_readObjectScript = hmodEXE + 0x182BEE;
		_Ret_readObjectScript = hmodEXE + 0x182BEE + 5;
	}

	void __fastcall InitializeLuaEngineOrigin(uintptr_t hmodEXE) {
		_p_Lua_pushvalue = hmodEXE + 0xA4F0;
		_p_Lua_tostring = hmodEXE + 0xA7B0;
		_p_Lua_pushnil = hmodEXE + 0xA8D0;
		_p_Lua_pushnumber = hmodEXE + 0xA930;
		_p_Lua_pushstring = hmodEXE + 0xA9A0;

		_F_luaScriptEngine = hmodEXE + 0x8E231C;
		_F_loadConfigINIPath = hmodEXE + 0x30A084;
		_Ret_loadConfigINIPath = hmodEXE + 0x30A084 + 6;

		_F_pushLuaExecuteFunction = hmodEXE + 0x6F90;
		_F_pushLuaGetFunction = hmodEXE + 0xACC0;
		_F_readLuaScript = hmodEXE + 0x1F22D9;
		_Ret_readLuaScript = hmodEXE + 0x1F22D9 + 5;

		_p_ExecuteAction = hmodEXE + 0x1C4370;
		_F_readObjectScript = hmodEXE + 0x1C430E;
		_Ret_readObjectScript = hmodEXE + 0x1C430E + 5;

		//
		_p_Lua_ToNumber = hmodEXE + 0xA720;
		_Ret_004DEE52 = hmodEXE + 0x120852;
	}

	void __fastcall HookLuaEngine() {
		if (inputSetting.LocalFlag) {
			if (!CampaignINIDefault.empty()) {
				CampaignINIPath.wstr = CampaignINIDefault;
				CampaignINIPath.init = 1;
			}
			else {
				if (inputSetting.setDebug) {
					MessageBoxW(NULL, L"Invalid campaign flag save path!", L"warning", MB_OK);
				}
				CampaignINIPath.init = 0;
			}
		}
		else {
			hookGameBlock((void*)_F_loadConfigINIPath, (uintptr_t)HookGetProfileDataINI);
		}

		hookGameBlock((void*)_F_readLuaScript, (uintptr_t)HookLuaScriptEngineASM);
		hookGameBlock((void*)_F_readObjectScript, (uintptr_t)HookObjectScriptEngineASM);
	}


	void __fastcall GetCFACampaignFlagINIPath(LPCWSTR in) {
		/**/
		std::wstring fileName = in;
		size_t lastindex = fileName.find_last_of(L"\\");
		if (lastindex != std::wstring::npos)
		{
			std::wstring extension = fileName.substr(lastindex + 1, 3);
			if (extension == L"Pro") {
				fileName.erase(lastindex + 1, std::wstring::npos);
			}
		}

		std::wstring checkFile = fileName + L"ProfileData.ini";
		if (std::filesystem::exists(checkFile)) {
			fileName += L"CFACampaignFlag.ini";
			//MessageBoxW(NULL, fileName.c_str(), L"test", MB_OK);
			CampaignINIPath.wstr = fileName;
			CampaignINIPath.init = 1;
			return;
		}

		if (!CampaignINIDefault.empty()) {
			//CampaignINIPath.wstr = L"Z:\\TEMP\\test.ini";
			CampaignINIPath.wstr = CampaignINIDefault;
			CampaignINIPath.init = 1;
		}
		else {
			CampaignINIPath.init = 0;
		}
		return;
	}

	__declspec(naked) void __fastcall HookGetProfileDataINI() {
		__asm {
			mov ecx, [esp + 0x40]
			test ecx, ecx
			je ofsReturn
			call GetCFACampaignFlagINIPath
		ofsReturn:
			mov ecx, [ebp + 0x7C]
			lea esi, [ebp + 0x74]
			jmp _Ret_loadConfigINIPath
		}
	}

	char luaStr_SetCFACampaignFlag[] = "SetCFACampaignFlag";
	int __cdecl SetCFACampaignFlag(void* pLua) {
		/*
		std::string arg1 = Lua_tostring(pLua, 1);
		std::string arg2 = Lua_tostring(pLua, 2);
		MessageBoxA(NULL, arg1.c_str(), arg2.c_str(), MB_OK);*/

		if (CampaignINIPath.init) {
			std::wstring arg1 = Lua_toWString(pLua, 1);
			std::wstring arg2 = Lua_toWString(pLua, 2);
			WritePrivateProfileStringW(L"Campaign", arg1.c_str(), arg2.c_str(), CampaignINIPath.wstr.c_str());
			/*
			if (strcmp(Lua_tostring(pLua, 2), "0")) {
				WritePrivateProfileStringW(L"Campaign", arg1.c_str(), L"1", CampaignINIPath.wstr.c_str());
			}
			else {
				WritePrivateProfileStringW(L"Campaign", arg1.c_str(), L"0", CampaignINIPath.wstr.c_str());
			}*/
		}
		return 1;
	}

	char luaStr_CheckCFACampaignFlag[] = "CheckCFACampaignFlag";
	int __cdecl CheckCFACampaignFlag(void* pLua) {
		int out = 0;
		if (CampaignINIPath.init) {
			std::wstring arg1 = Lua_toWString(pLua, 1);
			out = GetPrivateProfileIntW(L"Campaign", arg1.c_str(), 0, CampaignINIPath.wstr.c_str());
		}
		return out;
	}

	int __cdecl CheckSkirmishRule(void* pLua)
	{
		ruleDataStruct* pRule = *(ruleDataStruct**)_F_RuleSettingsCE3A74;
		int type = Lua_ToNumber(pLua, 1);
		char* rule74 = pRule->RandomCrate;

		int value = 0;
		switch (type)
		{
		case 1: {
			// is EnhancedMap
			BYTE checkBit = rule74[1];
			if (checkBit & 1) {
				value = 1;
			}
			break;
		}
		case 2:{
			// is NoSuperWeapon
			BYTE checkBit = rule74[1];
			if (checkBit & 0b10){
				value = 1;
			}
			break;
		}
		case 3: {
			// is CrazyMode
			BYTE checkBit = rule74[1];
			if (checkBit & 0b100) {
				value = 1;
			}
			break;
		}
		default:
			break;
		}
		return value;
	}

	int __cdecl GetThisPlayer(void* pLua)
	{
		GameObject* pGO = GetGameObjectPointer(pLua, 1);
		if (pGO) {
			Player* player = pGO->relevantPlayer;
			//MessageBoxA(NULL, player->inScriptName, "nore", MB_OK);
			Lua_pushstring(pLua, player->inScriptName);
		}
		else {
			Lua_pushstring(pLua, "Player_0");
		}
		
		return 1;
	}


	char luaStr_CurrentObjectSuicided[] = "CurDrawableCurrentObjectSuicided";
	__declspec(naked) int __cdecl CurDrawableCurrentObjectSuicided(void* pLua)
	{
		__asm {
			mov ecx, _F_luaScriptEngine
			mov eax, [ecx]
			xor ecx, ecx
			cmp eax, ecx
			je ofsReturn
			mov eax, [eax + 0x128]
			cmp eax, ecx
			je ofsReturn
			mov eax, [eax + 0xC]
			cmp eax, ecx
			je ofsReturn
			push 0
			push 8 // SUICIDED
			push 6
			mov ecx, eax
			call _F_CallKillGameObject
		ofsReturn:
			mov eax, 1
			ret
		}
	}

	void __fastcall HookLuaScriptEngineCPP(void* pLua)
	{
		pushLuaExecuteFunction(pLua, (uintptr_t)SetCFACampaignFlag, 0);
		pushLuaGetFunction(pLua, "SetCFACampaignFlag");
		pushLuaExecuteFunction(pLua, (uintptr_t)CheckCFACampaignFlag, 0);
		pushLuaGetFunction(pLua, "CheckCFACampaignFlag");
		pushLuaExecuteFunction(pLua, (uintptr_t)CheckSkirmishRule, 0);
		pushLuaGetFunction(pLua, "CheckSkirmishRule");

		pushLuaExecuteFunction(pLua, (uintptr_t)GetThisPlayer, 0);
		pushLuaGetFunction(pLua, "GetThisPlayer");
	}

	__declspec(naked) void __fastcall HookLuaScriptEngineASM() {
		__asm {
			call _F_pushLuaGetFunction
			// new block
			mov ecx, [esi + 0x24]
			call HookLuaScriptEngineCPP
			// return original
			jmp _Ret_readLuaScript
		}
	}

	void __fastcall HookObjectScriptEngineCPP(void* pLua)
	{
		pushLuaExecuteFunction(pLua, (uintptr_t)SetCFACampaignFlag, 0);
		pushLuaGetFunction(pLua, "SetCFACampaignFlag");
		pushLuaExecuteFunction(pLua, (uintptr_t)CheckCFACampaignFlag, 0);
		pushLuaGetFunction(pLua, "CheckCFACampaignFlag");
		pushLuaExecuteFunction(pLua, (uintptr_t)CheckSkirmishRule, 0);
		pushLuaGetFunction(pLua, "CheckSkirmishRule");

		pushLuaExecuteFunction(pLua, _p_ExecuteAction, 0);
		pushLuaGetFunction(pLua, "ExecuteActionInObject");

	}

	__declspec(naked) void __fastcall HookObjectScriptEngineASM() {
		__asm {
			call _F_pushLuaGetFunction
			// new block
			mov ecx, [esi + 0x28]
			call HookObjectScriptEngineCPP
			// return original
			jmp _Ret_readObjectScript
		}
	}

}
