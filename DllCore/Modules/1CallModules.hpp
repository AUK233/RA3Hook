#pragma once

#include "M_LaserState.h"
#include "M_ShieldSphereUpdate.h"

namespace RA3::Module {

void __fastcall HookFunctionSeries_Module() {
	M_LaserState_Hook();
	M_ShieldSphereUpdate_Hook();
}

void __fastcall InitializeHookFunctionSeries_Module(uintptr_t hmodEXE, int isNewSteam) {
	M_LaserState_Initialize(hmodEXE, isNewSteam);
	M_ShieldSphereUpdate_Initialize(hmodEXE, isNewSteam);
}

// end namespace RA3::Module
}
