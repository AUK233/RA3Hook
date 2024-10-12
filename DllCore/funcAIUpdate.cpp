#include "pch.h"

#include "utiliy.h"
#include "utiliy_game.h"
#include "commonStruct.h"
#include "funcAIUpdate.h"
#include "commonData.h"

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
		mov [esi + 0x3C], eax // StateMachinePointer
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

uintptr_t _F_MemoryAllocation = 0x4169A0;
uintptr_t _F_sub601550 = 0x601550;
uintptr_t _F_PushStateMachinePointer = 0x614A70;

__declspec(naked) void __fastcall Initialize_JetAIUpdate_StateMachineASM()
{
	__asm {
		push 0x24
		call _F_MemoryAllocation
		mov ebx, eax
		add esp, 4
		//
		push 0x323A2A24
		push esi
		mov ecx, ebx
		call _F_sub601550
		//
		mov [ebx + 0x20], ebx
		mov dword ptr [ebx], 0xC54AE8
		//
		push edi
		push edi
		push edi
		push ebx
		push 0x51
		mov ecx, esi
		call _F_PushStateMachinePointer
	ofs40FD00:
		pop edi
		pop esi
		pop ebx
		add esp, 0xC
		ret
	}
}

AIModuleStateMachineInitialization AIModuleStateMachine_7F8D60 = (AIModuleStateMachineInitialization)0x7F8D60;
AIModuleStateMachineInitialization AIModuleStateMachine_80EDF0 = (AIModuleStateMachineInitialization)0x80EDF0;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF0A0 = (AIModuleStateMachineInitialization)0x7FF0A0;
AIModuleStateMachineInitialization AIModuleStateMachine_80F5F0 = (AIModuleStateMachineInitialization)0x80F5F0;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF0C0 = (AIModuleStateMachineInitialization)0x7FF0C0;
AIModuleStateMachineInitialization AIModuleStateMachine_80FD20 = (AIModuleStateMachineInitialization)0x80FD20;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF1B0 = (AIModuleStateMachineInitialization)0x7FF1B0;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF370 = (AIModuleStateMachineInitialization)0x7FF370;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF120 = (AIModuleStateMachineInitialization)0x7FF120;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF0E0 = (AIModuleStateMachineInitialization)0x7FF0E0;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF100 = (AIModuleStateMachineInitialization)0x7FF100;
AIModuleStateMachineInitialization AIModuleStateMachine_80EE10 = (AIModuleStateMachineInitialization)0x80EE10;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF2D0 = (AIModuleStateMachineInitialization)0x7FF2D0;
AIModuleStateMachineInitialization AIModuleStateMachine_7FF190 = (AIModuleStateMachineInitialization)0x7FF190;
AIModuleStateMachineInitialization AIModuleStateMachine_80EE30 = (AIModuleStateMachineInitialization)0x80EE30;

AIModuleStateMachine* __fastcall Initialize_AIUpdateStateMachine2A4(AIUpdateModule* pAI)
{
    int StateMachineIndex = pAI->pModuleAsset->StateMachine;

    if (StateMachineIndex < AIStateMachineType_TotalCount) {
        AIModuleStateMachine* pModuleStateMachine = (AIModuleStateMachine*)newMemoryAllocation(sizeof(AIModuleStateMachine));
        UINT32 ofs8 = pAI->ofs008;
        switch (StateMachineIndex)
        {
        case AIStateMachineType_AIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7F8D60(pModuleStateMachine, 0, ofs8, 0x1539E337U);
            break;
        }
        case AIStateMachineType_CombinedHelicopterAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_80EDF0(pModuleStateMachine, 0, ofs8, 0x5263A540U);
            break;
        }
        case AIStateMachineType_HelicopterAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF0A0(pModuleStateMachine, 0, ofs8, 0x9C21578FU);
            break;
        }
        case AIStateMachineType_HordeAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_80F5F0(pModuleStateMachine, 0, ofs8, 0x70F64FFBU);
            break;
        }
        case AIStateMachineType_JetAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF0C0(pModuleStateMachine, 0, ofs8, 0xC2DA23E1U);
            break;
        }
        case AIStateMachineType_OffensiveStructureAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_80FD20(pModuleStateMachine, 0, ofs8, 0xD73FA90U);
            break;
        }
        case AIStateMachineType_StructureAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF1B0(pModuleStateMachine, 0, ofs8, 0x3005FB02U);
            break;
        }
        case AIStateMachineType_UnitAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF370(pModuleStateMachine, 0, ofs8, 0x66746CFBU);
            break;
        }
        case AIStateMachineType_OreNodeStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF120(pModuleStateMachine, 0, ofs8, 0xEFFD9759U);
            break;
        }
        case AIStateMachineType_OreMinerStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF0E0(pModuleStateMachine, 0, ofs8, 0x7FFE8E6EU);
            break;
        }
        case AIStateMachineType_OreMinerOffensiveStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF100(pModuleStateMachine, 0, ofs8, 0xE9450FFDU);
            break;
        }
        case AIStateMachineType_OreRefineryStateMachine: {
            // Now let it not return 0
            //return 0;
            pModuleStateMachine = AIModuleStateMachine_7F8D60(pModuleStateMachine, 0, ofs8, 0x1539E337U);
            break;
        }
        case AIStateMachineType_DisguiseWhenIdleAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_80EE10(pModuleStateMachine, 0, ofs8, 0x78F0EB66U);
            break;
        }
        case AIStateMachineType_TransportHelicopterAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF2D0(pModuleStateMachine, 0, ofs8, 0x165CA278U);
            break;
        }
        case AIStateMachineType_StrafingRunAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_7FF190(pModuleStateMachine, 0, ofs8, 0x1F784020U);
            break;
        }
        case AIStateMachineType_DiveBomberJetAIStateMachine: {
            pModuleStateMachine = AIModuleStateMachine_80EE30(pModuleStateMachine, 0, ofs8, 0xAEA2159AU);
            break;
        }
        default:
            return 0;
        }

        if (pModuleStateMachine) {
            uintptr_t v_table = (uintptr_t)pModuleStateMachine->v_table;
            CallFunctionOnlyECXandNoReturn callFunc = *(CallFunctionOnlyECXandNoReturn*)(v_table + 0x4C);
            callFunc(pModuleStateMachine);
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

void __fastcall HookAIUpdateModule()
{
	//hookGameBlock((void*)0x6D9233, (uintptr_t)InitializeAIUpdateModuleStateMachinePointer);
	hookGameBlock((void*)0x6D8F00, (uintptr_t)Initialize_AIUpdateStateMachine2A4);
	WriteHookToProcess((void*)(0x6D8F00 + 5), (void*)&nop2, 2U);
	hookGameBlock((void*)0x80FD00, (uintptr_t)Initialize_JetAIUpdate_StateMachineASM);
	WriteHookToProcess((void*)(0x80FD00 + 5), (void*)&nop2, 2U);
}
