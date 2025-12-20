#pragma once

#include "C_System.h"
#include "C_Subsystem.h"
#include "C_GameObject.h"
#include "C_AttributeModifier.h"
#include "C_Weapon.h"

namespace RA3::Core {

	void __fastcall HookFunctionSeries_Core() {
		C_System_Hook();
		C_Subsystem_Hook();
		C_GameObject_Hook();
		C_AttributeModifier_Hook();
		C_Weapon_Hook();
	}

	void __fastcall InitializeHookFunctionSeries_Core(uintptr_t hmodEXE, int isNewSteam) {
		C_System_Initialize(hmodEXE, isNewSteam);
		C_Subsystem_Initialize(hmodEXE, isNewSteam);
		C_GameObject_Initialize(hmodEXE, isNewSteam);
		C_AttributeModifier_Initialize(hmodEXE, isNewSteam);
		C_Weapon_Initialize(hmodEXE, isNewSteam);
	}

	// end namespace RA3::Core
}
