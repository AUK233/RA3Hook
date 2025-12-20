#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_BezierProjectile_t : baseCommonModule_t {
	char pad24[0xC];
	float posInBase[3]; // position of fire object
	char pad3C[0x1C];
	float StartPos[3], EndPos[3];
	float deltaPos[3];
	float ammospeed; // is WeaponSpeed / logicframe in 0077B8E4
	int int80, alivetime;
	char pad88[0x40];
	float CurrentSpeed; // really?
}*pM_BezierProjectile;
#if 1
	static_assert(offsetof(M_BezierProjectile_t, posInBase) == 0x30);
	static_assert(offsetof(M_BezierProjectile_t, ammospeed) == 0x7C);
	static_assert(offsetof(M_BezierProjectile_t, CurrentSpeed) == 0xC8);
#endif
// +D1 a byte now to check for ground collision

typedef struct Data_BezierProjectile_t : baseBinDataHeader_t {
	int Flags;
	int Type;
	char pad10[8];
	float FireStraightDistance; // maybe
	float FirstHeightMin, FirstHeightMax;
	float SecondHeightMin, SecondHeightMax;
	float FirstPercentIndent, SecondPercentIndent;
	char pad34[0xB4];
	float MaxDistanceToTravel, TargetVectorOffset, Acceleration;
}*pData_BezierProjectile;
#if 1
	static_assert(offsetof(Data_BezierProjectile_t, Type) == 0xC);
	static_assert(offsetof(Data_BezierProjectile_t, TargetVectorOffset) == 0xEC);
	static_assert(offsetof(Data_BezierProjectile_t, Acceleration) == 0xF0);
#endif

namespace RA3::Module {
	void __fastcall M_BezierProjectile_Hook();
	void __fastcall M_BezierProjectile_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_BezierProjectile_InitializeASM();
	int __fastcall M_BezierProjectile_Module00CPP(char* pIn);

	void __fastcall M_BezierProjectile_CheckProjectilePositionASM();
	void __fastcall M_BezierProjectile_CheckProjectilePositionCPP(pM_BezierProjectile pIn, void* pGO, float DefaultHeight);
}
