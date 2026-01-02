#include "../pch.h"
#include <format>
#include <corecrt_math_defines.h>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Modules/M_AIUpdate.h"
#include "C_System.h"
#include "C_Weapon.h"


namespace RA3::Core {
	uintptr_t _F_ShowAmmo = 0x528746;
	uintptr_t _F_SetupPreAttackWeaponPos = 0x79FB6E;
	/*uintptr_t _F_SetAttackPosition = 0x75A9FE;
	uintptr_t _Ret_SetWeaponAttackPosition = 0x75A9FE + 73;*/
	// new
	uintptr_t _F_SetWeaponAttackPosition = 0x75A7E0;
	uintptr_t _Ret_SetWeaponAttackPosition = 0x75A7E0 + 6;

	uintptr_t _F_WeaponFireNuggets = 0x75AA78;
	uintptr_t _F_GetWeaponScatterInForcedAttack = 0x719E9B;
	uintptr_t _F_GetWeaponScatterFromContactPoints = 0x747859;

	uintptr_t _call_GetCurrentWeaponAttackRange = 0x713770;
	uintptr_t _call_GetWeaponNuggetScatter = 0x713BD0;
	uintptr_t _call_GetTargetScatterRadiusVsType = 0x6F9460;


	void __fastcall C_Weapon_Hook()
	{
		// Let "ShowsAmmoPips" work
		hookGameCall((void*)_F_ShowAmmo, (uintptr_t)C_Weapon_ShowsAmmoPipsASM);
		WriteHookToProcess((void*)(_F_ShowAmmo + 5), (void*)&nop2, 2U);

		hookGameCall((void*)_F_SetupPreAttackWeaponPos, (uintptr_t)C_Weapon_SetupPreAttackWeaponPosASM);
		WriteHookToProcess((void*)(_F_SetupPreAttackWeaponPos + 5), (void*)&nop2, 2U);

		// Allow weapons to fire to maximum range
		hookGameBlock((void*)_F_SetWeaponAttackPosition, (uintptr_t)C_WeaponTemplate_SetAttackPositionASM);
		//WriteHookToProcess((void*)(_F_SetAttackPosition + 5), (void*)&nop5, 5U);
		WriteHookToProcess((void*)(_F_SetWeaponAttackPosition + 5), (void*)&nop1, 1U);

		hookGameBlock((void*)_F_WeaponFireNuggets, (uintptr_t)C_WeaponTemplate_FireNuggetsASM);
		WriteHookToProcess((void*)(_F_WeaponFireNuggets + 5), (void*)&nop3, 3U);

		// Update original to our new function
		hookGameCall((void*)_F_GetWeaponScatterInForcedAttack, (uintptr_t)C_WeaponTemplate_GetWeaponNuggetScatter);
		hookGameCall((void*)_F_GetWeaponScatterFromContactPoints, (uintptr_t)C_WeaponTemplate_GetWeaponScatterFromContactPoints);
	}

