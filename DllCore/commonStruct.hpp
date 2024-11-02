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

//
typedef struct vector2Dpos_t
{
	float x;
	float y;
} vector2Dpos;

typedef struct vector3Dpos_t
{
	float x;
	float y;
	float z;
} vector3Dpos;
//

typedef struct ruleDataStruct_t
{
	unsigned char pad[0x74];
	// should be int, but now as 4 bytes
	char RandomCrate[4];
} ruleDataStruct;
static_assert(offsetof(ruleDataStruct_t, RandomCrate) == 0x74);

typedef struct ruleDataPointer_t
{
	void* v_table;
	unsigned char pad[8];
	ruleDataStruct_t* ruleData;
} ruleDataPointer;
static_assert(offsetof(ruleDataPointer_t, ruleData) == 0xC);

typedef struct GameObject_t {
	void* v_table;
	unsigned char pad[0x40];
	// it is radian
	float objectAngle;
} GameObject;
static_assert(offsetof(GameObject_t, objectAngle) == 0x44);

typedef struct CommonAssetHashHeader_t {
	UINT32 moduleHash;
	UINT32 nameHash;
} CommonAssetHashHeader;
static_assert(offsetof(CommonAssetHashHeader_t, nameHash) == 4);

typedef struct AIStateMachine_t {
	void* v_table;
	UINT32 count[3];
	UINT32 toZero[2];
	void* pAIModuleStateMachine18;
	unsigned char byteSet[4];
	void* pAIModuleStateMachine20;
} AIStateMachine;
static_assert(offsetof(AIStateMachine_t, pAIModuleStateMachine18) == 0x18);
static_assert(offsetof(AIStateMachine_t, pAIModuleStateMachine20) == 0x20);

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
} AIModuleStateMachine;
static_assert(offsetof(AIModuleStateMachine_t, pStateMachine) == 4);
static_assert(offsetof(AIModuleStateMachine_t, CombinedGoal) == 0xA0);

typedef void(__fastcall* GetCurrentStateMachine_t)(CommonStateMachine_t* pCommonStateMachine, void* useless,
												StateMachinePointer_t** ppStateMachinePointer, UINT32* pState);
typedef void(__fastcall* CallFunctionOnlyECXandNoReturn)(void* pThis);
typedef void(__fastcall* PushStateMachinePointer_t)(AIModuleStateMachine* pMSM, void* useless,
													UINT32 state, AIStateMachine* pSM,
													int arg4, int arg5, void* arg6);
