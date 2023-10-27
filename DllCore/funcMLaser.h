#pragma once

enum SweepingLaserType {
	SweepLaserT_Original,
	SweepLaserT_Enhanced,
	SweepLaserT_toTarget0dot1,
	SweepLaserT_baseRotation,
	SweepLaserT_formSourceToTarget,
};

int __fastcall SweepingLaserStateGetID(uintptr_t ptr);
void __fastcall SweepingLaserStateCPP1(uintptr_t ptr, int cfg);
void __fastcall SweepingLaserStateASM1(uintptr_t ptr);

void __fastcall ActivateLaserNuggetASM(uintptr_t ptr);

struct SweepingLaserPos
{
	float x;
	float y;
	float z;
};
void __fastcall SweepingLaserActivateCPP(uintptr_t ptr, SweepingLaserPos* inPtr, float inXMM3);
void __fastcall SweepingLaserActivateASM(uintptr_t ptr);
