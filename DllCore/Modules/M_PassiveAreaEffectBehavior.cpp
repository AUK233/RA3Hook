#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Core/C_GameObject.h"
#include "M_PassiveAreaEffectBehavior.h"

extern uintptr_t _F_Call4EDEC0;
extern uintptr_t _F_GiveAttributeModifierInGroupToObject7181F0;
extern uintptr_t _F_UnitGarrison572100;

namespace RA3::Module {

	uintptr_t _F_PassiveAreaEffectBehaviorInitialize = 0x72D1F9;
	uintptr_t _F_PassiveAreaEffectBehaviorHealUnit = 0x763E27;
	uintptr_t _Ret_PassiveAreaEffectBehaviorHealUnit = 0x763E27 + 20;

	void __fastcall M_PassiveAreaEffectBehavior_Hook()
	{
		// ra3_1.12.game+3E1970
		hookGameBlock((void*)_F_PassiveAreaEffectBehaviorInitialize, (uintptr_t)M_PassiveAreaEffectBehavior_InitASM);

		hookGameBlock((void*)_F_PassiveAreaEffectBehaviorHealUnit, (uintptr_t)M_PassiveAreaEffectBehavior_HealUnitASM);
		WriteHookToProcess((void*)(_F_PassiveAreaEffectBehaviorHealUnit + 5), (void*)&nop3, 3U);
	}

	void __fastcall M_PassiveAreaEffectBehavior_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_PassiveAreaEffectBehaviorInitialize = 0x76B7B9;
			_F_PassiveAreaEffectBehaviorHealUnit = 0x7A22E7;
			_Ret_PassiveAreaEffectBehaviorHealUnit = 0x7A22E7 + 20;
		}
	}

	__declspec(naked) void __fastcall M_PassiveAreaEffectBehavior_InitASM()
	{
		__asm {
			call _F_Call4EDEC0
			mov ecx, esi
			call M_PassiveAreaEffectBehavior_InitCPP
		}
	ofs32D1FE:
		__asm {
			pop edi
			mov eax, esi
			pop esi
			pop ebx
			add esp, 8
			ret 8
		}
	}

	void __fastcall M_PassiveAreaEffectBehavior_InitCPP(pM_PassiveAreaEffectBehavior pModule)
	{
		pData_PassiveAreaEffectBehavior pData = (pData_PassiveAreaEffectBehavior)(pModule->pModuleData);
		UINT8 func_type = PassiveAreaEffectBehaviorT_Default;
		if (pData->IDHash == 3409082633U) {
			// ModuleTag_LargeGroupBonusUpdate
			func_type = PassiveAreaEffectBehaviorT_LargeGroupBonus;
		} else if (pData->IDHash == 3427333003U) {
			// ModuleTag_UnitGarrisonUpdate
			func_type = PassiveAreaEffectBehaviorT_UnitGarrison;
		}

		static_assert(offsetof(M_PassiveAreaEffectBehavior_t, bIsActivate) == 0x40);
		static_assert(offsetof(M_PassiveAreaEffectBehavior_t, func_type) == 0x41);
		pModule->func_type = func_type;
	}

	__declspec(naked) void __fastcall M_PassiveAreaEffectBehavior_HealUnitASM()
	{
		__asm {
			mov edx, [esp + 0x18]
			lea ecx, [edx-0x10]
			cmp byte ptr[edx + 0x41 - 0x10], 0
			je checkHealUnitExist
			push ebp
			mov edx, edi
			mov eax, _Ret_PassiveAreaEffectBehaviorHealUnit
			push eax
			jmp M_PassiveAreaEffectBehavior_ExtraFuncCPP
		}
	checkHealUnitExist:
		__asm {
			test edi, edi
			je ofs363E3B
			mov eax, [ecx]
			mov edx, [eax + 0x5C]
			push edi
			call edx
		}
	ofs363E3B:
		__asm {
			jmp _Ret_PassiveAreaEffectBehaviorHealUnit
		}
		// end
	}

	void __fastcall M_PassiveAreaEffectBehavior_ExtraFuncCPP(pM_PassiveAreaEffectBehavior pModule, void* pHealUnit, int UnitCount)
	{
		switch (pModule->func_type)
		{
		case PassiveAreaEffectBehaviorT_LargeGroupBonus: {
			pData_PassiveAreaEffectBehavior pData = (pData_PassiveAreaEffectBehavior)(pModule->pModuleData);
			if (UnitCount >= pData->MaxBeneficiaries) {
				M_PassiveAreaEffectBehavior_SetLargeGroupBonusCPP(pModule, pData);
			}
			return;
		}
		case PassiveAreaEffectBehaviorT_UnitGarrison: {
			if (pHealUnit) {
				M_PassiveAreaEffectBehavior_SetUnitGarrisonCPP(pModule, pHealUnit);
			}
			return;
		}
		default: {
			// default is heal.
			char* vft = (char*)pModule->vf_table;
			void_thiscallHasParameter1 callFunc = (void_thiscallHasParameter1)(vft+ 0x5C);
			callFunc(pModule, callFunc, (int)pHealUnit);
			return;
		}
		}
	}

	void __fastcall M_PassiveAreaEffectBehavior_SetLargeGroupBonusCPP(pM_PassiveAreaEffectBehavior pModule, pData_PassiveAreaEffectBehavior pData)
	{
		static_assert(offsetof(Data_PassiveAreaEffectBehavior_t, ModifierCount) == 0x30);
		int BuffCount = pData->ModifierCount;

		if (!BuffCount) {
			return;
		}

		// ra3_1.12.game+31CD96
		int* pBuff = (int*)pData->pModifier;
		void* pGameObject = pModule->pGameObject;
		void_thiscallHasParameter2 callFunc = (void_thiscallHasParameter2)_F_GiveAttributeModifierInGroupToObject7181F0;
		for(int i = 0; i < BuffCount; i++) {
			callFunc(pGameObject, callFunc, pBuff[i], -1);
		}
	}

	void __fastcall M_PassiveAreaEffectBehavior_SetUnitGarrisonCPP(pM_PassiveAreaEffectBehavior pModule, void* pHealUnit)
	{
		pC_GameObject pUnit = (pC_GameObject)pHealUnit;
		char* pAIUpdate = (char*)pUnit->pAIUpdateModule;
		if (!pAIUpdate) {
			return;
		}

		pData_PassiveAreaEffectBehavior pData = (pData_PassiveAreaEffectBehavior)(pModule->pModuleData);
		int BuffCount = pData->ModifierCount;
		if (BuffCount) {
			// yeah, only process the first one
			int* pBuff = (int*)pData->pModifier;
			void_thiscallHasParameter2 callBuff = (void_thiscallHasParameter2)_F_GiveAttributeModifierInGroupToObject7181F0;
			callBuff(pUnit, callBuff, pBuff[0], -1);
		}

		void_thiscallHasParameter2 callFunc = (void_thiscallHasParameter2)_F_UnitGarrison572100;
		void* pParent = pModule->pGameObject;
		callFunc(pAIUpdate + 0x24, callFunc, (int)pParent, 1);
	}

// end namespace RA3::Module
}
