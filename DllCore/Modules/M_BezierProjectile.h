#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_BezierProjectile_t : baseCommonModule_t {
	char pad24[0x58];
	float ammospeed; // is WeaponSpeed / logicframe in 0077B8E4
	char pad80[0x48];
	float CurrentSpeed; // really?
}*pM_BezierProjectile;
static_assert(offsetof(M_BezierProjectile_t, ammospeed) == 0x7C);

typedef struct Data_BezierProjectile_t : baseBinDataHeader_t {
	char pad08[0x10];
	float FireStraightDistance; // maybe
	float FirstHeightMin, FirstHeightMax;
	float SecondHeightMin, SecondHeightMax;
	float FirstPercentIndent, SecondPercentIndent;
	char pad34[0xB4];
	float MaxDistanceToTravel, TargetVectorOffset, Acceleration;
}*pData_BezierProjectile;
static_assert(offsetof(Data_BezierProjectile_t, Acceleration) == 0xF0);
