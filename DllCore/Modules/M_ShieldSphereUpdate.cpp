#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "M_ShieldSphereUpdate.h"

extern uintptr_t _F_Call4EDEC0;

namespace RA3::Module {

	uintptr_t _F_ShieldSphereUpdateInitialize = 0x7246EA;
	// is p+0x10
	uintptr_t _F_ShieldSphereUpdateVFT_M = 0xC3A548;

	void_thiscallHas1Float _call_ShieldSphereUpdate_ScaleModel = (void_thiscallHas1Float)0x70FE60;
	void_fastcallNoParameter _call_ShieldSphereUpdate_Reset = (void_fastcallNoParameter)0x73ED50;
	void_fastcallNoParameter _call_ShieldSphereUpdate_ResetStage = (void_fastcallNoParameter)0x7731D0;
	void_fastcallNoParameter _call_ShieldSphereUpdate_Activate = (void_fastcallNoParameter)0x78A050;

	uintptr_t _F_ShieldSphereUpdate_DisplayShieldBar = 0x52808B;
	uintptr_t _call_ShieldSphereUpdate_DisplayShieldBar = 0x523F80;

	void_thiscallHasParameter1 _call_ShieldSphereUpdateModule0C;
	// it has 4 functions.
	uintptr_t _VFT_module_ShieldSphereUpdate_Chargeable[4];

	void __fastcall M_ShieldSphereUpdate_Hook()
	{
		_call_ShieldSphereUpdateModule0C = *(void_thiscallHasParameter1*)(_F_ShieldSphereUpdateVFT_M + 0xC);

		memcpy(_VFT_module_ShieldSphereUpdate_Chargeable, (void*)_F_ShieldSphereUpdateVFT_M, 16U);
		_VFT_module_ShieldSphereUpdate_Chargeable[0] = (uintptr_t)M_ShieldSphereUpdate_Module00_ChargeableASM;
		_VFT_module_ShieldSphereUpdate_Chargeable[3] = (uintptr_t)M_ShieldSphereUpdate_Module0C_Movable;

		hookGameBlock((void*)_F_ShieldSphereUpdateInitialize, (uintptr_t)M_ShieldSphereUpdate_InitializeASM);

		hookGameBlock((void*)_F_ShieldSphereUpdate_DisplayShieldBar, (uintptr_t)M_ShieldSphereUpdate_DisplayShieldBarASM);
		WriteHookToProcess((void*)(_F_ShieldSphereUpdate_DisplayShieldBar + 5), (void*)&nop1, 1U);

		/*uintptr_t newVirtualFunction = (uintptr_t)M_ShieldSphereUpdate_Module0C;
		WriteHookToProcess((void*)(_F_ShieldSphereUpdateVFT_M + 0xC), &newVirtualFunction, 4U);*/
	}

