#include "../pch.h"
#include "GlobalStructure.h"

uintptr_t _F_GameSystemCD8CE4 = 0xCD8CE4;
pG_GameSystem GS_GetGameSystemPointer()
{
	return *(pG_GameSystem*)_F_GameSystemCD8CE4;
}

void __fastcall G_GlobalStructure_Initialize(uintptr_t hmodEXE, int isNewSteam)
{
	if (isNewSteam) {
		_F_GameSystemCD8CE4 = 0xCDDE84;
	}
}
