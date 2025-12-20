#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_System.h"

namespace RA3::Core {

	uintptr_t _F_sysCE8F60 = 0xCE8F60;
	uintptr_t _F_sysCE8F68 = 0xCE8F68;
	uintptr_t _call_CheckPositionInWater = 0x85A780;
	uintptr_t _call_CheckPositionIsHitTerrain = 0x85F8C0;

	void __fastcall C_System_Hook()
	{
	}

	void __fastcall C_System_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_sysCE8F60 = 0xCEE0F0;
			_F_sysCE8F68 = 0xCEE0F8;
			_call_CheckPositionInWater = 0x8985F0;
			_call_CheckPositionIsHitTerrain = 0x89D5B0;
		}
	}

	bool __fastcall C_System_CheckPositionInWater(void* uselessECX, void* uselessEDX, float posX, float posY, float* outWaterHeight, float* outLandHeight)
	{
		auto pECX = *(uintptr_t*)_F_sysCE8F68;
		auto fnCheck = (CallFunc_CheckPositionInWater)_call_CheckPositionInWater;
		return fnCheck((void*)pECX, fnCheck, posX, posY, outWaterHeight, outLandHeight);
	}

	int __fastcall C_System_CheckPositionIsHitTerrain(void* uselessECX, void* uselessEDX, int a1, float* pos)
	{
		auto pECX = *(uintptr_t*)_F_sysCE8F60;
		auto fnCheck = (CallFunc_CheckPositionIsHitTerrain)_call_CheckPositionIsHitTerrain;
		return fnCheck((void*)pECX, fnCheck, a1, pos);
	}

	pG_W3DTerrainLogic __fastcall C_System_GetW3DTerrainLogic()
	{
		return *(pG_W3DTerrainLogic*)_F_sysCE8F60;
	}

// end namespace RA3::Core
}