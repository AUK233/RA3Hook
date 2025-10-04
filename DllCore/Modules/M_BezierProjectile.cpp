#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "M_BezierProjectile.h"

namespace RA3::Module {

	// mov dword ptr [esi+10], ra3_1.12.C376A0
	uintptr_t _F_BezierProjectileInitialize = 0x74B3CB + 3;
	// is p+0x10
	uintptr_t _F_BezierProjectileVFT_M = 0xC376A0;

	int_fastcallNoParameter _call_BezierProjectileModule00;
	// it has 4 functions.
	uintptr_t _VFT_module_BezierProjectileNew[4];

	void __fastcall M_BezierProjectile_Hook()
	{
		_call_BezierProjectileModule00 = *(int_fastcallNoParameter*)(_F_BezierProjectileVFT_M);
		memcpy(_VFT_module_BezierProjectileNew, (void*)_F_BezierProjectileVFT_M, 16U);
		_VFT_module_BezierProjectileNew[0] = (uintptr_t)M_BezierProjectile_Module00CPP;

		// movement that always skips the first frame.
		uintptr_t newVirtualFunction = (uintptr_t)&_VFT_module_BezierProjectileNew[0];
		WriteHookToProcess((void*)_F_BezierProjectileInitialize, &newVirtualFunction, 4U);
	}

	void __fastcall M_BezierProjectile_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {
			_F_BezierProjectileInitialize = 0x7899EB + 3;
			_F_BezierProjectileVFT_M = 0xC3E828;
		}
	}

	int __fastcall M_BezierProjectile_Module00CPP(char* pIn)
	{
		// movement that always skips the first frame.
		pM_BezierProjectile pThis = (pM_BezierProjectile)(pIn - 0x10);
		pThis->vft_module = (void*)_F_BezierProjectileVFT_M;
		return 1;

		// unused code, just for structure check.
		//auto pData = (pData_BezierProjectile)pThis->pModuleData;
		static_assert(offsetof(Data_BezierProjectile_t, Acceleration) == 0xF0);
		static_assert(offsetof(M_BezierProjectile_t, ammospeed) == 0x7C);
		static_assert(offsetof(M_BezierProjectile_t, CurrentSpeed) == 0xC8);
	}

	// end namespace RA3::Module
}