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

__declspec(naked) void __fastcall AttributeModifierNo18BuffASM()
{
	__asm {
	// check it is 1.0f
		cmp dword ptr [esp + 0x18 + 4], 0x3F800000
		je returnOld
		addss xmm0, [esp + 0x18 + 4]
		ret
	// they need +4 because of esp has decreased by 4
	returnOld :
		//mulss xmm0, [esp + 0x18 + 4]
		ret
	}
}

float baseWarheadTargetPositionX = 1.0f;
float baseWarheadTargetPositionY = 1.0f;

__declspec(naked) void __fastcall WeaponScatterRadiusFixASM1()
{
	__asm {
		mov eax, [esp + 0x2C + 4] // Because EIP comes into the stack
		mov baseWarheadTargetPositionX, eax
		mov eax, [esp + 0x2C + 8]
		mov baseWarheadTargetPositionY, eax
		lea eax, [ebp + 8]
		mov ebp, [eax]
		ret
	}
}

__declspec(naked) void __fastcall WeaponScatterRadiusFixASM2()
{
	__asm {
		mov eax, baseWarheadTargetPositionX
		mov [esp + 0x2C + 4], eax
		mov ecx, baseWarheadTargetPositionY
		mov [esp + 0x2C + 8], ecx
		mov eax, [esi + 0x18]
		mov ecx, [esi + 0x1C]
		ret
	}
}

void __fastcall WeaponScatterRadiusCPP(float* pRadian, float* pPos)
{
	float radian = pRadian[1];
	float sqrtF2 = sqrt(pRadian[2]);
	pPos[0] += cos(radian) * pRadian[0] * sqrtF2;
	pPos[1] += sin(radian) * pRadian[0] * sqrtF2;
}

extern uintptr_t _Ret_WeaponScatterRadius;
extern uintptr_t _F_CallRandomRadius;

__declspec(naked) void __fastcall WeaponScatterRadiusASM()
{
	/*
	__asm {
		fstp dword ptr[esp]
		call _F_CallRandomRadius
		fstp dword ptr[esp + 8]
		//
		fld1
		fstp dword ptr[esp + 4]
		fldz
		fstp dword ptr[esp]
		call _F_CallRandomRadius
		fstp dword ptr[esp + 12]
		//
		//mov eax, [esp + 0x58]
		mov eax, [esp + 0x1C]
		mov [esp + 4], eax
		lea ecx, [esp + 4]
		mov edx, esi
		call WeaponScatterRadiusCPP
		//
		add esp, 0x10
		jmp _Ret_WeaponScatterRadius
	}*/
	// UR+326e94
	/**/
	__asm {
		fstp dword ptr[esp]
		call _F_CallRandomRadius
		add esp, 0x10
		fstp dword ptr[esp + 0x48]
		//
		sub esp, 8
		fld1
		fstp dword ptr[esp + 4]
		fldz
		fstp dword ptr[esp]
		call _F_CallRandomRadius
		fstp dword ptr[esp]
		fld dword ptr[esp]
		fsqrt
		fstp dword ptr[esp + 4]
		//
		fld dword ptr[esp + 0xC + 8]
		fmul dword ptr[esp + 4]
		fld dword ptr[esp + 0x48 + 8]
		fld st(0)
		fcos
		fmul st(0), st(2)
		fxch
		fsin
		fmulp st(2), st(0)
		fadd dword ptr[esi]
		fstp dword ptr[esi]
		fadd dword ptr[esi + 4]
		fstp dword ptr[esi + 4]
		add esp, 8
		jmp _Ret_WeaponScatterRadius
	}
}
