#pragma once

#include "M_LaserState.h"

namespace RA3::Module {

void __fastcall HookFunctionSeries_Module() {
	M_LaserState_Hook();
}

void __fastcall InitializeHookFunctionSeries_Module(uintptr_t hmodEXE, int isNewSteam) {
	M_LaserState_Initialize(hmodEXE, isNewSteam);
}

// end namespace RA3::Module
}
