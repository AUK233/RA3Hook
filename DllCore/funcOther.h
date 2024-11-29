#pragma once

void __fastcall SetNoBloomASM();

void __fastcall KillTibCrystalWhenEmptyASM1();
void __fastcall KillTibCrystalWhenEmptyASM2();
void __fastcall BehaviorUpdate_TiberiumCrystal();

struct BuildList_Produced
{
	BYTE pad1[0x10];
	void* objectHash;
	BYTE pad2[0x40];
	BuildList_Produced* next;
};

struct BuildList_Producer
{
	BYTE pad1[0x10];
	BuildList_Produced* firstUnit;
	BuildList_Produced* lastUnit;
	int32_t uselessCount;
	int32_t buildCount;
};

int32_t __fastcall BuildList_GetNewBuildCountASM(BuildList_Producer* pBuilder, int32_t shiftOn);
int32_t __fastcall BuildList_GetNewBuildCountCPP(BuildList_Producer* pBuilder, int32_t shiftOn, uintptr_t* pObjectHash);

void __fastcall SecondaryObjectListenerModule_Initialize();
void __fastcall SecondaryObjectListenerModule_SetupUpgrade1();
void __fastcall SecondaryObjectListenerModule_SetupUpgrade2(void* pModule, void* edx, void* pUpgrade, int32_t Gcount);

void __fastcall newForceFeedback0x5216D0ASM(void* pthis, void* edx, void* projectile, void* attacker);
