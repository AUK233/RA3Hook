#pragma once
#include "../Base/CommonStructure.hpp"
#include "../Base/SSE.hpp"

typedef struct M_LaserState_t : baseCommonModule_t {
	void* vft24;
	void* vft28;
	void* pointer2C;
	float SelfPos[3], TargetPos[3];
	float Pos48[3]; // mayebe?
	int TargetGameObjectIndex; // target is which object, is a serial number on the map.
	int LaserTimer; // Laser activation time
	char pad5C[0x38]; // pad size to 0x98
	bool bIsActivated, pad95;
	UINT16 i16_LaserType; // new
}*pM_LaserState;

// create in 007E0A80
typedef struct M_SweepingLaserState_t : M_LaserState_t {
	float SweepEndPos[3], SweepStartPos[3];
	int Lifetime; // is frame
	char padB4[8];
	bool bIsSwapDirection; // If true, swap start and end positions
	bool bIsRotation; // todo!!!! If true, change to rotating sweep. (new)
	char padToC0[0x2];
}*pM_SweepingLaserState;

typedef struct Data_SweepingLaserState_t : baseBinDataHeader_t {
	int LaserId;
	char pad0C[0x18];
	float* pEndOffset;
	int pad28;
	float Radius;
}*pData_SweepingLaserState;

namespace RA3::Module {
	enum SweepingLaserType {
		SweepLaserT_Original,
		SweepLaserT_Enhanced,
		SweepLaserT_toTarget0dot1,
		SweepLaserT_baseRotation,
		SweepLaserT_formSourceToTarget,
		SweepLaserT_formSourceToDistance,
	};

	void __fastcall M_LaserState_Hook();
	void __fastcall M_LaserState_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_SweepingLaserState_InitializeASM();
	void __fastcall M_SweepingLaserState_InitializeCPP(pM_SweepingLaserState pIn);
	UINT16 __fastcall M_SweepingLaserState_GetLaserType(pData_SweepingLaserState pData);

	void __fastcall M_SweepingLaserState_vft58CPP(pM_SweepingLaserState pIn, int, int inLifetime, int a2);
	void __fastcall M_SweepingLaserState_vft5Ccpp(pM_SweepingLaserState pIn, int, float* pOutPos);

	void __fastcall M_SweepingLaserState_SetPos_Default(pM_SweepingLaserState pIn);
	void __fastcall M_SweepingLaserState_SetPos_Enhanced(pM_SweepingLaserState pIn);
	void __fastcall M_SweepingLaserState_SetPos_toTarget0dot1(uintptr_t ptr);
	void __fastcall M_SweepingLaserState_SetPos_baseRotation(uintptr_t ptr);
	void __fastcall M_SweepingLaserState_SetPos_formSourceToTarget(uintptr_t ptr);
	void __fastcall M_SweepingLaserState_SetPos_formSourceToDistance(uintptr_t ptr);


	void __fastcall ActivateLaserNuggetASM(uintptr_t ptr);
}
