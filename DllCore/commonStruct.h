#pragma once
#include <cstddef>

typedef struct inputSettingINFOtype
{
	unsigned char cpuLimit;
	unsigned char CheckBloom;
	unsigned char setDebug;
	unsigned char UPto60;
	unsigned char LocalFlag;
	unsigned char align[3];
} inputSettingINFO;

typedef struct CommonAssetHashHeader_t {
	UINT32 moduleHash;
	UINT32 nameHash;
} CommonAssetHashHeader;
static_assert(offsetof(CommonAssetHashHeader_t, nameHash) == 4);

typedef struct AIStateMachine_t {
	void* v_table;
	unsigned char pad[0x1C];
	void* pAIModuleStateMachine;
} AIStateMachine;
static_assert(offsetof(AIStateMachine_t, pAIModuleStateMachine) == 0x20);

typedef struct StateMachinePointer_t {
	StateMachinePointer_t* above;
	StateMachinePointer_t* below;
	unsigned char pad[8];
	UINT32 state;
	AIStateMachine_t* pStateMachine;
} StateMachinePointer;
static_assert(offsetof(StateMachinePointer_t, pStateMachine) == 0x14);

typedef struct CommonStateMachine_t {
	unsigned char pad1[4];
	void* defaultStateMachine;
	unsigned char pad2[4];
	StateMachinePointer_t* pStateMachinePointer;
} CommonStateMachine;
static_assert(offsetof(CommonStateMachine_t, pStateMachinePointer) == 0xC);

typedef struct AIModuleStateMachine_t {
	void* v_table;
	AIStateMachine_t* pStateMachine;
	CommonStateMachine_t csm;
	unsigned char pad1111[0x88];
	// Original size end
	void* CombinedGoal;
	int SpecialSet;
} AIModuleStateMachine;
static_assert(offsetof(AIModuleStateMachine_t, pStateMachine) == 4);
static_assert(offsetof(AIModuleStateMachine_t, CombinedGoal) == 0xA0);

typedef struct AIUpdateModuleAsset_t {
	CommonAssetHashHeader_t hash;
	unsigned char pad[0x34];
	int StateMachine;
} AIUpdateModuleAsset;
static_assert(offsetof(AIUpdateModuleAsset_t, StateMachine) == 0x3C);

typedef struct AIUpdateModule_t {
	void* v_table;
	AIUpdateModuleAsset_t* pModuleAsset;
	UINT32 ofs008;
	unsigned char pad[0x30];
	AIModuleStateMachine_t* pModuleStateMachine;
} AIUpdateModule;
static_assert(offsetof(AIUpdateModule_t, ofs008) == 8);
static_assert(offsetof(AIUpdateModule_t, pModuleStateMachine) == 0x3C);

typedef void(__fastcall* GetCurrentStateMachine_t)(CommonStateMachine_t* pCommonStateMachine, void* useless,
                                                   StateMachinePointer_t** ppStateMachinePointer, UINT32* pState);
typedef AIModuleStateMachine_t* (__fastcall* AIModuleStateMachineInitialization)(AIModuleStateMachine_t* pCommonStateMachine, void* useless, UINT32 ofs008, UINT32 p4);
typedef void(__fastcall* CallFunctionOnlyECXandNoReturn)(void* pThis);
