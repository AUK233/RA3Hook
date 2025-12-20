#include "../pch.h"
#include <format>
#include <corecrt_math_defines.h>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Core/C_System.h"
#include "../Core/C_GameObject.h"
#include "M_LaserState.h"

namespace RA3::Module {

	uintptr_t _F_ActivateLaser = 0x7CF668;
	uintptr_t _Ret_ActivateLaser = 0x7CF668 + 6;
	uintptr_t _F_ActivateLaserCheck54h = 0x74BD77;

	uintptr_t _F_LaserStateVFT = 0xC41420;

	uintptr_t _F_SweepingLaserStateInitialize = 0x7C3D69;
	uintptr_t _F_SweepingLaserStateVFT = 0xC4DCE0;

	void_thiscallHasParameter2 _call_LaserStateVFT58;

	void __fastcall M_LaserState_Hook() {
		_call_LaserStateVFT58 = *(void_thiscallHasParameter2*)(_F_LaserStateVFT + 0x58);

		// do not increase size now.
		/*uintptr_t _F_SweepingLaserStateSize = 0x7E0A80 + 1;
		int newSweepingLaserStateSize = sizeof(M_SweepingLaserState_t);
		WriteHookToProcess((void*)_F_SweepingLaserStateSize, &newSweepingLaserStateSize, 4U);*/

		hookGameBlock((void*)_F_SweepingLaserStateInitialize, (uintptr_t)M_SweepingLaserState_InitializeASM);
		WriteHookToProcess((void*)(_F_SweepingLaserStateInitialize + 5), (void*)&nop3, 3U);

		uintptr_t newVirtualFunction = (uintptr_t)M_SweepingLaserState_vft58CPP;
		WriteHookToProcess((void*)(_F_SweepingLaserStateVFT + 0x58), &newVirtualFunction, 4U);

		newVirtualFunction = (uintptr_t)M_SweepingLaserState_vft5Ccpp;
		WriteHookToProcess((void*)(_F_SweepingLaserStateVFT + 0x5C), &newVirtualFunction, 4U);

		// in weapon ==========================================================

		// Set laser to activate only when not activated
		hookGameBlock((void*)_F_ActivateLaser, (uintptr_t)ActivateLaserNuggetASM);
		// Fix no laser for attacking a target when DamageDealtAtSelfPosition is true
		unsigned char set34BD77[] = {
			0x83, 0x7E, 0x54, 0x00, // cmp dword ptr [esi+0x54], 0
			0x5F,                   // pop edi
			0x74, 0x25,             // je
			0x85, 0xED,             // test ebp, ebp
			0x75, 0x16             // jne
		};
		WriteHookToProcess((void*)_F_ActivateLaserCheck54h, &set34BD77, 11U);
	}

	void __fastcall M_LaserState_Initialize(uintptr_t hmodEXE, int isNewSteam) {
		if (isNewSteam) {
			_F_ActivateLaser = 0x80D988;
			_Ret_ActivateLaser = 0x80D988 + 6;
			_F_ActivateLaserCheck54h = 0x78A397;

			_F_LaserStateVFT = 0xC48540;

			_F_SweepingLaserStateInitialize = 0x8020B9;
			_F_SweepingLaserStateVFT = 0xC54D30;
		}
	}

	__declspec(naked) void __fastcall M_SweepingLaserState_InitializeASM() {
		// xmm0 has been reset to 0.
		__asm {
			/*xor eax, eax
			movups[esi + 0x98], xmm0
			movups[esi + 0xA8], xmm0
			movsd qword ptr[esi + 0xB8], xmm0
			mov[esi + 0xC0], eax*/
			mov ecx, esi
			call M_SweepingLaserState_InitializeCPP
			//
			mov eax, esi
			pop esi
			ret 8
		}
	}

	void __fastcall M_SweepingLaserState_InitializeCPP(pM_SweepingLaserState pIn) {
		ZeroMemory(pIn->SweepEndPos, (sizeof(M_SweepingLaserState_t) - offsetof(M_SweepingLaserState_t, SweepEndPos)));

		int i_IsFollowTerrain = 0;
		pIn->i16_LaserType = M_SweepingLaserState_GetLaserType((pData_SweepingLaserState)pIn->pModuleData, &i_IsFollowTerrain);

		if (i_IsFollowTerrain) {
			pIn->bIsFollowTerrain = true;
		}
	}

