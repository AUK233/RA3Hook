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
		PushTheRuleTextToOnlineChatBox(prule, "GUI:RuleEnableRandomCrate");
	}

	uint8_t PowerfulMode = GetPrivateProfileIntW(L"SkirmishSetting", L"PowerfulMode", 0, SettingINI.c_str());
	rule74[1] = PowerfulMode;
	if (PowerfulMode) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:RuleEnablePowerfulMode");
	}

	uint8_t EnhancedMap = GetPrivateProfileIntW(L"SkirmishSetting", L"EnhancedMap", 0, SettingINI.c_str());
	rule74[2] = EnhancedMap;
	if (EnhancedMap) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:RuleEnableEnhancedMap");
	}

	uint8_t NoSuperWeapon = GetPrivateProfileIntW(L"SkirmishSetting", L"NoSuperWeapon", 0, SettingINI.c_str());
	rule74[3] = NoSuperWeapon;
	if (NoSuperWeapon) {
		PushTheRuleTextToOnlineChatBox(prule, "GUI:RuleEnableNoSuperWeapon");
	}
}

CHAR strGUIRuleDisableExtraSettings[] = "GUI:RuleDisableExtraSettings";
__declspec(naked) void __fastcall UpdateToggleRandomCrateOptionsASM()
{
	__asm {
		mov eax, [ebx + 0xC]
		mov ecx, [eax + 0x74]
		test ecx, ecx
		je isZeroBlock
		// ToggleRandomCrates turn off
		mov dword ptr[eax + 0x74], 0
		lea edx, strGUIRuleDisableExtraSettings
		mov ecx, ebx
		call PushTheRuleTextToOnlineChatBox
		jmp ofs72629B
	isZeroBlock:
		// ToggleRandomCrates turn on
		mov ecx, ebx
		call UpdateToggleRandomCrateOptionsCPP
	ofs72629B:
		pop edi
		pop esi
		pop ebx
		add esp, 0x3C
		ret 8
	}
}

__declspec(naked) void __fastcall UpdateRandomCrateCreateFunctionASM()
{
	__asm {
#include "ASM/UpdateRandomCrateCreateFunction.asm"
	}
}

uintptr_t _F_ToggleRandomCrateOptions = 0xB261A9;
uintptr_t _F_RandomCrateCreateFunction = 0x616DD9;

void __fastcall InitializeHookAptFunctionUpdateOrigin(uintptr_t hmodEXE)
{
	_F_ToggleRandomCrateOptions = hmodEXE + 0x6BB1D9;
	_F_RandomCrateCreateFunction = hmodEXE + 0x255CE9;
}

void __fastcall HookAptFunctionUpdate()
{
	hookGameBlock((void*)_F_ToggleRandomCrateOptions, (uintptr_t)UpdateToggleRandomCrateOptionsASM);

	hookGameBlock((void*)_F_RandomCrateCreateFunction, (uintptr_t)UpdateRandomCrateCreateFunctionASM);
	WriteHookToProcess((void*)(_F_RandomCrateCreateFunction + 5), (void*)&nop1, 1U);
}

}