	void __fastcall C_Weapon_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {
			_F_ShowAmmo = hmodEXE + 0x169D96;
			_F_SetupPreAttackWeaponPos = 0x7DDF0E;
			/*_F_SetAttackPosition = 0x79903E;
			_Ret_SetWeaponAttackPosition = 0x79903E + 73;*/
			_F_SetWeaponAttackPosition = 0x798E20;
			_Ret_SetWeaponAttackPosition = 0x798E20 + 6;

			_F_WeaponFireNuggets = 0x7990B8;
			_F_GetWeaponScatterInForcedAttack = 0x7582EB;
			_F_GetWeaponScatterFromContactPoints = 0x785DE9;

			_call_GetCurrentWeaponAttackRange = 0x751BC0;
			_call_GetWeaponNuggetScatter = 0x752020;
			_call_GetTargetScatterRadiusVsType = 0x737870;
		}
	}

	// Let "ShowsAmmoPips" work
	__declspec(naked) void __fastcall C_Weapon_ShowsAmmoPipsASM()
	{
		// edx is not display
		__asm {
			push eax
			// AutoReloadsClip is RETURN_TO_BASE
			cmp dword ptr[eax + 0xB8], 2
			je SetFlag
			// ShowsAmmoPips is true
			cmp byte ptr[eax + 0x134], 1
			je SetFlag
			mov eax, 1
			test eax, eax
			pop eax
			ret
		}

	SetFlag:
		__asm {
			xor eax, eax
			test eax, eax
			pop eax
			ret
		}
	}

	__declspec(naked) void __fastcall C_Weapon_SetupPreAttackWeaponPosASM()
	{
		// esp need +4 because of esp has decreased by 4
		__asm {
			// ShowsAmmoPips is true
			cmp byte ptr[edx + 0x134 - 0x10], 1 // Requires -10h, because it's not header.
			lea edx, [esp + 0x10 + 4]
			jne addr79FB6E
			test ecx, ecx
			jz NoTarget // = 0
			xor ecx, ecx
			lea eax, [esi + 0x4C] // FirePosInTarget
			ret
		NoTarget:
			lea eax, [esi + 0x40] // FirePosNoTarget
			ret
		addr79FB6E:
			lea eax, [edi + 0x38] // GameObject self pos
			ret
		}
	}

	__declspec(naked) void __fastcall C_WeaponTemplate_SetAttackPositionASM()
	{
		__asm {
			mov edx, [edi + 4] // yeah, this is pWeaponTemplate
			mov al, [edx + 0x137] // IsAimingWeapon
			test[edx + 0x138], al // NoVictimNeeded
			jz Original35A7E0
			lea ecx, [esp + 0x2C] // target position
			push ecx
			mov edx, esi
			mov ecx, edi
			call C_WeaponTemplate_SetAttackPositionToRangeCPP
		}
	Original35A7E0:
		__asm {
			mov ecx, [esi + 0x18]
			mov eax, [ecx + 0x10]
			jmp _Ret_SetWeaponAttackPosition
		}
		// Useless old block
	#if 0
		// Not using it now because we need GameObject pointer.
		//push ebx // this is self position
		__asm {
			mov ecx, [ebp + 4]
			//cmp byte ptr[ecx + 0x134], 0
			mov al, [ecx + 0x137] // IsAimingWeapon
			test [ecx + 0x138], al // NoVictimNeeded
			jz checkDamageDealtAtSelfPosition
			lea eax, ReturnOriginal
			lea ecx, [esp + 0x2C] // target position
			push ecx
			mov edx, esi
			mov ecx, ebp
			push eax // return address
			jmp C_WeaponTemplate_SetAttackPositionToRangeCPP
		}

	checkDamageDealtAtSelfPosition:
		__asm {
			cmp byte ptr [ecx + 0x12C], 0
			je check35AA28
			mov [esi + 8], ebx
			mov [esi + 0x1C], 0
			mov ecx, [ebx]
			mov [esp + 0x2C], ecx
			mov edx, [ebx + 4]
			mov [esp + 0x30], edx
			mov eax, [ebx + 8]
			mov [esp + 0x34], eax
			jmp ReturnOriginal
		}

	check35AA28:
		__asm {
			mov eax, [esp + 0x1C]
			test eax, eax
			jz ReturnOriginal
			mov ecx, [eax]
			mov [esp + 0x2C], ecx
			mov edx, [eax + 4]
			mov [esp + 0x30], edx
			mov eax, [eax + 8]
			mov [esp + 0x34], eax
		}

	ReturnOriginal:
		__asm {
			mov ecx, [ebp + 4]
			jmp _Ret_SetWeaponAttackPosition
		}
	#endif
	}

	void __fastcall C_WeaponTemplate_SetAttackPositionToRangeCPP(pC_WeaponTemplate pIn, PBYTE pESI, float* targetPos)
	{
		auto pGameObject = *(pC_GameObject*)(pESI + 4);
		auto pTargetPos = *(float**)(pESI + 0x2C); // used to set FX's target position

		__m128 v_selfPos = _mm_loadu_ps(pGameObject->Position);
		__m128 v_targetPos = _mm_loadu_ps(pTargetPos);
		__m128 v_deltaPos = _mm_sub_ps(v_targetPos, v_selfPos);
		__m128 v_delta = v_deltaPos;

		// Intel and AMD return different results for rsqrt, so it is not used at present.
		/*float DistanceFactor = _mm_dp_ps(v_deltaPos, v_deltaPos, 0b00110001).m128_f32[0];
		DistanceFactor = _mm_rsqrt_ss(_mm_set_ss(DistanceFactor)).m128_f32[0];
		float AttackRange = C_WeaponTemplate_GetCurrentWeaponAttackRange(pIn, 0, pGameObject, -v_selfPos.m128_f32[2]);
		float RangeFactor = AttackRange * DistanceFactor;
		if (RangeFactor <= 1.0f) {
			return;
		}*/

		float Distance = _mm_dp_ps(v_deltaPos, v_deltaPos, 0b00110001).m128_f32[0];
		Distance = _mm_sqrt_ss(_mm_set_ss(Distance)).m128_f32[0];

		float AttackRange = C_WeaponTemplate_GetCurrentWeaponAttackRange(pIn, 0, pGameObject, -v_selfPos.m128_f32[2]);
		if(AttackRange <= Distance) {
			return;
		}

		float RangeFactor = AttackRange / Distance;

		__m128 v_attackPos = _mm_mul_ps(v_delta, _mm_set_ps1(RangeFactor));
		float HeightDifference = std::abs(v_delta.m128_f32[2]);
		v_attackPos = _mm_add_ps(v_selfPos, v_attackPos);
		// prevent out range
		v_attackPos = _mm_max_ps(v_attackPos, _mm_setzero_ps());
		v_attackPos = _mm_min_ps(v_attackPos, _mm_set_ps1(9999.0f));

		targetPos[0] = v_attackPos.m128_f32[0];
		targetPos[1] = v_attackPos.m128_f32[1];
		pTargetPos[0] = v_attackPos.m128_f32[0];
		pTargetPos[1] = v_attackPos.m128_f32[1];

		if (HeightDifference > 20.0f) {
			targetPos[2] = v_attackPos.m128_f32[2];
			pTargetPos[2] = v_attackPos.m128_f32[2];
		}
	}

	__declspec(naked) void __fastcall C_WeaponTemplate_FireNuggetsASM()
	{
	#if 0
	startBlock:
		__asm {
			cmp byte ptr[esp + 0x60], 0
			mov edi, [ebp + 8]
			je fire35AAC4 // no scatter
		}
	scatter35AA8A:
		__asm {
			mov eax, [esi + 0x18]
			mov ecx, [esi + 0x1C]
			lea edx, [esp + 0x2C]
			push edx
			mov edx, [esi + 4]
			push eax
			push ecx
			mov ecx, [esp + 0x1C]
			push edx
			lea eax, [esp + 0x30]
			push eax
			call _call_GetWeaponNuggetScatter
			/*mov ecx, [eax]
			mov[esp + 0x2C], ecx
			mov edx, [eax + 4]
			mov[esp + 0x30], edx
			mov eax, [eax + 8]
			mov[esp + 0x34], eax*/
			mov[esi + 0x1C], 0
		}

	fire35AAC4:
		__asm {
			mov edx, [edi]
			mov edx, [edx + 0xC]
			lea eax, [esp + 0x20]
			push eax
			push esi
			mov ecx, edi
			call edx
			test al, al
			je checkCount35AAEE
			//
			mov ebx, [esi + 8]
			lea eax, [esp + 0x20]
			mov[esi + 8], eax
			mov edx, [edi]
			mov eax, [edx + 0x20]
			push esi
			mov ecx, edi
			call eax
			mov[esi + 8], ebx
		}

	checkCount35AAEE:
		__asm {
			mov eax, [esp + 0x10]
			mov ebp, [ebp]
			add eax, 8
			cmp ebp, eax
			jne startBlock
		}
	#else
		__asm {
			movzx edi, byte ptr[esp + 0x60]
			mov[esp + 0x20], ebp
			mov[esp + 0x24], edi
			//
			mov edx, esi
			lea ecx, [esp + 0x10]
			call C_WeaponTemplate_FireNuggetsCPP
		}
	#endif

	endBlock:
		__asm {
			pop ebx
			pop edi
			pop esi
			pop ebp
			add esp, 0x4C
			ret 4
		}
	}

	void __fastcall C_WeaponTemplate_FireNuggetsCPP(void* psp, pWeaponFireInStack pFire)
	{
		struct inNuggetList_t;
		struct inNuggetList_t {
			inNuggetList_t* next;
			void* pad4;
			int* pNugget;
		};

		struct inSpecial_t {
			pC_WeaponTemplate pWT;
			char pad4[0xC];
			inNuggetList_t* pList;
			int hasScatter;
			int* pNugget; //useless
			float targetPos[3];
		};
		static_assert(offsetof(inSpecial_t, pList) == 0x10);

		// ok, let's go
		auto pInECX = (inSpecial_t*)psp;
		auto targetPos = pInECX->targetPos;
		float attack_pos[3];
		memcpy(attack_pos, targetPos, 12);

		auto pWT = pInECX->pWT;
		inNuggetList_t* pNuggetList = pInECX->pList;
		goto fireNugget; // skip check, because it has been checked before.

		while(pNuggetList != (inNuggetList_t*)&pWT->pNuggetList){
		fireNugget:
			auto pNugget = pNuggetList->pNugget;
			if (pInECX->hasScatter) {
				C_WeaponTemplate_GetWeaponNuggetScatter(pWT, 0, attack_pos, pFire->pSelf, pFire->pTarget, pFire->pWeapon, targetPos);
				pFire->pTarget = 0;
			}

			int* vft = reinterpret_cast<int*>(pNugget[0]);

			auto fnFireNugget0C = (CallFunc_WeaponTemplateFireNugget0C)vft[3]; // 0xC / 4
			auto result = fnFireNugget0C(pNugget, fnFireNugget0C, pFire, attack_pos);
			if (result) {
				auto fnFireNugget20 = (CallFunc_WeaponTemplateFireNugget20)vft[8]; // 0x20 / 4
				auto savePointer = pFire->targetPos;
				pFire->targetPos = attack_pos;
				fnFireNugget20(pNugget, fnFireNugget20, pFire);
				pFire->targetPos = savePointer;
			}

			pNuggetList = pNuggetList->next;
		}
		// end
	}

	float __fastcall C_WeaponTemplate_GetCurrentWeaponAttackRange(pC_WeaponTemplate pIn, int useless, void* pGameObject, float negativeHeight)
	{
		auto fnGetRange = (CallFunc_GetCurrentWeaponAttackRange)_call_GetCurrentWeaponAttackRange;
		return fnGetRange(pIn, fnGetRange, pGameObject, negativeHeight);
	}

	float* __fastcall C_WeaponTemplate_GetWeaponNuggetScatter(pC_WeaponTemplate pIn, int useless, float* outPos, pC_GameObject pSelf, pC_GameObject pTarget, pC_Weapon pWeapon, float* inPos)
	{
		/*auto fnGetScatter = (CallFunc_GetWeaponNuggetScatter)_call_GetWeaponNuggetScatter;
		return fnGetScatter(pIn, fnGetScatter, outPos, pSelf, pTarget, pWeapon, inPos);*/

		//volatile int stackDebug = -2;

		float pOutPos[4];
		pOutPos[3] = 0;

		memcpy(pOutPos, inPos, 12);

		float targetScatter = 0;
		if (pTarget) {
			// 00713C06
			auto TargetScatter = C_WeaponTemplate_GetTargetScatterRadiusVsType(pIn, 0, pTarget);
			if (TargetScatter > 0) {
				targetScatter = TargetScatter;
			}
		} else {
			C_System_CheckPositionIsHitTerrain(0, 0, 0, pOutPos);
		}

		auto pWeaponData = pIn->pWeaponData;
		auto scatterRadius = pWeaponData->ScatterRadius;
		auto weaponFlag = pWeaponData->Flags;
		if(weaponFlag[(int)WeaponFlagsType::ORTHOGONAL_SCATTER]){
			// 00713C7E
			// 00713D94 this will crash
			__m128 v_selfPos = _mm_loadu_ps(pSelf->Position);
			__m128 v_targetPos = _mm_loadu_ps(pOutPos);
			__m128 v_deltaPos = _mm_sub_ps(v_targetPos, v_selfPos);
			__m128 v_delta = v_deltaPos;

			float Distance = _mm_dp_ps(v_deltaPos, v_deltaPos, 0b00110001).m128_f32[0];
			if (Distance < 0.0001f) {
				goto overwriteHeight;
			}
			Distance = _mm_sqrt_ss(_mm_set_ss(Distance)).m128_f32[0];

			float scatterLength = pWeaponData->ScatterLength;
			// get horizontal
			if (std::abs(scatterRadius) < 1) {
				scatterRadius = scatterLength;
			}

			if (weaponFlag[(int)WeaponFlagsType::LENGTH_SCATTER]) {
				// to the quadrilateral spread.
				float f_scatter[4];
				f_scatter[1] = getRadomFloatValue(-scatterLength, scatterLength);
				f_scatter[0] = getRadomFloatValue(-scatterRadius, scatterRadius);
				__m128 v_scatter = _mm_loadu_ps(f_scatter);

				// 0,2 is radius. 1,3 is length.
				v_scatter = _mm_shuffle_ps(v_scatter, v_scatter, MY_SHUFFLE(0, 1, 0, 1));
				v_scatter = _mm_div_ps(v_scatter, _mm_set_ps1(Distance));
				// 0,2 is *radius. 1,3 is *length.
				v_delta = _mm_shuffle_ps(v_delta, v_delta, MY_SHUFFLE(1, 0, 0, 1));
				// get -y
				alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
				//__m128 negativeX = _mm_setr_ps(-0.0f, 0.0f, 0.0f, 0.0f);
				v_delta = _mm_xor_ps(v_delta, negativeX);
				// get delta pos
				v_scatter = _mm_mul_ps(v_delta, v_scatter);
				// over is x1+x2, y1+y2
				v_scatter = _mm_hadd_ps(v_scatter, v_scatter);

				// get pos
				__m128 v_attackPos = _mm_add_ps(v_targetPos, v_scatter);
				pOutPos[0] = v_attackPos.m128_f32[0];
				pOutPos[1] = v_attackPos.m128_f32[1];
			}
			else {
				auto randf = getRadomFloatValue(0, 1);
				if (randf >= 0.5) {
					scatterLength = scatterRadius;

					v_delta = _mm_shuffle_ps(v_delta, v_delta, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm));
					// get -y
					alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
					//__m128 negativeX = _mm_setr_ps(-0.0f, 0.0f, 0.0f, 0.0f);
					v_delta = _mm_xor_ps(v_delta, negativeX);
				}

				auto f_scatter = getRadomFloatValue(-scatterLength, scatterLength);
				f_scatter /= Distance;
				__m128 v_attackPos = _mm_mul_ps(v_delta, _mm_set_ps1(f_scatter));
				v_attackPos = _mm_add_ps(v_targetPos, v_attackPos);
				pOutPos[0] = v_attackPos.m128_f32[0];
				pOutPos[1] = v_attackPos.m128_f32[1];
			}
			// end
		}
		else if (weaponFlag[(int)WeaponFlagsType::LENGTH_SCATTER]){

			auto i_curAmmo = pWeapon->CurrentAmmoCount;
			// 0x7FFFFFFF is too large, so max of 10k of ammo will be counted.
			if (i_curAmmo > 9999) {
				goto overwriteHeight;
			}
			float currentAmmo = i_curAmmo;

			// 00713E9E
			__m128 v_selfPos = _mm_loadu_ps(pSelf->Position);
			__m128 v_targetPos = _mm_loadu_ps(pOutPos);
			__m128 v_deltaPos = _mm_sub_ps(v_targetPos, v_selfPos);
			__m128 v_delta = v_deltaPos;
			
			float Distance = _mm_dp_ps(v_deltaPos, v_deltaPos, 0b00110001).m128_f32[0];
			if (Distance < 0.0001f) {
				goto overwriteHeight;
			}

			Distance = _mm_sqrt_ss(_mm_set_ss(Distance)).m128_f32[0];


			if (*(int*)&pWeaponData->ScatterLength) {
				// calculate current distance
				float scatterLength = pWeaponData->ScatterLength * currentAmmo;
				float totalAmmo = pWeaponData->ClipSize;
				float totalDiv = totalAmmo * Distance;

				float Dfactor = scatterLength / totalDiv;
				Dfactor = -Dfactor;
				__m128 v_attackPos = v_delta;
				v_attackPos = _mm_mul_ps(v_attackPos, _mm_set_ps1(Dfactor));
				v_attackPos = _mm_add_ps(v_targetPos, v_attackPos);
				// over calculate

				// get horizontal scatter
				auto f_scatter = getRadomFloatValue(-scatterRadius, scatterRadius);
				if (std::abs(f_scatter) > 1) {
					f_scatter /= Distance;
					__m128 v_horizontal = _mm_shuffle_ps(v_delta, v_delta, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm));
					alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
					v_horizontal = _mm_xor_ps(v_horizontal, negativeX);

					v_horizontal = _mm_mul_ps(v_horizontal, _mm_set_ps1(f_scatter));
					v_attackPos = _mm_add_ps(v_attackPos, v_horizontal);
				}

				pOutPos[0] = v_attackPos.m128_f32[0];
				pOutPos[1] = v_attackPos.m128_f32[1];
			}
			else {
				//to horizontal scatter
				float baseScatter = -scatterRadius;
				float deltaScatter = scatterRadius * 2;
				float ammoFactor = currentAmmo / pWeaponData->ClipSize;

				deltaScatter *= ammoFactor;
				baseScatter += deltaScatter;
				baseScatter /= Distance;

				__m128 v_attackPos = _mm_mul_ps(v_delta, _mm_set_ps1(baseScatter));
				v_attackPos = _mm_shuffle_ps(v_attackPos, v_attackPos, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm));
				// get -y
				alignas(16) const __m128 negativeX = { -0.0f, 0.0f, 0.0f, 0.0f };
				v_attackPos = _mm_xor_ps(v_attackPos, negativeX);

				v_attackPos = _mm_add_ps(v_targetPos, v_attackPos);
				pOutPos[0] = v_attackPos.m128_f32[0];
				pOutPos[1] = v_attackPos.m128_f32[1];
			}
			
			// end
		}
		else {
			// 0071407B
			auto f_radian = getRadomFloatValue(0, 2 * M_PI);
			auto f_scatter = getRadomFloatValue(-scatterRadius, scatterRadius);
			
			float ofsx = std::cos(f_radian) * f_scatter;
			float ofsy = std::sin(f_radian) * f_scatter;

			pOutPos[0] += ofsx;
			pOutPos[1] += ofsy;
			// end
		}

		// 007140F0 check target ai
		// because only bool is returned, it is not used.
		/*
		if (pTarget) {
			auto pThing = (char*)pTarget->pThingTemplate;
			if (pThing[0xC7] & 1) {
				goto overwriteHeight;
			}

			auto pAI = (pM_AIUpdate)pTarget->pAIUpdateModule;
			if (pAI) {
				pAI->vf_table->func274(pAI); // call ra3_1.12.game+2EFC10 
			}
		}*/

	overwriteHeight:
		if (!pWeaponData->NoVictimNeeded) {
			if(pTarget){
				pOutPos[2] = pTarget->Position[2];
			} else {
				float outHeight[2]; // 0 is land, 1 is water
				bool inWater = C_System_CheckPositionInWater(0, 0, pOutPos[0], pOutPos[1], &outHeight[1], &outHeight[0]);
				pOutPos[2] = outHeight[inWater];
			}
			// end
		}

		// support ScatterRadiusVsType
		if (*(int*)&targetScatter) {
			auto f_radian = getRadomFloatValue(0, 2 * M_PI);
			float ofsx = std::cos(f_radian) * targetScatter;
			float ofsy = std::sin(f_radian) * targetScatter;
			pOutPos[0] += ofsx;
			pOutPos[1] += ofsy;
		}

		__m128 v_pos = _mm_loadu_ps(pOutPos);
		v_pos = _mm_min_ps(v_pos, _mm_set_ps1(9900.0f));
		outPos[0] = v_pos.m128_f32[0];
		outPos[1] = v_pos.m128_f32[1];
		outPos[2] = v_pos.m128_f32[2];

		return outPos;
	}

	float* __fastcall C_WeaponTemplate_GetWeaponNuggetScatter_GetPos(pC_WeaponTemplate pIn, int useless, float* outPos, pC_GameObject pSelf, pC_GameObject pTarget, pC_Weapon pWeapon, float* inPos)
	{
		memcpy(outPos, inPos, 12);
		if (pTarget) {
			auto TargetScatter = C_WeaponTemplate_GetTargetScatterRadiusVsType(pIn, 0, pTarget);

			if (TargetScatter > 0) {
				auto f_radian = getRadomFloatValue(0, 2 * M_PI);
				float ofsx = std::cos(f_radian) * TargetScatter;
				float ofsy = std::sin(f_radian) * TargetScatter;
				outPos[0] += ofsx;
				outPos[1] += ofsy;
			}

			outPos[2] = pTarget->Position[2];
		}
		return outPos;
	}

	float* __fastcall C_WeaponTemplate_GetWeaponScatterFromContactPoints(pC_WeaponTemplate pIn, int useless, float* outPos, pC_GameObject pSelf, pC_GameObject pTarget, pC_Weapon pWeapon, float* inPos)
	{
		auto pData = pIn->pWeaponData;
		if (pData->ScatterIndependently) {
			return C_WeaponTemplate_GetWeaponNuggetScatter_GetPos(pIn, 0, outPos, pSelf, pTarget, pWeapon, inPos);
		} else {
			return C_WeaponTemplate_GetWeaponNuggetScatter(pIn, 0, outPos, pSelf, pTarget, pWeapon, inPos);
		}
	}

	float __fastcall C_WeaponTemplate_GetTargetScatterRadiusVsType(pC_WeaponTemplate pIn, int useless, pC_GameObject pGO)
	{
		auto fnGetScatter = (CallFunc_GetTargetScatterRadiusVsType)_call_GetTargetScatterRadiusVsType;
		return fnGetScatter(pIn, fnGetScatter, pGO);
	}

// end namespace RA3::Core
}