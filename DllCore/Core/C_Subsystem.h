#pragma once


typedef void(__fastcall* CallFunc_SetSpecialPowerToPublicTimer)(void* pIn);

namespace RA3::Core {
	void __fastcall C_Subsystem_Hook();
	void __fastcall C_Subsystem_Initialize(uintptr_t hmodEXE, int isNewSteam);
	
	void __fastcall C_Skirmish_UpdateSpecialPowerPublicTimerListASM();
}
