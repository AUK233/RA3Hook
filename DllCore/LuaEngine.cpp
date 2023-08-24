#include "pch.h"
#include <string>
#include <codecvt>
#pragma warning(disable:4996);

#include "utiliy.h"
#include "LuaEngine.h"

struct CFACampaignINI {
	std::wstring wstr;
	UINT init = 0;
} CampaignINIPath;
std::wstring CampaignINIDefault;

namespace RA3::LuaEngine {

	uintptr_t _F_luaScriptEngine = 0;
	uintptr_t _F_loadConfigINIPath = 0;
	//
	uintptr_t _F_pushLuaExecuteFunction = 0;
	uintptr_t _F_pushLuaGetFunction = 0;
	uintptr_t _F_readLuaScript = 0;
	uintptr_t _Ret_readLuaScript = 0;
	uintptr_t _p_ExecuteAction = 0;
	uintptr_t _F_readObjectScript = 0;
	uintptr_t _Ret_readObjectScript = 0;

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

	uintptr_t _p_Lua_pushnil = 0;
	void __cdecl Lua_pushnil(void* pLua) {
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
	void __cdecl Lua_pushstring(void* pLua, const char* text) {
		__asm {
			jmp _p_Lua_pushstring
		}
	}

	void __fastcall InitializeLuaEngineSteam(uintptr_t hmodEXE) {
		_p_Lua_tostring = hmodEXE + 0xA790;
		_p_Lua_pushnil = hmodEXE + 0xA8B0;
		_p_Lua_pushnumber = hmodEXE + 0xA910;
		_p_Lua_pushstring = hmodEXE + 0xA980;
		_F_luaScriptEngine = hmodEXE + 0x8DD17C;
		_F_loadConfigINIPath = hmodEXE + 0x2CBC1B;

		_F_pushLuaExecuteFunction = hmodEXE + 0x6F70;
		_F_pushLuaGetFunction = hmodEXE + 0xACA0;
		_F_readLuaScript = hmodEXE + 0x1B0BC9;
		_Ret_readLuaScript = hmodEXE + 0x1B0BC9 + 5;

		_p_ExecuteAction = hmodEXE + 0x182C50;
		_F_readObjectScript = hmodEXE + 0x182BEE;
		_Ret_readObjectScript = hmodEXE + 0x182BEE + 5;
	}

	void __fastcall InitializeLuaEngineOrigin(uintptr_t hmodEXE)
	{
		_p_Lua_tostring = hmodEXE + 0;
		_p_Lua_pushnil = hmodEXE + 0;
		_p_Lua_pushnumber = hmodEXE + 0;
		_p_Lua_pushstring = hmodEXE + 0;
		_F_luaScriptEngine = hmodEXE + 0x8E231C;
		_F_loadConfigINIPath = hmodEXE + 0;

		_F_pushLuaExecuteFunction = hmodEXE + 0;
		_F_pushLuaGetFunction = hmodEXE + 0;
		_F_readLuaScript = hmodEXE + 0;
		_Ret_readLuaScript = hmodEXE + 0 + 5;

		_p_ExecuteAction = hmodEXE + 0;
		_F_readObjectScript = hmodEXE + 0;
		_Ret_readObjectScript = hmodEXE + 0 + 5;
	}

	void __fastcall HookLuaEngine() {
		hookGameBlock((void*)_F_loadConfigINIPath, (uintptr_t)HookGetProfileDataINI);
		hookGameBlock((void*)_F_readLuaScript, (uintptr_t)HookLuaScriptEngineASM);
		hookGameBlock((void*)_F_readObjectScript, (uintptr_t)HookObjectScriptEngineASM);
	}


	void __fastcall GetCFACampaignFlagINIPath(LPCWSTR in) {
		/*
		std::wstring fileName = in;
		size_t lastindex = fileName.find_last_of(L"\\");
		if (lastindex != std::wstring::npos)
		{
			std::wstring extension = fileName.substr(lastindex + 1, 3);
			if (extension == L"Pro") {
				fileName.erase(lastindex + 1, std::wstring::npos);
				fileName += L"CFACampaignFlag.ini";

				//MessageBoxW(NULL, fileName.c_str(), L"test", MB_OK);
				CampaignINIPath.wstr = fileName;
				CampaignINIPath.init = 1;
				return;
			}
		}*/

		if (!CampaignINIDefault.empty()) {
			//CampaignINIPath.wstr = CampaignINIDefault;
			CampaignINIPath.wstr = L"Z:\\TEMP\\test.ini";
			CampaignINIPath.init = 1;
		}
		else {
			CampaignINIPath.init = 0;
		}
		return;
	}

	__declspec(naked) void __fastcall HookGetProfileDataINI() {
		__asm {
			sub esp, 0xCBC
			mov ecx, esp
			call GetCFACampaignFlagINIPath
			add esp, 0xCBC
			pop edi
			pop esi
			pop ebp
			pop ebx
			add esp, 0x34
			ret
		}
	}

	char luaStr_SetCFACampaignFlag[] = "SetCFACampaignFlag";
	int SetCFACampaignFlag(void* pLua) {
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
	int CheckCFACampaignFlag(void* pLua) {
		int out = 0;
		if (CampaignINIPath.init) {
			std::wstring arg1 = Lua_toWString(pLua, 1);
			out = GetPrivateProfileIntW(L"Campaign", arg1.c_str(), 0, CampaignINIPath.wstr.c_str());
		}
		return out;
	}

	__declspec(naked) uintptr_t __fastcall GetCurrentGameObjectASM()
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
			mov eax, [eax + 0x138]
		ofsReturn:
			ret
		}
	}