	void __fastcall M_ShieldSphereUpdate_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {
			_F_ShieldSphereUpdateInitialize = 0x762AEA;
			_F_ShieldSphereUpdateVFT_M = 0xC416D0;

			_call_ShieldSphereUpdate_ScaleModel = (void_thiscallHas1Float)0x74E250;
			_call_ShieldSphereUpdate_Reset = (void_fastcallNoParameter)0x77D300;
			_call_ShieldSphereUpdate_ResetStage = (void_fastcallNoParameter)0x7B16B0;
			_call_ShieldSphereUpdate_Activate = (void_fastcallNoParameter)0x7C83F0;

			_F_ShieldSphereUpdate_DisplayShieldBar = 0x5696DB;
			_call_ShieldSphereUpdate_DisplayShieldBar = 0x565660;
		}
	}

	__declspec(naked) void __fastcall M_ShieldSphereUpdate_InitializeASM()
	{
		__asm {
			call _F_Call4EDEC0
			mov edx, edi
			mov ecx, esi
			call M_ShieldSphereUpdate_InitializeCPP
			//
		Addr7246EF:
			pop edi // pModuleData
			mov eax, esi
			pop esi
			add esp, 8
			ret 8
		}
	}

	void __fastcall M_ShieldSphereUpdate_InitializeCPP(pM_ShieldSphereUpdate pIn, pData_ShieldSphereUpdate pData)
	{
		static_assert(offsetof(M_ShieldSphereUpdate_t, bAlwaysDisplayBar) == 0x69);
		pIn->vft_module = _VFT_module_ShieldSphereUpdate_Chargeable;
		pIn->bAlwaysDisplayBar = true;
	}

	__declspec(naked) int __fastcall M_ShieldSphereUpdate_Module00_ChargeableASM(char* pIn)
	{
		__asm{
			mov eax, [ecx - 8] // pGameObject
			add eax, 0x26C
			add ecx, -0x10
			cmp dword ptr[eax], 0
			jne maxDamage
			push ecx
			call M_ShieldSphereUpdate_Module00_ChargeableCPP
			test eax, eax
			pop ecx
			jz checkActived // = 0
			ret
		maxDamage:
			mov eax, [ecx+0x4C]
			mov [ecx+0x48], eax
		checkActived:
			cmp byte ptr[ecx + 0x68], 0
			je toReturn1
			push ecx // save pointer
			call _call_ShieldSphereUpdate_Reset
			xor eax, eax
			pop ecx
			push eax
			call _call_ShieldSphereUpdate_ScaleModel
		toReturn1:
			mov eax, 1
			ret
		}
	}

	int __fastcall M_ShieldSphereUpdate_Module00_ChargeableCPP(pM_ShieldSphereUpdate pIn)
	{
		static_assert(offsetof(M_ShieldSphereUpdate_t, ShieldCurrentDamage) == 0x48);
		static_assert(offsetof(M_ShieldSphereUpdate_t, ShieldMaxDamage) == 0x4C);
		static_assert(offsetof(M_ShieldSphereUpdate_t, ShieldCurrentState) == 0x58);
		static_assert(offsetof(M_ShieldSphereUpdate_t, bIsActivate) == 0x68);
		if (pIn->bIsActivate) {
			float currentDamage = pIn->ShieldCurrentDamage + pIn->ShieldDamageChange + pIn->ShieldDamageChange;
			pIn->ShieldCurrentDamage = currentDamage;

			float maxDamage = pIn->ShieldMaxDamage;
			if (currentDamage >= maxDamage) {
				return 0;
			}

			float scale = (maxDamage - currentDamage) / maxDamage;
			scale *= pIn->ShieldDeltaRadius;

			auto pData = (pData_ShieldSphereUpdate)pIn->pModuleData;
			scale += pData->RadiusMin;
			scale /= pData->RadiusMax;
			_call_ShieldSphereUpdate_ScaleModel(pIn, 0, scale);


			UINT32 currentState = pIn->ShieldCurrentState + 1;
			pIn->ShieldCurrentState = currentState;
			if (currentState >= pIn->ShieldMaxState) {
				pIn->ShieldCurrentState = 0;
				_call_ShieldSphereUpdate_ResetStage(pIn);
			}

			return 1;
		}
		else {
			float currentDamage = pIn->ShieldCurrentDamage - (pIn->ShieldDamageChange * 10);
			pIn->ShieldCurrentDamage = currentDamage;
			if (currentDamage <= 0.0f) {
				_call_ShieldSphereUpdate_Activate(pIn);

				auto pGameObject = (char*)pIn->pGameObject;
				// set to can move
				*(int*)(pGameObject + 0x84) &= 0xFFF7FFFF;
			}

			return 1;
			//return 0x3FFFFFFF;
		}

		//DllCore.dll+8059
		// original
		/*if (pIn->bIsActivate) {
			float currentDamage = pIn->ShieldCurrentDamage + pIn->ShieldDamageChange;
			pIn->ShieldCurrentDamage = currentDamage;

			float maxDamage = pIn->ShieldMaxDamage;
			if (currentDamage >= maxDamage) {
				return 0;
			}

			float scale = (maxDamage - currentDamage) / maxDamage;
			scale *= pIn->ShieldDeltaRadius;

			auto pData = (pData_ShieldSphereUpdate)pIn->pModuleData;
			scale += pData->RadiusMin;
			scale /= pData->RadiusMax;
			_call_ShieldSphereUpdate_ScaleModel(pIn, 0, scale);


			UINT32 currentState = pIn->ShieldCurrentState + 1;
			pIn->ShieldCurrentState = currentState;
			if (currentState >= pIn->ShieldMaxState) {
				pIn->ShieldCurrentState = 0;
				_call_ShieldSphereUpdate_ResetStage(pIn);
			}

			return 1;
		}
		else {
			return 0x3FFFFFFF;
		}*/
	}

	void __fastcall M_ShieldSphereUpdate_Module0C_Movable(char* pIn, int, int a1)
	{
		_call_ShieldSphereUpdateModule0C(pIn, 0, a1);
		pM_ShieldSphereUpdate pM = (pM_ShieldSphereUpdate)(pIn-0x10);
		auto pGameObject = (char*)pM->pGameObject;
		// set to can move
		*(int*)(pGameObject + 0x84) &= 0xFFF7FFFF;
	}

	__declspec(naked) void __fastcall M_ShieldSphereUpdate_DisplayShieldBarASM()
	{
		__asm {
			cmp byte ptr[eax + 0x69], 0
			jne showBar
			cmp byte ptr[eax + 0x68], 0
			je toReturn
		showBar:
			movss xmm1, dword ptr[eax + 0x4C]
			movaps xmm0, xmm1
			subss xmm0, dword ptr[eax + 0x48]
			divss xmm0, xmm1
			// max - current / max
			push ecx
			mov eax, [esi + 0x3A4]
			sub eax, [esi + 0x39C]
			movss dword ptr[esp], xmm0
			mov edx, [esp + 0x10]
			mov ecx, [esp + 0x14]
			push eax
			push ecx
			push edx
			call _call_ShieldSphereUpdate_DisplayShieldBar
			add esp, 0x10
		toReturn:
			pop edi
			pop esi
			ret 8
		}
	}
}
