#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "M_AttributeModifierAuraUpdate.h"

namespace RA3::Module {
	CallFunc_InitializeModule M_AttributeModifierAuraUpdate_ModuleInit = (CallFunc_InitializeModule)0x70DAA0;
	CallFunc_InitializeModule M_LargeGroupBonusUpdate_ModuleInit = (CallFunc_InitializeModule)0x6F2BA0;

	void __fastcall M_AttributeModifierAura_Hook()
	{
		/*hookGameBlock((void*)0x7E1080, (uintptr_t)M_AttributeModifierAuraUpdate_AllocateMemoryAndInit);
		WriteHookToProcess((void*)(0x7E1080 + 5), (void*)&nop2, 2U);*/

		// DllCore.dll+79BE 
	}

	void __fastcall M_AttributeModifierAura_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
	}

	void* __fastcall M_AttributeModifierAuraUpdate_AllocateMemoryAndInit(void* pECX, void* useless, void* pA1, void* pModuleData)
	{
		void* p = 0;
		pData_AttributeModifierAuraUpdate pModule = (pData_AttributeModifierAuraUpdate)pModuleData;
		if (pModule->IDHash == 3409082633U) {
			// ModuleTag_LargeGroupBonusUpdate
			// 30 in 007E0740
			p = newMemoryAllocation(0x30 + sizeof(Data_LargeGroupBonusUpdate_t));
			if (p) {
				pData_LargeGroupBonusUpdate pData = (pData_LargeGroupBonusUpdate)((char*)p + 0x30);
				pData->ModuleHash = 0xDE31D539;
				pData->IDHash = pModule->IDHash;
				pData->UpdateInterval = pModule->RefreshDelay;
				pData->MinCount = pModule->MaxActiveRank;
				pData->MinDist = pModule->Range;
				pData->RubOffRadius = pModule->Range*2;
				pData->FlagSubObjIds = nullptr;
				pData->pAttributeModifier = pModule->pAttributeModifier;
				pData->pObjectFilter = pModule->pObjectFilter;
				pData->AlliesOnly = pModule->AllowSelf;

				CallFunc_InitializeModule call = M_LargeGroupBonusUpdate_ModuleInit;
				p = call(p, call, pA1, pData);
			}
			return p;
		}
		else {
			// 2C in 007E1080
			p = newMemoryAllocation(0x2C);
			if (p) {
				CallFunc_InitializeModule call = M_AttributeModifierAuraUpdate_ModuleInit;
				p = call(p, call, pA1, pModuleData);
			}
			return p;
		}
		// end
	}

	// end namespace RA3::Module
}