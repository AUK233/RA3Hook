#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct C_GameObject_t {
	void* vf_table;
	char pad04[0x40];
	float Orientation; // radian
	char pad48[0x31C];
	void* pTiberiumCrystalBehavior;
	char pad368[0xC];
	void* pAIUpdateModule;
	char pad378[0xA0];
	void* pRelevantPlayer;
}*pC_GameObject;
static_assert(offsetof(C_GameObject_t, Orientation) == 0x44);
static_assert(offsetof(C_GameObject_t, pTiberiumCrystalBehavior) == 0x364);
static_assert(offsetof(C_GameObject_t, pAIUpdateModule) == 0x374);
static_assert(offsetof(C_GameObject_t, pRelevantPlayer) == 0x418);
