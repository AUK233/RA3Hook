#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "C_GameObject.h"

namespace RA3::Core {

	uintptr_t _F_InitializeGameObject1 = 0x4D842F + 1;
	uintptr_t _F_InitializeGameObject2 = 0x7C251E;

	uintptr_t _F_SetGameObjectPosition7E3D1B = 0x7E3D1B;
	uintptr_t _F_UpdateGameObjectTransform = 0x7E3480;
	uintptr_t _F_SetGameObjectFireAllowedStatus = 0x746358;

	void __fastcall C_GameObject_Hook()
	{
		int newGameObjectSize = 0x4FC + 4;
		WriteHookToProcess((void*)_F_InitializeGameObject1, &newGameObjectSize, 4U);

		hookGameBlock((void*)_F_InitializeGameObject2, (uintptr_t)C_GameObject_InitializeASM);
		WriteHookToProcess((void*)(_F_InitializeGameObject2 + 5), (void*)&nop5, 5U);

		// ======================

		hookGameBlock((void*)_F_SetGameObjectPosition7E3D1B, (uintptr_t)C_GameObject_SetGameObjectPosition7E3D1B_ASM);

		hookGameBlock((void*)_F_UpdateGameObjectTransform, (uintptr_t)C_GameObject_UpdateGameObjectTransformCPP);
		WriteHookToProcess((void*)(_F_UpdateGameObjectTransform + 5), (void*)&nop2, 2U);

		// ============================================================================
		BYTE FireAllowedStatus[] = {
			0x8B, 0x4B, 0x04, // mov ecx, [ebx+4]
			0x5F,             // pop edi
			0x5B,             // pop ebx
			0x83, 0xC4, 0x0C  // add esp, 0xC
		};
		WriteHookToProcess((void*)_F_SetGameObjectFireAllowedStatus, (void*)FireAllowedStatus, 8);
		hookGameBlock((void*)(_F_SetGameObjectFireAllowedStatus + 8), (uintptr_t)C_GameObject_SetFireAllowedStatusCPP);
		WriteHookToProcess((void*)(_F_SetGameObjectFireAllowedStatus + 8 + 5), (void*)&nop1, 1U);
	}

	void __fastcall C_GameObject_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_InitializeGameObject1 = 0x51A21F + 1;
			_F_InitializeGameObject2 = 0x80086E;

			_F_SetGameObjectPosition7E3D1B = 0x821FFB;
			_F_UpdateGameObjectTransform = 0x821760;
			_F_SetGameObjectFireAllowedStatus = 0x7848E8;
		}
	}

	__declspec(naked) void __fastcall C_GameObject_InitializeASM()
	{
		__asm {
			xor eax, eax
			mov [ebp + 0x4FC], eax
		}

	original:
		__asm {
			mov eax, ebp
			pop ebp
			pop ebx
			add esp, 0x24
			ret 8
		}
		// end
	}

	__declspec(naked) void __fastcall C_GameObject_SetGameObjectPosition7E3D1B_ASM()
	{
		__asm {
			movups xmm0, [esi + 0x38]
			// 38=8, 3C=C, 40=10, 44=4
			shufps xmm0, xmm0, MY_SHUFFLE(3, 0, 1, 2)
			movups [esp+4], xmm0
			//
			movups xmm1, [esi + 8]
			movups [esp + 0x14], xmm1
			movups xmm0, [esi + 0x18]
			movups [esp + 0x24], xmm0
			movups xmm1, [esi + 0x28]
			movups [esp + 0x34], xmm1
			// set pos to matrix
			mov eax, [esp+0x48]
			mov edx, [eax]
			mov [esi+0x14], edx
			mov [esi+0x38], edx
			mov ecx, [eax+4]
			mov [esi+0x24], ecx
			mov [esi+0x3C], ecx
			mov edx, [eax+8]
			mov [esi+0x34], edx
			mov [esi+0x40], edx
			//
			and dword ptr [esi+0x48], 0xFFFFFFF9
			// push parameter
			mov eax, [esp+4] // is orientation
			push eax
			lea ecx, [esp+0xC] // 8+4, is pos
			push ecx
			lea edx, [esp+0x1C] // 14+8
			push edx
			// get func
			mov eax, [esi]
			mov eax, [eax+0x14]
			mov ecx, esi
			call eax
			pop esi
			add esp, 0x40
			ret 4
		}
	}

	void __fastcall C_GameObject_UpdateGameObjectTransformCPP(pC_GameObject pIn, int useless, Matrix3D_t* newMat)
	{
		/*float OrientationAndPos[4];
		__m128 v_selfPos = _mm_loadu_ps(pIn->Position);
		v_selfPos = _mm_shuffle_ps(v_selfPos, v_selfPos, MY_SHUFFLE(3, 0, 1, 2));
		_mm_storeu_ps(OrientationAndPos, v_selfPos);
		float* p_pos = &OrientationAndPos[1];
		float orientation = OrientationAndPos[0];*/

		float pos[4];
		memcpy(&pos, &pIn->Position, 16);

		Matrix3D_t CurrentMatrix;
		memcpy(&CurrentMatrix, &pIn->transform, sizeof(Matrix3D_t));

		// update matrix
		memcpy(&pIn->transform, newMat, sizeof(Matrix3D_t));

		// update pos
		pIn->Position[0] = pIn->transform.x;
		pIn->Position[1] = pIn->transform.y;
		pIn->Position[2] = pIn->transform.z;
		pIn->Orientation = C_GameObject_atanASM(&pIn->transform); 
		//pIn->Orientation = atan2f(pIn->transform.m1[0], pIn->transform.m0[0]);

		//
		float* p_pos = &pos[0];
		float orientation = pos[3];
		auto fnUpdateTransform = pIn->vf_table->func14;
		fnUpdateTransform(pIn, fnUpdateTransform, &CurrentMatrix, p_pos, orientation);
	}

	__declspec(naked) float __fastcall C_GameObject_atanASM(Matrix3D_t* mat)
	{
		__asm {
			fld dword ptr[ecx + 0x10]
			fld dword ptr[ecx]
			fpatan
			ret
		}
	}

	int __fastcall C_GameObject_SetFireAllowedStatusCPP(pC_ThingTemplate pIn)
	{
		if (pIn->KindOf[(int)KindOfType::STRUCTURE]) {
			if (pIn->KindOf[(int)KindOfType::FS_BASE_DEFENSE]) {
				return 1;
			} else {
				return 0;
			}
		}

		return 1;
	}

// end namespace RA3::Core
}
