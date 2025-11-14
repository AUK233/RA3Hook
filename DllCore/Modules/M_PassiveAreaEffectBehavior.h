#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_PassiveAreaEffectBehavior_t : baseCommonModule_t {
	char pad24[0x1c];
	bool bIsActivate; // really?
	// next is new
	UINT8 func_type; // requires additional functions to be executed
}*pM_PassiveAreaEffectBehavior;

typedef struct Data_PassiveAreaEffectBehavior_t : baseBinDataHeader_t {
	float EffectRadius;
	int MaxBeneficiaries;
	float HealingPercentPerSecond, HealingPointsPerSecond;
	UINT32 SurveyDelayFrames;
	char pad18[0x14];
	int ModifierCount;
	void* pModifier;
	void* pAllowFilter;
	void* pContainerFilter;
	void* pObjectStatusValidation;
	bool NonStackable, AffectAttached, AffectWoundedOnly, AffectUnderAttack;
	bool AffectContained;
}*pData_PassiveAreaEffectBehavior;

namespace RA3::Module {
	enum PassiveAreaEffectBehaviorType {
		PassiveAreaEffectBehaviorT_Default,
		PassiveAreaEffectBehaviorT_LargeGroupBonus,
		PassiveAreaEffectBehaviorT_UnitGarrison,
	};

	void __fastcall M_PassiveAreaEffectBehavior_Hook();
	void __fastcall M_PassiveAreaEffectBehavior_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_PassiveAreaEffectBehavior_InitASM();
	void __fastcall M_PassiveAreaEffectBehavior_InitCPP(pM_PassiveAreaEffectBehavior pModule);
	void __fastcall M_PassiveAreaEffectBehavior_HealUnitASM();
	void __fastcall M_PassiveAreaEffectBehavior_ExtraFuncCPP(pM_PassiveAreaEffectBehavior pModule, void* pHealUnit, int UnitCount);
	void __fastcall M_PassiveAreaEffectBehavior_SetLargeGroupBonusCPP(pM_PassiveAreaEffectBehavior pModule, pData_PassiveAreaEffectBehavior pData);
	void __fastcall M_PassiveAreaEffectBehavior_SetUnitGarrisonCPP(pM_PassiveAreaEffectBehavior pModule, void* pHealUnit);
}
