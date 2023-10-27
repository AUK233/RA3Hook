#include "pch.h"
#include <format>

#include "funcOther.h"
#include "commonStruct.h"

extern inputSettingINFO inputSetting;
extern uintptr_t _F_CallKillGameObject;
extern uintptr_t _Ret_BloomOpen;

__declspec(naked) void __fastcall SetNoBloomASM()
{
	__asm {
			test eax, eax
			je ofs5FB9E8
			cmp inputSetting.CheckBloom, 0
			jne ofs5FB9EC
			cmp dword ptr[eax + 0x2EC], 1
			jle ofs5FB9EC
		ofs5FB9E8:
			mov bl, 1
			jmp _Ret_BloomOpen
		ofs5FB9EC:
			xor bl, bl
			jmp _Ret_BloomOpen
	}
}

__declspec(naked) void __fastcall KillTibCrystalWhenEmptyASM1()
{
	__asm {
		// check if the mine is empty
		cmp dword ptr[edi + 0x30], 0
		jg ofs422717
		push 0
		push 8 // SUICIDED
		push 6
		mov ecx, [edi + 8]
		call _F_CallKillGameObject
		add esp, 4
		pop edi
		pop esi
		pop ebp
		mov eax, -2
		pop ebx
		add esp, 0x18
		ret
	ofs422717:
		mov edx, [esi + 0x28]
		fild dword ptr[esi + 0x28]
		ret
	}
}

__declspec(naked) void __fastcall KillTibCrystalWhenEmptyASM2()
{
	__asm {
		// check if the mine is empty
		cmp dword ptr[edi + 0x30], 0
		jne ofs42278D
		push 0
		push 8 // SUICIDED
		push 6
		mov ecx, [edi+8]
		call _F_CallKillGameObject
	ofs42278D:
		add [esi + 0x2C], ebp
		test ebp, ebp
		ret
	}
}
