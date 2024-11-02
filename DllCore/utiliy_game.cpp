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

uintptr_t _F_CallGetEAString4CB750 = 0x4CB750;
uintptr_t _F_CallFreeEAString4CB6D0 = 0x4CB6D0;
uintptr_t _F_CallGetPointer7E4230 = 0x7E4230;
uintptr_t _F_Call4D8420 = 0x4D8420;
uintptr_t _F_Call416830 = 0x416830;

void __fastcall InitializeUtiliyGameFunctionsOrigin(uintptr_t hmodEXE)
{
	_F_CallRandomRadius = hmodEXE + 0x23F990;
	_F_ToMemoryAllocation = 0x416740;
}
