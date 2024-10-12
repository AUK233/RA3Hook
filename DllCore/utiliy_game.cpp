#include "pch.h"

#include "utiliy_game.h"

extern uintptr_t _F_CallRandomRadius;

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
