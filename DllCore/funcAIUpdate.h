#pragma once

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

void __fastcall SetNewAIUpdateModuleStateMachinePointer(AIUpdateModule* pAI);
void __fastcall InitializeAIUpdateModuleStateMachinePointer();

void __fastcall Initialize_JetAIUpdate_StateMachineASM();

AIModuleStateMachine* __fastcall Initialize_AIUpdateStateMachine2A4(AIUpdateModule* pAI);

void __fastcall HookAIUpdateModule();
