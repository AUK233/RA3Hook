#include "pch.h"
#include <format>

#include "utiliy.h"
#include "utiliy_game.h"
#include "commonStruct.hpp"
#include "funcAIUpdate.h"
#include "funcAIUpdate_Address.hpp"
#include "commonData.hpp"

namespace RA3::AI {

	GetCurrentStateMachine_t _GetCurrentStateMachine = (GetCurrentStateMachine_t)0xA87DE0;

	void __fastcall SetNewAIUpdateModuleStateMachinePointer(AIUpdateModule* pAI)
	{
		// ModuleTag_JetAIUpdateForCombine
		if (pAI->pModuleAsset->hash.nameHash == 3661739227U) {
			AIModuleStateMachine* pModuleStateMachine = pAI->pModuleStateMachine;
			// UnitAICombinedUnitState
			StateMachinePointer* pStateMachinePointer51;
			UINT32 state = 0x51;
			_GetCurrentStateMachine(&pModuleStateMachine->csm, 0, &pStateMachinePointer51, &state);

			StateMachinePointer* defaultStateMachine = (StateMachinePointer*)&pModuleStateMachine->csm.defaultStateMachine;
			if (pStateMachinePointer51 != defaultStateMachine) {
				//MessageBox(NULL, L"It is ModuleTag_JetAIUpdateForCombine!", L"test", MB_OK);
				// AIGuardState
				StateMachinePointer* pStateMachinePointer10;
				state = 0x10;
				_GetCurrentStateMachine(&pModuleStateMachine->csm, 0, &pStateMachinePointer10, &state);

				if (pStateMachinePointer10 != defaultStateMachine) {
					AIStateMachine* pStateMachine = pStateMachinePointer51->pStateMachine;
					pStateMachinePointer51->pStateMachine = pStateMachinePointer10->pStateMachine;
					pStateMachinePointer10->pStateMachine = pStateMachine;
				}
			}
			// 
		}
	}

	__declspec(naked) void __fastcall InitializeAIUpdateModuleStateMachinePointer()
	{
		__asm {
			mov[esi + 0x3C], eax // StateMachinePointer
			mov ecx, esi
			call SetNewAIUpdateModuleStateMachinePointer
			//ofs2D9236:
			mov eax, [esi + 0x3C]
			mov edx, [eax]
			mov ecx, eax
			mov eax, [edx + 0x18]
			pop esi
			jmp eax
		}
	}


	static_assert(offsetof(AIModuleStateMachine, CombinedGoal) == 0xA0);
	__declspec(naked) void __fastcall PushStateMachineIndex0x21ASM()
	{
		__asm {
			mov eax, [ecx + 0xA0]
			mov edx, _ret_PushStateMachineIndex0x21
			test eax, eax
			jne Call51
			mov[esi + 0x50], ebx
			mov eax, [ecx]
			push 0x21
			push edx // return address
			jmp dword ptr[eax + 0x20]
		Call51:
			push ebx
			push eax
			mov ecx, esi
			push edx // return address
			jmp CallStateMachineIndex0x51ASM
		}
	}

	static_assert(offsetof(JetAIUpdateModuleAsset_t, ParkingOffset) == 0x54);
	void __fastcall JetAIUpdateCirclesForAttackNewPosition(AIUpdateModule* pAI, vector3Dpos* pPos)
	{
		//is rectangular movement
		float radian = atan2(pPos[1].y, pPos[1].x);
		//float radian = pAI->pGameObject->objectAngle;
		//radian += 0.314f * getRadomFloatValue(0.0f, 1.0f);
		
		//JetAIUpdateModuleAsset* pJetAI = (JetAIUpdateModuleAsset*)pAI->pModuleAsset;
		//float ringRadius = pJetAI->ParkingOffset;
		float ringRadius = pAI->pModuleStateMachine->CircleRadius;
		if (ringRadius > 0) {
			// move clockwise
			radian += 1.57f*1.5f;
		}
		else {
			// move anticlockwise
			radian -= 1.57f * 1.5f;
			ringRadius *= -1.0f;
		}
		// pPos[0] is attacked position
		// pPos[1] is will move to position
		// pPos[2] is self position
		pPos[1].x = (ringRadius * cos(radian)) + pPos[0].x;
		pPos[1].y = (ringRadius * sin(radian)) + pPos[0].y;
	}


