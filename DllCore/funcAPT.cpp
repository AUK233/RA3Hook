#include "pch.h"
#include <format>

#include "utiliy.h"
#include "utiliy_game.h"
#include "commonData.hpp"
#include "commonStruct.hpp"

#include "funcAPT.h"

extern std::wstring SettingINI;
extern uintptr_t _F_SkirmishSettingsPointerCDBBE0;
extern uintptr_t _F_GameTextPointerCDB754;
extern uintptr_t _F_GameObjectHashCE6C58;
extern uintptr_t _F_CallGetEAString4CB750;
extern uintptr_t _F_CallFreeEAString4CB6D0;
extern uintptr_t _F_CallGetPointer7E4230;
extern uintptr_t _F_Call4D8420;
extern uintptr_t _F_Call416830;

namespace RA3::APT {

uintptr_t _ret_ToggleRandomCrateOptions = 0xB26271;

__declspec(naked) void __fastcall PushTheRuleTextToOnlineChatBox(ruleDataPointer* pRule, LPCSTR pText)
{
	__asm {
#include "ASM/PushTheRuleTextToOnlineChatBox.asm"
	}
}

void __fastcall UpdateToggleRandomCrateOptionsCPP(ruleDataPointer* prule)
{
	char* rule74 = prule->ruleData->RandomCrate;
	if (SettingINI.empty()) {
		*(int*)rule74 = 1;
		return;
	}

	*(int*)rule74 = 0;
	uint8_t RandomCrate = GetPrivateProfileIntW(L"SkirmishSetting", L"RandomCrate", 1, SettingINI.c_str());
	rule74[0] = RandomCrate;
	if (RandomCrate) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:SANDBOXMODE");
	}

	uint8_t PowerfulMode = GetPrivateProfileIntW(L"SkirmishSetting", L"PowerfulMode", 0, SettingINI.c_str());
	rule74[1] = PowerfulMode;
	if (PowerfulMode) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:SANDBOXMODE");
	}

	uint8_t EnhancedMap = GetPrivateProfileIntW(L"SkirmishSetting", L"EnhancedMap", 0, SettingINI.c_str());
	rule74[2] = EnhancedMap;
	if (EnhancedMap) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:SANDBOXMODE");
	}
}

__declspec(naked) void __fastcall UpdateToggleRandomCrateOptionsASM()
{
	__asm {
		mov eax, [ebx + 0xC]
		mov ecx, [eax + 0x74]
		test ecx, ecx
		je isZeroBlock
		// ToggleRandomCrates turn off
		push 6
		mov dword ptr[eax + 0x74], 0
		jmp _ret_ToggleRandomCrateOptions
	isZeroBlock:
		// ToggleRandomCrates turn on
		push 0
		mov edx, _ret_ToggleRandomCrateOptions
		mov ecx, ebx
		push edx
		jmp UpdateToggleRandomCrateOptionsCPP
	}
}

__declspec(naked) void __fastcall UpdateRandomCrateCreateFunctionASM()
{
	__asm {
#include "ASM/UpdateRandomCrateCreateFunction.asm"
	}
}

void __fastcall HookAptFunctionUpdate()
{
	hookGameBlock((void*)0xB261A9, (uintptr_t)UpdateToggleRandomCrateOptionsASM);

	hookGameBlock((void*)0x616DD9, (uintptr_t)UpdateRandomCrateCreateFunctionASM);
	WriteHookToProcess((void*)(0x616DD9 + 5), (void*)&nop1, 1U);
}

}
