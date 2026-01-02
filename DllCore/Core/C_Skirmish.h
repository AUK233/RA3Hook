#pragma once

typedef void(__fastcall* CallFunc_SetThisPlayerAsAnEnemyToEveryone)(void* pIn, void* callFunc, const char* PlayerName, const int pSize);
typedef void(__cdecl* CallFunc_SetThisPlayerAsEnemy)(void* pIn, void* a1, void* a2);

namespace RA3::Core {
	void __fastcall C_Skirmish_Hook();
	void __fastcall C_Skirmish_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall C_Skirmish_SetStartCashASM();
	int __fastcall C_Skirmish_SetAdditionalPlayersCPP(const char* inPlayerName, const int inSize);
}
