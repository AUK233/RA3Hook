#pragma once

namespace RA3::AI{

typedef enum AIStateMachineType {
	AIStateMachineType_AIStateMachine,
	AIStateMachineType_CombinedHelicopterAIStateMachine,
	AIStateMachineType_HelicopterAIStateMachine,
	AIStateMachineType_HordeAIStateMachine,
	AIStateMachineType_JetAIStateMachine,
	AIStateMachineType_OffensiveStructureAIStateMachine,
	AIStateMachineType_StructureAIStateMachine,
	AIStateMachineType_UnitAIStateMachine,
	AIStateMachineType_OreNodeStateMachine,
	AIStateMachineType_OreMinerStateMachine,
	AIStateMachineType_OreMinerOffensiveStateMachine,
	AIStateMachineType_OreRefineryStateMachine,
	AIStateMachineType_DisguiseWhenIdleAIStateMachine,
	AIStateMachineType_TransportHelicopterAIStateMachine,
	AIStateMachineType_StrafingRunAIStateMachine,
	AIStateMachineType_DiveBomberJetAIStateMachine,
	AIStateMachineType_TotalCount
};

typedef struct AIUpdateModuleAsset_t {
	CommonAssetHashHeader_t hash;
	unsigned char pad[0x34];
	int StateMachine;
} AIUpdateModuleAsset;
static_assert(offsetof(AIUpdateModuleAsset_t, StateMachine) == 0x3C);

typedef struct JetAIUpdateModuleAsset_t {
	AIUpdateModuleAsset_t common;
	unsigned char pad1[0x14];
	float ParkingOffset;
	unsigned char pad2[0xB];
	char CirclesForAttack;
} JetAIUpdateModuleAsset;
static_assert(offsetof(JetAIUpdateModuleAsset_t, CirclesForAttack) == 0x63);

typedef struct AIUpdateModule_t {
	void* v_table;
	AIUpdateModuleAsset_t* pModuleAsset;
	GameObject* pGameObject;
	unsigned char pad[0x30];
	AIModuleStateMachine_t* pModuleStateMachine;
} AIUpdateModule;
static_assert(offsetof(AIUpdateModule_t, pGameObject) == 8);
static_assert(offsetof(AIUpdateModule_t, pModuleStateMachine) == 0x3C);

//
void __fastcall SetNewAIUpdateModuleStateMachinePointer(AIUpdateModule* pAI);
void __fastcall InitializeAIUpdateModuleStateMachinePointer();
//

void __fastcall PushStateMachineIndex0x21ASM();
//
void __fastcall JetAIUpdateCirclesForAttackNewPosition(AIUpdateModule* pAI, vector3Dpos* pPos);
//
void __fastcall JetAIUpdateCirclesForAttackCheckApproach();

//
void __fastcall CallStateMachineIndex0x51ASM(AIUpdateModule* pAI, void* useless, int arg2, void* pTarget);
//
void __fastcall Initialize_CommonStateMachine601550(AIStateMachine* pSM, AIModuleStateMachine* pMSM);
void __fastcall Initialize_CommonStateMachineNew(AIStateMachine* pSM, AIModuleStateMachine* pMSM, void* v_table);
// add UnitAICombinedUnitState support
void __fastcall Initialize_JetAIUpdate_StateMachine4C(AIModuleStateMachine* pMSM);
//
void __fastcall Initialize_AIModuleStateMachineNewData(AIModuleStateMachine* pMSM);
//
AIModuleStateMachine* __fastcall Initialize_AIUpdateStateMachine2A4(AIUpdateModule* pAI);

void __fastcall InitializeHookAIUpdateModuleOrigin(uintptr_t hmodEXE);
void __fastcall HookAIUpdateModule();

//
typedef AIModuleStateMachine_t* (__fastcall* AIModuleStateMachineInitialization)(AIModuleStateMachine_t* pCommonStateMachine, void* useless,
																				GameObject* pGameObject, UINT32 arg3);
}
