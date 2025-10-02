#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_Weapon.h"

namespace RA3::Core {

	uintptr_t _F_SetupPreAttackWeaponPos = 0x79FB6E;

	void __fastcall C_Weapon_Hook()
	{
		static_assert(offsetof(C_Weapon_t, FirePosNoTarget) == 0x40);
		static_assert(offsetof(C_Weapon_t, FirePosInTarget) == 0x4C);
		static_assert(offsetof(C_Weapon_t, bHasTarget) == 0x62);
		static_assert(offsetof(C_Weapon_t, FireOutPos) == 0x6C);

		hookGameCall((void*)_F_SetupPreAttackWeaponPos, (uintptr_t)C_Weapon_SetupPreAttackWeaponPosASM);
		WriteHookToProcess((void*)(_F_SetupPreAttackWeaponPos + 5), (void*)&nop2, 2U);
	}

	void __fastcall C_Weapon_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {}
	}

	__declspec(naked) void __fastcall C_Weapon_SetupPreAttackWeaponPosASM()
	{
		// esp need +4 because of esp has decreased by 4
		__asm {
			// ShowsAmmoPips is true
			cmp dword ptr[edx + 0x134 - 0x10], 1 // Requires -10h, because it's not header.
			lea edx, [esp + 0x10 + 4]
			jne addr79FB6E
			test ecx, ecx
			jz NoTarget // = 0
			xor ecx, ecx
			lea eax, [esi + 0x4C] // FirePosInTarget
			ret
		NoTarget:
			lea eax, [esi + 0x40] // FirePosNoTarget
			ret
		addr79FB6E:
			lea eax, [edi + 0x38] // GameObject self pos
			ret
		}
	}

// end namespace RA3::Core
}