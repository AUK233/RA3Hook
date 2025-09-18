#pragma once

// size is 0x320, create in 004FFCF0
// pointer is 00CD8CE4 
typedef struct G_GameSystem_t {
	void* vf_table; // 00BEB630
	char pad0[0x4C];
	int IngameTimer; // every logical frame +1, is current match's time.
	int IngameTimerWithSecond; // update once per second, that is a frame, is current match's time.
	void* pointer58;
	char pad5C[0x8];
	int GameTimerWithSecond; // update once per second, that is a frame, is total time since game started.
}*pG_GameSystem;
pG_GameSystem GS_GetGameSystemPointer();


void __fastcall G_GlobalStructure_Initialize(uintptr_t hmodEXE, int isNewSteam);
