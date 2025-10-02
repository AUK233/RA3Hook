#pragma once

namespace RA3::Core {
	void __fastcall C_AttributeModifier_Hook();
	void __fastcall C_AttributeModifier_Initialize(uintptr_t hmodEXE, int isNewSteam);

	// BOUNTY_PERCENTAGE is 18
	void __fastcall C_AttributeModifier_BuffState1ASM();
	void __fastcall C_AttributeModifier_T18CheckBuffASM();
}