	char luaStr_GetCurrentID[] = "GetCurrentID";
	int GetCurrentGameObjectId(void* pLua)
	{
		uintptr_t getID = GetCurrentGameObjectASM();
		if (getID) {
			Lua_pushnumber(pLua, getID);
			return 1;
		}

		Lua_pushnil(pLua);
		Lua_pushstring(pLua, "Invalid address");
		return 2;
	}

	__declspec(naked) void __fastcall HookLuaScriptEngineASM() {
		__asm {
			call _F_pushLuaGetFunction
			movd xmm3, esp // save stack pointer
			// new block
			// SetCFACampaignFlag
			mov ecx, [esi + 0x24]
			lea eax, SetCFACampaignFlag
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x24]
			lea eax, luaStr_SetCFACampaignFlag
			push eax
			push edx
			call _F_pushLuaGetFunction
			// CheckCFACampaignFlag
			mov ecx, [esi + 0x24]
			lea eax, CheckCFACampaignFlag
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x24]
			lea eax, luaStr_CheckCFACampaignFlag
			push eax
			push edx
			call _F_pushLuaGetFunction
			// return original
			movd esp, xmm3
			jmp _Ret_readLuaScript
		}
	}

	char luaStr_ExecuteActionInObject[] = "ExecuteActionInObject";
	__declspec(naked) void __fastcall HookObjectScriptEngineASM() {
		__asm {
			call _F_pushLuaGetFunction
			movd xmm3, esp // save stack pointer
			// new block
			// SetCFACampaignFlag
			mov ecx, [esi + 0x28]
			lea eax, SetCFACampaignFlag
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x28]
			lea eax, luaStr_SetCFACampaignFlag
			push eax
			push edx
			call _F_pushLuaGetFunction
			// CheckCFACampaignFlag
			mov ecx, [esi + 0x28]
			lea eax, CheckCFACampaignFlag
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x28]
			lea eax, luaStr_CheckCFACampaignFlag
			push eax
			push edx
			call _F_pushLuaGetFunction
			// GetCurrentID
			/*
			mov ecx, [esi + 0x28]
			lea eax, GetCurrentGameObjectId
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x28]
			lea eax, luaStr_GetCurrentID
			push eax
			push edx
			call _F_pushLuaGetFunction
			*/
			// ExecuteActionInObject
			mov ecx, [esi + 0x28]
			mov eax, _p_ExecuteAction
			push 0
			push eax
			push ecx
			call _F_pushLuaExecuteFunction
			mov edx, [esi + 0x28]
			lea eax, luaStr_ExecuteActionInObject
			push eax
			push edx
			call _F_pushLuaGetFunction
			// return original
			movd esp, xmm3
			jmp _Ret_readObjectScript
		}
	}

}
