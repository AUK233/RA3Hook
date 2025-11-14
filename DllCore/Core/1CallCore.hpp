#pragma once

#include "C_Subsystem.h"
#include "C_AttributeModifier.h"
#include "C_Weapon.h"

namespace RA3::Core {

	void __fastcall HookFunctionSeries_Core() {
		C_Subsystem_Hook();
		C_AttributeModifier_Hook();
		C_Weapon_Hook();
	}

	void __fastcall InitializeHookFunctionSeries_Core(uintptr_t hmodEXE, int isNewSteam) {
		C_Subsystem_Initialize(hmodEXE, isNewSteam);
		C_AttributeModifier_Initialize(hmodEXE, isNewSteam);
		C_Weapon_Initialize(hmodEXE, isNewSteam);
	}

	// end namespace RA3::Core
}
