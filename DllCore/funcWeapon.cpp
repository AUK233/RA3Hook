#include "pch.h"
#include <format>

#include "funcWeapon.h"

// Let "ShowsAmmoPips" work
__declspec(naked) void __fastcall ShowsAmmoPipsASM()
{
	// edx is not display
	__asm {
			push eax
			// AutoReloadsClip is RETURN_TO_BASE
			cmp dword ptr[eax + 0xB8], 2
			je SetFlag
			// ShowsAmmoPips is true
			cmp dword ptr[eax + 0x134], 1
			je SetFlag
			mov eax, 1
			test eax, eax
			pop eax
			ret
		SetFlag :
			xor eax, eax
			test eax, eax
			pop eax
			ret
	}
}

__declspec(naked) void __fastcall WeaponReloadActiveASM()
{
	__asm {
		push eax
		mov edx, [ecx + 0x1C]
	// check reload type
		mov eax, [ecx + 4]
		mov eax, [eax + 4]
		cmp dword ptr [eax+0xB8], 0
		jne OldBlock // if it != auto
	// current ammo == 0
		cmp edx, 0
		je OldBlock
	// total ammo < current ammo
		mov eax, [esp] // get eax from "push eax"
		cmp eax, edx
		jbe OldBlock
		// store registers
		sub esp, 8
		mov [esp+4], edx
		mov [esp], eax
		// calculate reload time
		sub eax, edx // total ammo - current ammo
		mul dword ptr [esi + 0x28] // *= reload time
		div dword ptr [esp] // /= total ammo
		mov [esi + 0x28], eax // write to reload time
		// clear clip
		xor eax, eax
		mov [ecx + 0x1C], eax
		// restore registers
		mov edx, [esp + 4]
		add esp, 12 // don't forget "push eax"s 4-bytes
		ret
		//
	OldBlock:
		pop eax
		mov [ecx + 0x1C], eax
		ret
	}
}

__declspec(naked) void __fastcall WeaponReloadTimeCountASM()
{
	__asm {
		// reload time
		mov eax, [ecx + 0x28]
		// current time
		add eax, [ecx + 0x24]
		cmp edi, eax
		jb returnOld
		// get total ammo
		push eax
		mov ecx, [ecx + 0x20]
		mov eax, [ecx + 0x4]
		mov eax, [eax + 0x4]
		mov eax, [eax + 0x70]
		mov [ecx + 0x1C], eax
		pop eax
		//
	returnOld :
		ret
	}
}
