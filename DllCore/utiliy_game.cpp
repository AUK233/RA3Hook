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

void __fastcall InitializeUtiliyGameFunctionsOrigin(uintptr_t hmodEXE)
{
	_F_CallRandomRadius = hmodEXE + 0x23F990;
	_F_ToMemoryAllocation = 0x416740;
}