	float _float2500 = 2500.0f;
	__declspec(naked) void __fastcall JetAIUpdateCirclesForAttackCheckApproach()
	{
		__asm {
#include "ASM/JetAIUpdateCirclesForAttackCheckApproach.asm"
		}
	}

	__declspec(naked) void __fastcall CallStateMachineIndex0x51ASM(AIUpdateModule* pAI, void* useless, int arg2, void* arg3)
	{
		__asm {
			push esi
			mov esi, ecx
			mov ecx, [esi + 0x3C]
			mov eax, [ecx]
			call dword ptr[eax + 0x38]
			//
			mov ecx, [esi + 0x3C]
			mov eax, [ecx]
			mov edx, [esp + 8]
			mov [ecx + 0xA0], edx // here add
			push edx
			call dword ptr[eax + 0x3C]
			//
			mov edx, [esp + 0xC]
			mov[esi + 0x50], edx
			mov ecx, [esi + 0x3C]
			mov eax, [ecx]
			push 0x51
			call dword ptr[eax + 0x20]
			pop esi
			ret 8
		}
	}

	void __fastcall Initialize_CommonStateMachine601550(AIStateMachine* pSM, AIModuleStateMachine* pMSM)
	{
		pSM->v_table = (void*)_vt_FailureStateC0D0C8;
		pSM->count[0] = 99999;
		pSM->count[1] = 99999;
		pSM->count[2] = 99999;
		pSM->toZero[0] = 0;
		pSM->toZero[1] = 0;
		pSM->pAIModuleStateMachine18 = pMSM;
		pSM->byteSet[0] = 0;
	}

	void __fastcall Initialize_CommonStateMachineNew(AIStateMachine* pSM, AIModuleStateMachine* pMSM, void* v_table)
	{
		pSM->v_table = v_table;
		pSM->count[0] = 99999;
		pSM->count[1] = 99999;
		pSM->count[2] = 99999;
		pSM->toZero[0] = 0;
		pSM->toZero[1] = 0;
		pSM->pAIModuleStateMachine18 = pMSM;
		pSM->byteSet[0] = 0;
	}

	// add UnitAICombinedUnitState support
	void __fastcall Initialize_JetAIUpdate_StateMachine4C(AIModuleStateMachine* pMSM)
	{
		AIStateMachine* pStateMachine = (AIStateMachine*)newMemoryAllocation(0x24U);
		// 0x323A2A24, UnitAICombinedUnitState
		Initialize_CommonStateMachineNew(pStateMachine, pMSM, (void*)_vt_UnitAICombinedUnitStateC54AE8);
		pStateMachine->pAIModuleStateMachine20 = 0;
		PushStateMachinePointer_614A70(pMSM, 0, 0x51, pStateMachine, 0, 0, 0);
	}

	void __fastcall Initialize_AIModuleStateMachineNewData(AIModuleStateMachine* pMSM)
	{
		//pMSM->CombinedGoal = 0;
		//*(int*)&pMSM->AttackWithCircle = 1;
		ZeroMemory(&pMSM->CombinedGoal, 12U);
	}


