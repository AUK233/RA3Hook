#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_ShieldSphereUpdate_t : baseCommonModule_t {
	char pad[0x24];
	float ShieldCurrentDamage, ShieldMaxDamage;
	float ShieldDeltaRadius; // is RadiusMax - RadiusMin
	float ShieldDamageChange; // increased damage value per logic
	UINT32 ShieldCurrentState, ShieldMaxState; // maybe? see 0078A1F4
	char padEnd[0x8]; // pad size to 0x6C
	bool bIsActivate;
	bool bAlwaysDisplayBar; // new
	char pad6A[2];
}*pM_ShieldSphereUpdate;

typedef struct Data_ShieldSphereUpdate_t : baseBinDataHeader_t {
	float RadiusMax, RadiusMin;
	char DamageTypesNotToAbsorbBit[8];
	float ScanFrequency, Duration; // unit is seconds
	float MaxDamage;
	char BitFlagsPad[0x5C];
	char* ShieldBoneName;
	float ShieldSizeMultiplier;
}*pData_ShieldSphereUpdate;

namespace RA3::Module {
	void __fastcall M_ShieldSphereUpdate_Hook();
	void __fastcall M_ShieldSphereUpdate_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_ShieldSphereUpdate_InitializeASM();
	void __fastcall M_ShieldSphereUpdate_InitializeCPP(pM_ShieldSphereUpdate pIn, pData_ShieldSphereUpdate pData);

	int __fastcall M_ShieldSphereUpdate_Module00_ChargeableASM(char* pIn);
	int __fastcall M_ShieldSphereUpdate_Module00_ChargeableCPP(pM_ShieldSphereUpdate pIn);
	void __fastcall M_ShieldSphereUpdate_Module0C_Chargeable(char* pIn, int, int a1);

	void __fastcall M_ShieldSphereUpdate_DisplayShieldBarASM();
}
