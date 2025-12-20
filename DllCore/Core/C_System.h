#pragma once

typedef float(__fastcall* CallFunc_W3DTerrainLogicVFT18)(void* pIn, void* callFunc, float posX, float posY, float* unk);
typedef float(__fastcall* CallFunc_W3DTerrainLogicVFT1C)(void* pIn, void* callFunc, float* pos, float* unk);

typedef struct G_W3DTerrainLogic_VFT_t {
	char pad[0x18];
	CallFunc_W3DTerrainLogicVFT18 func18;
	CallFunc_W3DTerrainLogicVFT1C func1C;
} *pG_W3DTerrainLogic_VFT;
#if 1
	static_assert(offsetof(G_W3DTerrainLogic_VFT_t, func18) == 0x18);
	static_assert(offsetof(G_W3DTerrainLogic_VFT_t, func1C) == 0x1C);
#endif

typedef struct G_W3DTerrainLogic_t {
	pG_W3DTerrainLogic_VFT vf_table;
}*pG_W3DTerrainLogic;

typedef bool(__fastcall* CallFunc_CheckPositionInWater)(void* pIn, void* callFunc, float posX, float posY, float* outWaterHeight, float* outLandHeight);
typedef int(__fastcall* CallFunc_CheckPositionIsHitTerrain)(void* pIn, void* callFunc, int a1, float* pos);

namespace RA3::Core {
	void __fastcall C_System_Hook();
	void __fastcall C_System_Initialize(uintptr_t hmodEXE, int isNewSteam);

	// 0 is in land, 1 is in water
	bool __fastcall C_System_CheckPositionInWater(void* uselessECX, void* uselessEDX, float posX, float posY, float* outWaterHeight, float* outLandHeight);
	// 1 is not hit water. 0x10 is not hit land?
	int __fastcall C_System_CheckPositionIsHitTerrain(void* uselessECX, void* uselessEDX, int a1, float* pos);

	pG_W3DTerrainLogic __fastcall C_System_GetW3DTerrainLogic();
}