	AIModuleStateMachine* __fastcall Initialize_AIUpdateStateMachine2A4(AIUpdateModule* pAI)
	{
		// return value written to pAI->pModuleStateMachine
		int StateMachineIndex = pAI->pModuleAsset->StateMachine;

		if (StateMachineIndex < AIStateMachineType_TotalCount) {
			AIModuleStateMachine* pModuleStateMachine = (AIModuleStateMachine*)newMemoryAllocation(sizeof(AIModuleStateMachine));
			GameObject* pGameObject = pAI->pGameObject;
			switch (StateMachineIndex)
			{
			case AIStateMachineType_AIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7F8D60(pModuleStateMachine, 0, pGameObject, 0x1539E337U);
				break;
			}
			case AIStateMachineType_CombinedHelicopterAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_80EDF0(pModuleStateMachine, 0, pGameObject, 0x5263A540U);
				break;
			}
			case AIStateMachineType_HelicopterAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF0A0(pModuleStateMachine, 0, pGameObject, 0x9C21578FU);
				break;
			}
			case AIStateMachineType_HordeAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_80F5F0(pModuleStateMachine, 0, pGameObject, 0x70F64FFBU);
				break;
			}
			case AIStateMachineType_JetAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF0C0(pModuleStateMachine, 0, pGameObject, 0xC2DA23E1U);
				break;
			}
			case AIStateMachineType_OffensiveStructureAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_80FD20(pModuleStateMachine, 0, pGameObject, 0xD73FA90U);
				break;
			}
			case AIStateMachineType_StructureAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF1B0(pModuleStateMachine, 0, pGameObject, 0x3005FB02U);
				break;
			}
			case AIStateMachineType_UnitAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF370(pModuleStateMachine, 0, pGameObject, 0x66746CFBU);
				break;
			}
			case AIStateMachineType_OreNodeStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF120(pModuleStateMachine, 0, pGameObject, 0xEFFD9759U);
				break;
			}
			case AIStateMachineType_OreMinerStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF0E0(pModuleStateMachine, 0, pGameObject, 0x7FFE8E6EU);
				break;
			}
			case AIStateMachineType_OreMinerOffensiveStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF100(pModuleStateMachine, 0, pGameObject, 0xE9450FFDU);
				break;
			}
			case AIStateMachineType_OreRefineryStateMachine: {
				if (pAI->pModuleAsset->hash.nameHash == 3661739227U) {
					// ModuleTag_JetAIUpdateForCombine
					pModuleStateMachine = AIModuleStateMachine_7FF0C0(pModuleStateMachine, 0, pGameObject, 0xC2DA23E1U);
					uintptr_t v_table = (uintptr_t)pModuleStateMachine->v_table;
					CallFunctionOnlyECXandNoReturn callFunc = *(CallFunctionOnlyECXandNoReturn*)(v_table + 0x4C);
					callFunc(pModuleStateMachine);
					
					Initialize_AIModuleStateMachineNewData(pModuleStateMachine);
					Initialize_JetAIUpdate_StateMachine4C(pModuleStateMachine);
					return pModuleStateMachine;
				} else if (pAI->pModuleAsset->hash.nameHash == 3494529041U) {
					// ModuleTag_JetAIUpdateForGunship
					pModuleStateMachine = AIModuleStateMachine_7FF0C0(pModuleStateMachine, 0, pGameObject, 0xC2DA23E1U);
					uintptr_t v_table = (uintptr_t)pModuleStateMachine->v_table;
					CallFunctionOnlyECXandNoReturn callFunc = *(CallFunctionOnlyECXandNoReturn*)(v_table + 0x4C);
					callFunc(pModuleStateMachine);

					Initialize_AIModuleStateMachineNewData(pModuleStateMachine);
					// used for circle
					pModuleStateMachine->AttackWithCircle = 1;
					PJetAIUpdateModuleAsset pJetAI = (PJetAIUpdateModuleAsset)pAI->pModuleAsset;
					if (*(int*)&pJetAI->SpawnOffsetRadius == 0) {
						pModuleStateMachine->CircleRadius = 277.0f;
					}
					else {
						pModuleStateMachine->CircleRadius = pJetAI->SpawnOffsetRadius;
					}

					return pModuleStateMachine;
				}
				// default case
				return 0;
			}
			case AIStateMachineType_DisguiseWhenIdleAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_80EE10(pModuleStateMachine, 0, pGameObject, 0x78F0EB66U);
				break;
			}
			case AIStateMachineType_TransportHelicopterAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF2D0(pModuleStateMachine, 0, pGameObject, 0x165CA278U);
				break;
			}
			case AIStateMachineType_StrafingRunAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_7FF190(pModuleStateMachine, 0, pGameObject, 0x1F784020U);
				break;
			}
			case AIStateMachineType_DiveBomberJetAIStateMachine: {
				pModuleStateMachine = AIModuleStateMachine_80EE30(pModuleStateMachine, 0, pGameObject, 0xAEA2159AU);
				break;
			}
			default:
				return 0;
			}

			if (pModuleStateMachine) {
				uintptr_t v_table = (uintptr_t)pModuleStateMachine->v_table;
				CallFunctionOnlyECXandNoReturn callFunc = *(CallFunctionOnlyECXandNoReturn*)(v_table + 0x4C);
				callFunc(pModuleStateMachine);

				Initialize_AIModuleStateMachineNewData(pModuleStateMachine);
				return pModuleStateMachine;
			}
			else {
				return 0;
			}
			//
		}
		else {
			return 0;
		}

	}

	void __fastcall InitializeHookAIUpdateModuleOrigin(uintptr_t hmodEXE)
	{
		_F_AIUpdateStateMachine2A4 = hmodEXE + 0x317490;
		_F_CallStateMachineIndex0x51 = hmodEXE + 0x32E3C0;
		_F_PushStateMachineIndex0x21 = hmodEXE + 0x3AD2F9;
		_ret_PushStateMachineIndex0x21 = hmodEXE + 0x3AD2F9 + 0xC;
		_F_CirclesForAttack787119 = hmodEXE + 0x3C54B9;
		_ret_CirclesForAttack787138 = hmodEXE + 0x3C54D2;
		_F_ofs8E8F60 = 13558000;
		_ret_CirclesForAttack787282 = hmodEXE + 0x3C5622;

		_vt_FailureStateC0D0C8 = hmodEXE + 0x814600 - 0xA0;
		_vt_UnitAICombinedUnitStateC54AE8 = hmodEXE + 0x85BAD4 - 0x54;
		PushStateMachinePointer_614A70 = (PushStateMachinePointer_t)(hmodEXE + 0x253950);

		AIModuleStateMachine_7F8D60 = (AIModuleStateMachineInitialization)0x836FD0;
		AIModuleStateMachine_80EDF0 = (AIModuleStateMachineInitialization)0x84CFA0;
		AIModuleStateMachine_7FF0A0 = (AIModuleStateMachineInitialization)0x83D2A0;
		AIModuleStateMachine_80F5F0 = (AIModuleStateMachineInitialization)0x84D7A0;
		AIModuleStateMachine_7FF0C0 = (AIModuleStateMachineInitialization)0x83D2C0;
		AIModuleStateMachine_80FD20 = (AIModuleStateMachineInitialization)0x84DED0;
		AIModuleStateMachine_7FF1B0 = (AIModuleStateMachineInitialization)0x83D3B0;
		AIModuleStateMachine_7FF370 = (AIModuleStateMachineInitialization)0x83D570;
		AIModuleStateMachine_7FF120 = (AIModuleStateMachineInitialization)0x83D370;
		AIModuleStateMachine_7FF0E0 = (AIModuleStateMachineInitialization)0x83D2E0;
		AIModuleStateMachine_7FF100 = (AIModuleStateMachineInitialization)0x83D350;
		AIModuleStateMachine_80EE10 = (AIModuleStateMachineInitialization)0x84CFC0;
		AIModuleStateMachine_7FF2D0 = (AIModuleStateMachineInitialization)0x83D4D0;
		AIModuleStateMachine_7FF190 = (AIModuleStateMachineInitialization)0x83D390;
		AIModuleStateMachine_80EE30 = (AIModuleStateMachineInitialization)0x84CFE0;


		_F_ofs43CBB8 = hmodEXE + 0x47AC68;
	}

	void __fastcall HookAIUpdateModule()
	{
		//hookGameBlock((void*)0x6D9233, (uintptr_t)InitializeAIUpdateModuleStateMachinePointer);

		// initialization series
		hookGameBlock((void*)_F_AIUpdateStateMachine2A4, (uintptr_t)Initialize_AIUpdateStateMachine2A4);
		WriteHookToProcess((void*)(_F_AIUpdateStateMachine2A4 + 5), (void*)&nop2, 2U);
		hookGameBlock((void*)_F_CallStateMachineIndex0x51, (uintptr_t)CallStateMachineIndex0x51ASM);
		WriteHookToProcess((void*)(_F_CallStateMachineIndex0x51 + 5), (void*)&nop1, 1U);

		// execution series
		hookGameBlock((void*)_F_PushStateMachineIndex0x21, (uintptr_t)PushStateMachineIndex0x21ASM);
		hookGameBlock((void*)_F_CirclesForAttack787119, (uintptr_t)JetAIUpdateCirclesForAttackCheckApproach);
		WriteHookToProcess((void*)(_F_CirclesForAttack787119 + 5), (void*)&nop2, 2U);

		//
		//test eax, eax
		//je ra3_1.12.83CBC2
		//cmp dword ptr[eax + 10], ecx
		//setne al
		//ret
		BYTE playerAC[]= {
			0x85, 0xC0,
			0x74, 0x06,
			0x39, 0x48, 0x10,
			0x0F, 0x95, 0xC0,
			0xC3
		};
		//WriteHookToProcess((void*)_F_ofs43CBB8, (void*)&playerAC, 11U);
		//WriteHookToProcess((void*)(_F_ofs43CBB8+0x20), (void*)&playerAC, 11U);
	}

}