	UINT16 __fastcall M_SweepingLaserState_GetLaserType(pData_SweepingLaserState pData, int* IsFollowTerrain) {
		int laserID = pData->LaserId;
		if (laserID > 499) {
			if (laserID > 999) {
				int laserIndex = (laserID - 1000) / 20;
				switch (laserIndex)
				{
				case 0:
					return SweepLaserT_toTarget0dot1;
				case 1:
					return SweepLaserT_baseRotation;
				case 100: // 3000
					return SweepLaserT_formSourceToTarget;
				case 101:{
					// 3020
					*IsFollowTerrain = 1;
					return SweepLaserT_formSourceToDistance;
				}
				case 102: // 3040
					return SweepLaserT_formSourceToDistance;
				default:
					return SweepLaserT_Enhanced;
				}
			} else {
				return SweepLaserT_Enhanced;
			}
		} else {
			if (laserID > 299) {
				*IsFollowTerrain = 1;
			}

			return SweepLaserT_Original;
		}
		// end
	}

	// maybe is set sweeping positions
	void __fastcall M_SweepingLaserState_vft58CPP(pM_SweepingLaserState pIn, int, int inLifetime, int a2) {
		// original is 007C3E80
		_call_LaserStateVFT58(pIn, 0, inLifetime, a2);
		pIn->Lifetime = inLifetime; // this is necessary!

		switch (pIn->i16_LaserType)
		{
		case SweepLaserT_Enhanced:
			M_SweepingLaserState_SetPos_Enhanced(pIn);
			break;
		case SweepLaserT_toTarget0dot1:
			M_SweepingLaserState_SetPos_toTarget0dot1((uintptr_t)pIn);
			break;
		case SweepLaserT_baseRotation:
			M_SweepingLaserState_SetPos_baseRotation((uintptr_t)pIn);
			break;
		case SweepLaserT_formSourceToTarget:
			M_SweepingLaserState_SetPos_formSourceToTarget(pIn);
			break;
		case SweepLaserT_formSourceToDistance:
			M_SweepingLaserState_SetPos_formSourceToDistance(pIn);
			break;
		default:
			M_SweepingLaserState_SetPos_Default(pIn);
			break;
		}
		// end
	}

