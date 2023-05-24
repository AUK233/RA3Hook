#include "pch.h"
#include <format>

extern int noBloomSet;
extern uintptr_t _Ret_BloomOpen;

__declspec(naked) void __fastcall SetNoBloomASM()
{
	__asm {
			test eax, eax
			je ofs5FB9E8
			cmp noBloomSet, 0
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
