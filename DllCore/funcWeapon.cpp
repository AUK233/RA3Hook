#include "pch.h"
#include <format>

#include "utiliy.h"
#include "utiliy_game.h"
#include "commonData.hpp"
#include "commonStruct.hpp"

#include "funcWeapon.h"

extern uintptr_t _Ret_WeaponScatterRadius;
extern uintptr_t _F_CallRandomRadius;

namespace RA3::Weapon {

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
			cmp byte ptr[eax + 0x134], 1
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
		mov eax, [esp + 0x14]
		mov ecx, [eax + 4]
		mov eax, [ecx + 0xAC]
		shr eax, 3
		test al, 1 // check Flags="LENGTH_SCATTER"
		jne ofs35AA8A
		mov eax, baseWarheadTargetPositionX
		mov [esp + 0x2C + 4], eax
		mov ecx, baseWarheadTargetPositionY
		mov [esp + 0x2C + 8], ecx
	ofs35AA8A:
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


// Fix weapon scatter radius (now it is useless)
uintptr_t _F_WeaponScatterRadius = 0x00400000 + 0x3140CB;
uintptr_t _Ret_WeaponScatterRadius = 0x00400000 + 0x3140F0;
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

float EMPTimeToDamageShieldFactor = 100.0f;
uintptr_t _Ret_CheckNoEMPInUnitShield = 0x71E141;
__declspec(naked) bool __fastcall CheckNoEMPInUnitShieldASM(void* pShieldBody)
{
	__asm {
		sub esp, 0xC
		push esi
		mov esi, ecx
		mov eax, [esi + 0x8]
		mov ecx, [eax + 0x8C]
		shr ecx, 0x1A
		test cl, 1
		je hasEMP
		mov edx, [eax + 0x88]
		shr edx, 0xC
		test dl, 1
		jne hasEMP
		movss xmm1, dword ptr[esi + 0x12C]
		xorps xmm0, xmm0
		comiss xmm1, xmm0
		jbe hasEMP // if <= 0
		// new block
		mov eax, [edi + 0x14] // edi is caller's
		movss xmm2, dword ptr[eax + 0x2C] // emp time
		mulss xmm2, EMPTimeToDamageShieldFactor
		subss xmm1, xmm2 // reduce shield
		comiss xmm1, xmm0
		jbe ofs31E141 // if <= 0
		movss dword ptr[esi + 0x12C], xmm1 // write back to current shield value
		mov al, 1
		pop esi
		add esp, 0xC
		ret
		// new block end
	ofs31E141:
		jmp _Ret_CheckNoEMPInUnitShield
	hasEMP:
		xor al, al
		pop esi
		add esp, 0xC
		ret
	}
}

uintptr_t _F_ShowAmmo = 0x00400000 + 0x128746;
uintptr_t _F_WeaponReloadActive = 0x00400000 + 0x3BE05F;
uintptr_t _F_WeaponReloadTimeCount = 0x00400000 + 0x2DC270;
// Fix weapon scatter radius
uintptr_t _F_WeaponScatterRadius1 = 0x00400000 + 0x35AA6B;
uintptr_t _F_WeaponScatterRadius2 = 0x00400000 + 0x35AA8A;
// Convert emp time to unit shield damage
uintptr_t _F_CheckNoEMPInUnitShield = 0x7CFAB3;

void __fastcall InitializeHookWeaponFunctionUpdateOrigin(uintptr_t hmodEXE)
{
	_F_ShowAmmo = hmodEXE + 0x169D96;

	_F_WeaponReloadActive = hmodEXE + 0x3FC3AF;
	_F_WeaponReloadTimeCount = hmodEXE + 0x31A7E0;


	_F_WeaponScatterRadius1 = hmodEXE + 0x3990AB;
	_F_WeaponScatterRadius2 = hmodEXE + 0x3990CA;

	_F_CheckNoEMPInUnitShield = hmodEXE + 0x40DDD3;
	_Ret_CheckNoEMPInUnitShield = hmodEXE + 0x35C541;
}

void __fastcall HookWeaponFunctionUpdate()
{
	// Let "ShowsAmmoPips" work
	hookGameCall((void*)_F_ShowAmmo, (uintptr_t)ShowsAmmoPipsASM);
	WriteHookToProcess((void*)(_F_ShowAmmo + 5), (void*)&nop2, 2U);
	// Set up reloading ammunition
	hookGameCall((void*)_F_WeaponReloadActive, (uintptr_t)WeaponReloadActiveASM);
	WriteHookToProcess((void*)(_F_WeaponReloadActive + 5), (void*)&nop1, 1U);
	hookGameCall((void*)_F_WeaponReloadTimeCount, (uintptr_t)WeaponReloadTimeCountASM);
	WriteHookToProcess((void*)(_F_WeaponReloadTimeCount + 5), (void*)&nop1, 1U);


	// Fix weapon scatter radius
	//hookGameBlock((void*)_F_WeaponScatterRadius, (uintptr_t)WeaponScatterRadiusASM);
	//WriteHookToProcess((void*)(_F_WeaponScatterRadius + 5), &nop3, 3U);
	hookGameCall((void*)_F_WeaponScatterRadius1, (uintptr_t)WeaponScatterRadiusFixASM1);
	hookGameCall((void*)_F_WeaponScatterRadius2, (uintptr_t)WeaponScatterRadiusFixASM2);
	WriteHookToProcess((void*)(_F_WeaponScatterRadius2 + 5), (void*)&nop1, 1U);

	// Convert emp time to unit shield damage
	hookGameCall((void*)_F_CheckNoEMPInUnitShield, (uintptr_t)CheckNoEMPInUnitShieldASM);
}

}