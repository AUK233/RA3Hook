#pragma once
#include "../Base/CommonStructure.hpp"
#include "../Base/SSE.hpp"
#include "C_ThingTemplate.h"

typedef void(__fastcall* CallFunc_GameObjectVFT14)(void* pIn, void* callFunc, Matrix3D_t* mat, float* pos, float orientation);

typedef struct C_GameObject_VFT_t {
	char pad[0x14];
	CallFunc_GameObjectVFT14 func14;
} *pC_GameObject_VFT;
#if 1
	static_assert(offsetof(C_GameObject_VFT_t, func14) == 0x14);
#endif

// old is 0x4FC
typedef struct C_GameObject_t {
	pC_GameObject_VFT vf_table;
	void* pThingTemplate;
	Matrix3D_t transform;
	float Position[3];
	float Orientation; // radian
	char pad48[0x15C];
	float previous_Orientation, previous_Position[3];
	Matrix3D_t previous_transform;
	bool bNeedsTransformUpdate; // check saves current transformation
	char pad1E5[0x17F];
	void* pTiberiumCrystalBehavior;
	char pad368[0xC];
	void* pAIUpdateModule;
	char pad378[0xA0];
	void* pRelevantPlayer;
	char pad41C[0xE0];
	// new
	int LuaAdditionalData;
}*pC_GameObject;
#if 1
	static_assert(offsetof(C_GameObject_t, Position) == 0x38);
	static_assert(offsetof(C_GameObject_t, Orientation) == 0x44);
	static_assert(offsetof(C_GameObject_t, previous_Orientation) == 0x1A4);
	static_assert(offsetof(C_GameObject_t, previous_Position) == 0x1A8);
	static_assert(offsetof(C_GameObject_t, previous_transform) == 0x1B4);
	static_assert(offsetof(C_GameObject_t, bNeedsTransformUpdate) == 0x1E4);
	static_assert(offsetof(C_GameObject_t, pTiberiumCrystalBehavior) == 0x364);
	static_assert(offsetof(C_GameObject_t, pAIUpdateModule) == 0x374);
	static_assert(offsetof(C_GameObject_t, pRelevantPlayer) == 0x418);
	static_assert(offsetof(C_GameObject_t, LuaAdditionalData) == 0x4FC);
#endif

namespace RA3::Core {

	void __fastcall C_GameObject_Hook();
	void __fastcall C_GameObject_Initialize(uintptr_t hmodEXE, int isNewSteam);


	void __fastcall C_GameObject_InitializeASM();


	void __fastcall C_GameObject_SetGameObjectPosition7E3D1B_ASM();
	void __fastcall C_GameObject_UpdateGameObjectTransformCPP(pC_GameObject pIn, int useless, Matrix3D_t* newMat);

	float __fastcall C_GameObject_atanASM(Matrix3D_t* mat);

	int __fastcall C_GameObject_SetFireAllowedStatusCPP(pC_ThingTemplate pIn);
}
