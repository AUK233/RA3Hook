#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct Data_AttributeModifierAuraUpdate_t : baseBinDataHeader_t {
	char pad08[0x1C];
	void* pAttributeModifier;
	float RefreshDelay, Range;
	char pad30[0xC];
	int MaxActiveRank;
	char pad40[0x1C];
	void* pObjectFilter;
	bool AllowPowerWhenAttacking, InitiallyActive, RunWhileDead, AllowSelf;
	bool AffectContainedOnly, AffectHordeMembersOnly, AffectAttacheeOnly, ShowDebugCircle;
}*pData_AttributeModifierAuraUpdate;
static_assert(offsetof(Data_AttributeModifierAuraUpdate_t, Range) == 0x2C);
static_assert(offsetof(Data_AttributeModifierAuraUpdate_t, InitiallyActive) == 0x61);

typedef struct Data_LargeGroupBonusUpdate_t : baseBinDataHeader_t {
	float UpdateInterval;
	int MinCount;
	float MinDist, RubOffRadius;
	char* FlagSubObjIds;
	void* pAttributeModifier;
	void* pObjectFilter;
	bool AlliesOnly;
}*pData_LargeGroupBonusUpdate;

namespace RA3::Module {
	void __fastcall M_AttributeModifierAura_Hook();
	void __fastcall M_AttributeModifierAura_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void* __fastcall M_AttributeModifierAuraUpdate_AllocateMemoryAndInit(void* pECX, void* useless, void* pA1, void* pModuleData);
}
