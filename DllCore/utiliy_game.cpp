#include "pch.h"

#include "utiliy_game.h"

uintptr_t _F_CallRandomRadius = 0x600B10;
__declspec(naked) float __cdecl getRadomFloatValue(float minValue, float maxValue)
{
	__asm {
		jmp _F_CallRandomRadius
	}
}

uintptr_t _F_ToMemoryAllocation = 0x416720;
__declspec(naked) void* __fastcall newMemoryAllocation(UINT32 size)
{
	__asm {
		push ecx
		call _F_ToMemoryAllocation
		add esp, 4
		ret
	}
}

uintptr_t _F_SkirmishSettingsPointerCDBBE0 = 0xCDBBE0;
uintptr_t _F_GameTextPointerCDB754 = 0xCDB754;
uintptr_t _F_GameObjectHashCE6C58 = 0xCE6C58;
uintptr_t _F_RuleSettingsCE3A74 = 0xCE3A74;

uintptr_t _F_CallGetEAString4CB750 = 0x4CB750;
uintptr_t _F_CallFreeEAString4CB6D0 = 0x4CB6D0;
uintptr_t _F_CallGetPointer7E4230 = 0x7E4230;
uintptr_t _F_Call4D8420 = 0x4D8420;
uintptr_t _F_Call416830 = 0x416830;

uintptr_t _F_Call4EDEC0 = 0x4EDEC0;

// void(pGameObject, useless, pAttributeModifier, int)
uintptr_t _F_GiveAttributeModifierInGroupToObject7181F0 = 0x7181F0;
// see 0058F950
uintptr_t _F_UnitGarrison572100 = 0x572100;

void __fastcall InitializeUtiliyGameFunctionsOrigin(uintptr_t hmodEXE)
{
	_F_CallRandomRadius = hmodEXE + 0x23F990;
	_F_ToMemoryAllocation = 0x416740;
	//
	_F_SkirmishSettingsPointerCDBBE0 = 0xCE0D80;
	_F_GameTextPointerCDB754 = 0xCE08F4;
	_F_GameObjectHashCE6C58 = 0xCEBDE8;
	_F_RuleSettingsCE3A74 = 0xCE8C04;

	_F_CallGetEAString4CB750 = hmodEXE + 0xCBAB0;
	_F_CallFreeEAString4CB6D0 = hmodEXE + 0xCBA30;
	_F_CallGetPointer7E4230 = hmodEXE + 0x422510;
	_F_Call4D8420 = hmodEXE + 0x11A210;
	_F_Call416830 = hmodEXE + 0x16A00;
	_F_Call4EDEC0 = 0x52F7D0;

	_F_GiveAttributeModifierInGroupToObject7181F0 = 0x756640;
	_F_UnitGarrison572100 = 0x5B37F0;
}
