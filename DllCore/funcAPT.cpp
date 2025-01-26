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

__declspec(naked) void __fastcall UpdateCheckAPTHashA31A11ASM()
{
	__asm {
		// new
		cmp eax, 0x10000
		jbe ofs631A2A
		// new end
		cmp [eax + 0xC], ecx
		je ofs631A1E
		align 16
	ofs631A16:
		mov eax, [eax + 0xC]
		cmp [eax + 0xC], ecx
		jne ofs631A16
	ofs631A1E:
		mov edx, [ecx + 0xC]
		mov [eax + 0xC], edx
		mov eax, [ecx]
		mov edx, [eax + 4]
		pop esi
		jmp edx
	ofs631A2A:
		pop esi
		ret
	}
}

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

uintptr_t _Ret_GetRadarColorA8C49A = 0xA8C49A;
uintptr_t _Ret_SetRadarColorA8C507 = 0xA8C507;
__declspec(naked) void __fastcall UpdateGameObjectRadarColorASM()
{
	__asm {
		mov eax, [esi+0x364] // TiberiumCrystalBehavior pointer
		test eax, eax
		je ofs68C49A
		mov ecx, [eax+4]
		mov edi, [ecx+0x1C] // RadarColor
		jmp _Ret_SetRadarColorA8C507
	ofs68C49A:
		mov ecx, [ebx + 0xDC]
		jmp _Ret_GetRadarColorA8C49A
	}
}

uintptr_t _F_CheckAPTHashA31A11 = 0xA31A11;
uintptr_t _F_ToggleRandomCrateOptions = 0xB261A9;
uintptr_t _F_RandomCrateCreateFunction = 0x616DD9;
uintptr_t _F_GameObjectRadarColor = 0xA8C494;
uintptr_t _F_ShowTiberiumMoney = 0x7557C5;

void __fastcall InitializeHookAptFunctionUpdateOrigin(uintptr_t hmodEXE)
{
	_F_CheckAPTHashA31A11 = hmodEXE + 0x5CF691;

	_F_ToggleRandomCrateOptions = hmodEXE + 0x6BB1D9;
	_F_RandomCrateCreateFunction = hmodEXE + 0x255CE9;

	_F_GameObjectRadarColor = hmodEXE + 0x621004;
	_Ret_GetRadarColorA8C49A = hmodEXE + 0x62100A;
	_Ret_SetRadarColorA8C507 = hmodEXE + 0x621077;
	_F_ShowTiberiumMoney = hmodEXE + 0x393E05;
}

void __fastcall HookAptFunctionUpdate()
{
	//hookGameBlock((void*)_F_CheckAPTHashA31A11, (uintptr_t)UpdateCheckAPTHashA31A11ASM);

	hookGameBlock((void*)_F_ToggleRandomCrateOptions, (uintptr_t)UpdateToggleRandomCrateOptionsASM);

	hookGameBlock((void*)_F_RandomCrateCreateFunction, (uintptr_t)UpdateRandomCrateCreateFunctionASM);
	WriteHookToProcess((void*)(_F_RandomCrateCreateFunction + 5), (void*)&nop1, 1U);

	hookGameBlock((void*)_F_GameObjectRadarColor, (uintptr_t)UpdateGameObjectRadarColorASM);
	WriteHookToProcess((void*)(_F_GameObjectRadarColor + 5), (void*)&nop1, 1U);

	unsigned char noShowTiberiumMoney[] = {
		0x8B, 0x7C, 0x24, 0x20, // mov edi, [esp+0x20]
		0xEB, 0x10,             // jmp
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)_F_ShowTiberiumMoney, &noShowTiberiumMoney, 22U);
}

}