	// update laser position
	void __fastcall M_SweepingLaserState_vft5Ccpp(pM_SweepingLaserState pIn, int, float* pOutPos) {
		// original is 006E3714
		static_assert(offsetof(M_SweepingLaserState_t, LaserTimer) == 0x58);
		int laserTimer = pIn->LaserTimer;
		auto pGameSystem = GS_GetGameSystemPointer();
		static_assert(offsetof(G_GameSystem_t, IngameTimer) == 0x50);
		laserTimer -= pGameSystem->IngameTimer;

		static_assert(offsetof(M_SweepingLaserState_t, Lifetime) == 0xB0);
		int lifetime = pIn->Lifetime;
		float laserProgress = 0.0f;
		if (laserTimer > 0 && lifetime > 0) {
			/*__m128 zero128 = _mm_setzero_ps();
			float laserDelta = _mm_max_ss(_mm_set_ss(laserTimer), zero128).m128_f32[0];
			float lifetimeF = _mm_max_ss(_mm_set_ss(lifetime), zero128).m128_f32[0];*/
			
			float laserDelta = laserTimer;
			float lifetimeF = lifetime;
			laserProgress = laserDelta / lifetimeF;
		}

		// check swap direction
		if (pIn->bIsSwapDirection) {
			laserProgress = 1.0f - laserProgress;
		}

		if (pIn->bIsRotation == 0) {
			__m128 startPos = _mm_loadu_ps(pIn->SweepStartPos);
			__m128 endPos = _mm_loadu_ps(pIn->SweepEndPos);
			// yeah, it's like this.
			__m128 deltaPos = _mm_sub_ps(startPos, endPos);
			deltaPos = _mm_mul_ps(deltaPos, _mm_set_ps1(laserProgress));
			deltaPos = _mm_add_ps(deltaPos, endPos);

			// output!
			pOutPos[0] = deltaPos.m128_f32[0];
			pOutPos[1] = deltaPos.m128_f32[1];
			pOutPos[2] = deltaPos.m128_f32[2];
		}
		else {
			float sweepAngle = laserProgress * 2 * M_PI;
			float cosAngle = std::cos(sweepAngle);
			float sinAngle = std::sin(sweepAngle);

			/*float ofsx = pIn->SweepEndPos[0];
			float ofsy = pIn->SweepEndPos[1];
			pOutPos[0] = pIn->SweepStartPos[0] + (cosAngle * ofsx) - (sinAngle * ofsy);
			pOutPos[1] = pIn->SweepStartPos[1] + (cosAngle * ofsy) + (sinAngle * ofsx);*/

			// sin, 0, cos, 0
			__m128 vSinCos = _mm_movelh_ps(_mm_set_ss(sinAngle), _mm_set_ss(cosAngle));
			// to sin, cos, sin, cos
			vSinCos = _mm_shuffle_ps(vSinCos, vSinCos, MY_SHUFFLE(0, 2, 0, 2));

			__m128 deltaPos = _mm_loadu_ps(pIn->SweepEndPos);
			// to -y, x, x, y
			deltaPos = _mm_shuffle_ps(deltaPos, deltaPos, MY_SHUFFLE(1, 0, 0, 1));
			deltaPos = _mm_xor_ps(deltaPos, _mm_setr_ps(-0.0f, 0.0f, 0.0f, 0.0f));
			// -siny, cosx, sinx, cosy
			deltaPos = _mm_mul_ps(deltaPos, vSinCos);
			// cosx - siny (0+1), cosy + sinx (2+3)
			deltaPos = _mm_hadd_ps(deltaPos, deltaPos);

			__m128 endPos = _mm_loadu_ps(pIn->SweepStartPos); // this is necessary!
			deltaPos = _mm_add_ps(deltaPos, endPos);
			pOutPos[0] = deltaPos.m128_f32[0];
			pOutPos[1] = deltaPos.m128_f32[1];
			pOutPos[2] = pIn->SweepStartPos[2];
		}

		if (pIn->bIsFollowTerrain) {
			float outHeight[2]; // 0 is land, 1 is water
			bool inWater = RA3::Core::C_System_CheckPositionInWater(0, 0, pOutPos[0], pOutPos[1], &outHeight[1], &outHeight[0]);
			pOutPos[2] = outHeight[inWater];
		}
		// end
	}

	void __fastcall M_SweepingLaserState_SetPos_Default(pM_SweepingLaserState pIn) {
		__m128 selfPos = _mm_loadu_ps(pIn->SelfPos);
		__m128 targetPos = _mm_loadu_ps(pIn->TargetPos);

		__m128 deltaPos = _mm_sub_ps(targetPos, selfPos);
		float dot = _mm_dp_ps(deltaPos, deltaPos, 0b01110001).m128_f32[0];

		__m128 deltaXYZ;
		if (dot <= 0.0001f) 
		{
			deltaXYZ = _mm_set_ss(1.0f);
		}
		else
		{
			__m128 scale;
			if (dot == 0.0f)
			{
				scale = _mm_setzero_ps();
			}
			else
			{
				scale = _mm_rsqrt_ss(_mm_set_ss(dot));
				scale = _mm_shuffle_ps(scale, scale, 0);
			}
			deltaXYZ = _mm_mul_ps(deltaPos, scale);
		}

		pData_SweepingLaserState pData = (pData_SweepingLaserState)pIn->pModuleData;
		float Radius = pData->Radius;
		__m128 radiusVec = _mm_set_ps1(Radius);
		deltaXYZ = _mm_mul_ps(deltaXYZ, radiusVec);

		if (Radius < -0.1f) {
			// vertical sweep, one way
			pIn->bIsSwapDirection = 0;
		}
		else {
			// horizontal sweep, both directions
			pIn->bIsSwapDirection = !pIn->bIsSwapDirection;
			alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
			deltaXYZ = _mm_xor_ps(deltaXYZ, negativeX);
			deltaXYZ = _mm_shuffle_ps(deltaXYZ, deltaXYZ, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm));
			/*float DeltaX = -deltaXYZ.m128_f32[0];

			deltaXYZ.m128_f32[0] = deltaXYZ.m128_f32[1];
			deltaXYZ.m128_f32[1] = DeltaX;*/
		}

