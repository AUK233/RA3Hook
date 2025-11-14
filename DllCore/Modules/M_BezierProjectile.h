#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_BezierProjectile_t : baseCommonModule_t {
	char pad24[0x34];
	float pos1[3], pos2[3];
	float deltaPos[3];
	float ammospeed; // is WeaponSpeed / logicframe in 0077B8E4
	int int80, alivetime;
	char pad88[0x40];
	float CurrentSpeed; // really?
}*pM_BezierProjectile;

typedef struct Data_BezierProjectile_t : baseBinDataHeader_t {
	char pad08[0x10];
	float FireStraightDistance; // maybe
	float FirstHeightMin, FirstHeightMax;
	float SecondHeightMin, SecondHeightMax;
	float FirstPercentIndent, SecondPercentIndent;
	char pad34[0xB4];
	float MaxDistanceToTravel, TargetVectorOffset, Acceleration;
}*pData_BezierProjectile;

namespace RA3::Module {
	void __fastcall M_BezierProjectile_Hook();
	void __fastcall M_BezierProjectile_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_BezierProjectile_InitializeASM();
	int __fastcall M_BezierProjectile_Module00CPP(char* pIn);
}
