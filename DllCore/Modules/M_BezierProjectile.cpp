#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Core/C_System.h"
#include "../Core/C_GameObject.h"
#include "M_BezierProjectile.h"

namespace RA3::Core {
	extern uintptr_t _F_sysCE8F60;
	extern uintptr_t _F_sysCE8F68;
	extern uintptr_t _call_CheckPositionInWater;
}

namespace RA3::Module {

	uintptr_t _F_BezierProjectileInitialize = 0x74B51C;
	uintptr_t _F_BezierProjectile_CheckProjectilePosition = 0x6E0F72;
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
		hookGameBlock((void*)_F_BezierProjectileInitialize, (uintptr_t)M_BezierProjectile_InitializeASM);
		WriteHookToProcess((void*)(_F_BezierProjectileInitialize + 5), (void*)&nop2, 2U);

		hookGameBlock((void*)_F_BezierProjectile_CheckProjectilePosition, (uintptr_t)M_BezierProjectile_CheckProjectilePositionASM);
		WriteHookToProcess((void*)(_F_BezierProjectile_CheckProjectilePosition + 5), (void*)&nop2, 2U);
	}

	void __fastcall M_BezierProjectile_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {
			_F_BezierProjectileInitialize = 0x789B3C;
			_F_BezierProjectile_CheckProjectilePosition = 0x71F3B2;
			_F_BezierProjectileVFT_M = 0xC3E828;
		}
	}

	__declspec(naked) void __fastcall M_BezierProjectile_InitializeASM()
	{
		__asm {
			mov edx, [esi + 4] // get module data pointer
			cmp dword ptr[edx + 0xF0], 0 // check Acceleration
			jne setWithGroundCollision
			// set new vft
			lea eax, _VFT_module_BezierProjectileNew
			mov[esi + 0x10], eax
		}

	setWithGroundCollision:
		__asm {
			cmp dword ptr[edx + 4], 3873806428U // ModuleTag_BezierProjectileGroundHit
			sete al
			mov[esi + 0xD1], al
		}

	ofs34B51C:
		__asm {
			pop edi
			mov eax, esi
			pop esi
			ret 8
		}
	}

	int __fastcall M_BezierProjectile_Module00CPP(char* pIn)
	{
		// movement that always skips the first frame.
		pM_BezierProjectile pThis = (pM_BezierProjectile)(pIn - 0x10);
		pThis->vft_module = (void*)_F_BezierProjectileVFT_M;
		return 1;
	}

	__declspec(naked) void __fastcall M_BezierProjectile_CheckProjectilePositionASM()
	{
		__asm {
			mov al, [ebp + 0xD1]
			test al, al
			jz checkIsTORPEDO
			//js IsAlive
			mov edx, RA3::Core::_F_sysCE8F60
			mov ecx, [edx]
		}
	#if 0
		__asm {
			push 0 // get pos?
			mov eax, [ecx] // eax is 00C09E70
			push ebx
			call dword ptr[eax + 0x1C]
			movss xmm0, dword ptr[ebx + 8] // self z
			fstp dword ptr[ebx + 8] // get ground height
			movss xmm1, dword ptr[ebx + 8]
			// if ground z > self z
			comiss xmm1, xmm0
			jae ToDestroy
			movss dword ptr[ebx + 8], xmm0 // restore self z
			jmp IsAlive
		}
	#else
		__asm {
			movq xmm1, qword ptr[ebx]
			sub esp, 0x10
			movups[esp], xmm1
			mov eax, [ecx]
			call dword ptr[eax + 0x18]
			fstp dword ptr[esp]
			movss xmm0, dword ptr[ebx + 8]
			movss xmm1, dword ptr[esp]
			comiss xmm1, xmm0
			jae preciseCheck
			add esp, 4
			jmp IsAlive
		}
	preciseCheck:
		__asm {
			mov edx, esi
			mov ecx, ebp
			call M_BezierProjectile_CheckProjectilePositionCPP
			//mov byte ptr[ebp + 0xD1], -1
			jmp ToDestroy
		}
	#endif

	checkIsTORPEDO:
		__asm {
			mov ecx, [ebp + 4]
			cmp dword ptr[ecx + 0xC], 1
			jne IsAlive
			mov eax, RA3::Core::_F_sysCE8F68
			mov ecx, [eax]
			movq xmm1, qword ptr[ebx]
			sub esp, 0x10
			movups[esp], xmm1
			// old
			/*fld dword ptr[ebx]0x6E0F72
			fld dword ptr[ebx + 4]
			push 0
			push 0
			sub esp, 8
			fstp dword ptr[esp + 4]
			fstp dword ptr[esp]*/
			call RA3::Core::_call_CheckPositionInWater
			test al, al
			jz ToDestroy
		}

	IsAlive:
		__asm {
			pop edi
			pop esi
			pop ebp
			mov al, 1
			pop ebx
			add esp, 0xC
			ret 4
		}

	ToDestroy:
		__asm {
			mov edx, [ebp + 0x24]
			mov eax, [edx + 0xC]
			lea ecx, [ebp + 0x24]
			push 0
			call eax
			pop edi
			pop esi
			pop ebp
			xor al, al
			pop ebx
			add esp, 0xC
			ret 4
		}
	}


	void __fastcall M_BezierProjectile_CheckProjectilePositionCPP(pM_BezierProjectile pIn, void* pGO, float DefaultHeight)
	{
		/*pIn->ammospeed = 0;
		pIn->CurrentSpeed = 0;*/

		float* previous_pos;
		auto pGameObject = (pC_GameObject)pGO;
		if (pIn->alivetime) {
			previous_pos = pGameObject->previous_Position;
		} else {
			previous_pos = pIn->StartPos;
		}

		__m128 v_selfPos = _mm_loadu_ps(pGameObject->Position);
		__m128 v_previousPos = _mm_loadu_ps(previous_pos);
		__m128 v_deltaPos = _mm_sub_ps(v_selfPos, v_previousPos);
		// yeah, check thrice
		v_deltaPos = _mm_mul_ps(v_deltaPos, _mm_set_ps1(0.25f));

		alignas(16) float v_pos[4];
		__m128 v_currentPos = v_previousPos;
		for (int i = 0; i < 3; i++) {
			v_currentPos = _mm_add_ps(v_currentPos, v_deltaPos);
			_mm_store_ps(v_pos, v_currentPos);

			auto pSystem = RA3::Core::C_System_GetW3DTerrainLogic();
			auto fnCheck = pSystem->vf_table->func18;
			float groundHeight = fnCheck(pSystem, fnCheck, v_pos[0], v_pos[1], nullptr);

			if (v_pos[2] <= groundHeight) {
				// hit ground
				pGameObject->Position[0] = v_pos[0];
				pGameObject->transform.x = v_pos[0];
				pGameObject->Position[1] = v_pos[1];
				pGameObject->transform.y = v_pos[1];
				pGameObject->Position[2] = groundHeight;
				pGameObject->transform.z = groundHeight;
				/*memcpy(pIn->EndPos, v_pos, 8);
				pIn->EndPos[2] = groundHeight;*/
				return;
			}

			v_currentPos = _mm_load_ps(v_pos);
		}

		/*memcpy(pIn->EndPos,pGameObject->Position, 8);
		pIn->EndPos[2] = DefaultHeight;*/

		pGameObject->Position[2] = DefaultHeight;
		pGameObject->transform.z = DefaultHeight;
	}

// end namespace RA3::Module
}