		// clear high 64bit
		__m128 ofsPos = _mm_movelh_ps(deltaXYZ, _mm_setzero_ps());

		__m128 sweepEndPos = _mm_sub_ps(targetPos, ofsPos);
		_mm_storeu_ps(pIn->SweepEndPos, sweepEndPos);

		__m128 sweepStartPos = _mm_add_ps(targetPos, ofsPos);
		/*pIn->SweepStartPos[0] = sweepStartPos.m128_f32[0];
		pIn->SweepStartPos[1] = sweepStartPos.m128_f32[1];
		pIn->SweepStartPos[2] = sweepStartPos.m128_f32[2];*/
		auto Lifetime = pIn->Lifetime;
		_mm_storeu_ps(pIn->SweepStartPos, sweepStartPos);
		pIn->Lifetime = Lifetime;
	}

	void __fastcall M_SweepingLaserState_SetPos_Enhanced(pM_SweepingLaserState pIn) {
		__m128 selfPos = _mm_loadu_ps(pIn->SelfPos);
		__m128 targetPos = _mm_loadu_ps(pIn->TargetPos);
		__m128 deltaPos = _mm_sub_ps(targetPos, selfPos);

		// Read EndOffset's z as radian.
		pData_SweepingLaserState pData = (pData_SweepingLaserState)pIn->pModuleData;
		if (pData->pEndOffset) {
			if (*(int*)&pData->pEndOffset[2]) {
				float ofsRadian = pData->pEndOffset[2];
				float sinRadian = std::sin(ofsRadian);
				float cosRadian = std::cos(ofsRadian);

				float spx = deltaPos.m128_f32[0];
				float spy = deltaPos.m128_f32[1];

				float dx = cosRadian * spx - sinRadian * spy;
				float dy = cosRadian * spy + sinRadian * spx;
				deltaPos.m128_f32[0] = dx;
				deltaPos.m128_f32[1] = dy;
			}
		}

		float dot = _mm_dp_ps(deltaPos, deltaPos, 0b01110001).m128_f32[0];

		__m128 deltaXYZ;
		if (dot <= 0.0001f) {
			deltaXYZ = _mm_set_ss(1.0f);
		} else {
			__m128 scale;
			if (dot == 0.0f)
			{
				scale = _mm_setzero_ps();
			}
			else
			{
				scale = _mm_rsqrt_ss(_mm_set_ss(dot));
				scale = _mm_shuffle_ps(scale, scale, 0);
			}
			deltaXYZ = _mm_mul_ps(deltaPos, scale);
		}

		float Radius = pData->Radius;
		__m128 radiusVec = _mm_set_ps1(Radius);
		deltaXYZ = _mm_mul_ps(deltaXYZ, radiusVec);

		if (Radius < -0.1f) {
			// vertical sweep, one way
			pIn->bIsSwapDirection = 0;
		}
		else {
			// horizontal sweep, both directions
			pIn->bIsSwapDirection = !pIn->bIsSwapDirection;
			alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
			deltaXYZ = _mm_xor_ps(deltaXYZ, negativeX);
			deltaXYZ = _mm_shuffle_ps(deltaXYZ, deltaXYZ, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm));
		}

		// clear high 64bit
		__m128 ofsPos = _mm_movelh_ps(deltaXYZ, _mm_setzero_ps());

		__m128 sweepEndPos = _mm_sub_ps(targetPos, ofsPos);
		_mm_storeu_ps(pIn->SweepEndPos, sweepEndPos);

		__m128 sweepStartPos = _mm_add_ps(targetPos, ofsPos);
		/*pIn->SweepStartPos[0] = sweepStartPos.m128_f32[0];
		pIn->SweepStartPos[1] = sweepStartPos.m128_f32[1];
		pIn->SweepStartPos[2] = sweepStartPos.m128_f32[2];*/
		auto Lifetime = pIn->Lifetime;
		_mm_storeu_ps(pIn->SweepStartPos, sweepStartPos);
		pIn->Lifetime = Lifetime;
	}

	void __fastcall M_SweepingLaserState_SetPos_toTarget0dot1(uintptr_t ptr) {
		// Target Position
		float opx = *(float*)(ptr + 0x3C);
		float opy = *(float*)(ptr + 0x40);
		float opz = *(float*)(ptr + 0x44);
		// The difference between the target and its own position.
		float spx = opx - *(float*)(ptr + 0x30);
		float spy = opy - *(float*)(ptr + 0x34);
		// Read EndOffset's z as radian.
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
		if (posOffsetPtr)
		{
			float ofsRadian = *(float*)(posOffsetPtr + 8);
			if (ofsRadian) {
				// rotate coordinates
				float dx = cos(ofsRadian) * spx - sin(ofsRadian) * spy;
				float dy = cos(ofsRadian) * spy + sin(ofsRadian) * spx;
				spx = dx;
				spy = dy;
			}
		}

		float spz = opz - *(float*)(ptr + 0x38);

		float fr = (spz * spz) + (spy * spy) + (spx * spx);
		float DeltaX;
		float DeltaY;
		float DeltaZ;
		if (fr <= 0.0001f)
		{
			DeltaX = 1.0f;
			DeltaY = 0.0f;
			DeltaZ = 0.0f;
		}
		else
		{
			float scale;
			if (fr == 0.0f)
			{
				scale = 0.0f;
			}
			else
			{
				scale = 1.0f / sqrt(fr);
			}
			DeltaX = spx * scale;
			DeltaY = spy * scale;
			DeltaZ = spz * scale;
		}

		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
		float ofsx = DeltaX * Radius;
		float ofsy = DeltaY * Radius;
		float ofsz = 0.0f;
		*(char*)(ptr + 0xBC) = 0;

		// sweep start position
		*(float*)(ptr + 0xA4) = opx - ofsx;
		*(float*)(ptr + 0xA8) = opy - ofsy;
		*(float*)(ptr + 0xAC) = opz - ofsz;
		// sweep end 
		ofsx *= 0.1f;
		ofsy *= 0.1f;
		*(float*)(ptr + 0x98) = opx + ofsx;
		*(float*)(ptr + 0x9C) = opy + ofsy;
		*(float*)(ptr + 0xA0) = opz + ofsz;
	}

	void __fastcall M_SweepingLaserState_SetPos_baseRotation(uintptr_t ptr) {
		pM_SweepingLaserState pIn = (pM_SweepingLaserState)ptr;
		pIn->bIsRotation = true;
		*(int*)(ptr + 0xA0) = 0;
		// sweep start position
		*(float*)(ptr + 0xA4) = *(float*)(ptr + 0x3C);
		*(float*)(ptr + 0xA8) = *(float*)(ptr + 0x40);
		*(float*)(ptr + 0xAC) = *(float*)(ptr + 0x44);

		// sweep rotation angle
		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
		float Orientation;
		if (Radius < -0.1f) {
			Orientation = getRadomFloatValue(0.0f, 2 * M_PI);
			Radius *= -1.0f;
		}
		else {
			Orientation = *(float*)(*(uintptr_t*)(ptr + 8) + 0x44);
			pIn->bIsSwapDirection = !pIn->bIsSwapDirection;
		}
		// Read EndOffset's z as angle.
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
		if (posOffsetPtr)
		{
			Orientation += *(float*)(posOffsetPtr + 8);
		}

		float ofsx = std::cos(Orientation) * Radius;
		float ofsy = std::sin(Orientation) * Radius;
		*(float*)(ptr + 0x98) = ofsx;
		*(float*)(ptr + 0x9C) = ofsy;
	}

	void __fastcall M_SweepingLaserState_SetPos_formSourceToTarget(pM_SweepingLaserState pIn)
	{
		pIn->bIsSwapDirection = 0;
		auto pGameObject = (pC_GameObject)pIn->pGameObject;
		float Orientation = pGameObject->Orientation;

		pData_SweepingLaserState pData = (pData_SweepingLaserState)pIn->pModuleData;
		if (pData->pEndOffset) {
			Orientation += pData->pEndOffset[2];
		}

		float Radius = pData->Radius;
		__m128d vd_ofs;
		vd_ofs.m128d_f64[0] = std::cos((double)Orientation);
		vd_ofs.m128d_f64[1] = std::sin((double)Orientation);

		__m128 v_ofs = _mm_cvtpd_ps(vd_ofs);
		v_ofs = _mm_mul_ps(v_ofs, _mm_set_ps1(Radius));

		auto Lifetime = pIn->Lifetime;
		__m128 selfPos = _mm_loadu_ps(pIn->SelfPos);
		__m128 sweepEndPos = _mm_add_ps(selfPos, v_ofs);
		_mm_storeu_ps(pIn->SweepEndPos, sweepEndPos);
		_mm_storeu_ps(pIn->SweepStartPos, selfPos);
		//pIn->SweepEndPos[2] = selfPos.m128_f32[2];
		pIn->Lifetime = Lifetime;
	}

	void __fastcall M_SweepingLaserState_SetPos_formSourceToDistance(pM_SweepingLaserState pIn)
	{
		pIn->bIsSwapDirection = 0;
		__m128 selfPos = _mm_loadu_ps(pIn->SelfPos);
		__m128 targetPos = _mm_loadu_ps(pIn->TargetPos);
		__m128 deltaPos = _mm_sub_ps(targetPos, selfPos);
		__m128 v_delta = deltaPos;
		//float HeightDifference = deltaPos.m128_f32[2];
		float DistanceFactor = _mm_dp_ps(deltaPos, deltaPos, 0b00110001).m128_f32[0];
		DistanceFactor = _mm_rsqrt_ss(_mm_set_ss(DistanceFactor)).m128_f32[0];

		// Read EndOffset's z as minimum distance.
		float minDistance = 0.0f;
		pData_SweepingLaserState pData = (pData_SweepingLaserState)pIn->pModuleData;
		if (pData->pEndOffset) {
			minDistance = pData->pEndOffset[2];
		}

		// Read target distance
		float Length = pData->Radius;
		float LengthFactor = Length * DistanceFactor;

		if (LengthFactor <= 1.0f) {
			minDistance += Length;
			minDistance -= (Length * LengthFactor);
			LengthFactor = 1.0f;
		}
		minDistance *= DistanceFactor;

		__m128 sweepEndPos = _mm_mul_ps(v_delta, _mm_set_ps1(LengthFactor));
		sweepEndPos = _mm_add_ps(selfPos, sweepEndPos);
		_mm_storeu_ps(pIn->SweepEndPos, sweepEndPos);
		if (std::abs(v_delta.m128_f32[2]) <= 20.0f) {
			pIn->SweepEndPos[2] = pIn->TargetPos[2];
		}

		__m128 sweepStartPos = _mm_mul_ps(v_delta, _mm_set_ps1(minDistance));
		sweepStartPos = _mm_add_ps(selfPos, sweepStartPos);
		pIn->SweepStartPos[0] = sweepStartPos.m128_f32[0];
		pIn->SweepStartPos[1] = sweepStartPos.m128_f32[1];
		pIn->SweepStartPos[2] = selfPos.m128_f32[2]; // Yeah, use unchanged xmm instead of reading it from memory.
	}

	__declspec(naked) void __fastcall ActivateLaserNuggetASM(uintptr_t ptr)
	{
		__asm {
			je retFunc
			mov eax, [ebp + 0x10]
			// check laser id
			cmp dword ptr[eax + 0x2C], 3000
			jl Original
			// check laser activation
			cmp byte ptr[esi + 0x94], 0
			jne retFunc
		Original:
			jmp _Ret_ActivateLaser
		retFunc:
			pop edi
			pop esi
			pop ebp
			pop ebx
			add esp, 0x10
			ret 0x10
		}
	}

// end namespace RA3::Module
}
