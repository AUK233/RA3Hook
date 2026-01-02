#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_Subsystem.h"

namespace RA3::Core {

	uintptr_t _F_SpecialPowerPublicTimerInit = 0xABE46D;
	uintptr_t _F_UpdateSpecialPowerPublicTimerList = 0xACAC9F;
	uintptr_t _Ret_UpdateSpecialPowerPublicTimerList = 0xACAC9F + 31;

	auto _call_SpecialPowerPublicTimerUpdate = (CallFunc_SetSpecialPowerToPublicTimer)0xABE510;
	auto _call_SpecialPowerPublicTimerRemove = (CallFunc_SetSpecialPowerToPublicTimer)0xAB9350;
	//auto _call_SetSpecialPowerToPublicTimer = (CallFunc_SetSpecialPowerToPublicTimer)0xAB9280;
	
	void __fastcall C_Subsystem_Hook() {
		// now +35h as a flag for display (no!)
		BYTE SpecialPowerPublicTimerInit[]= {
			0xC7, 0x46, 0x34, 0x00, 0x00, 0x00, 0x00, // mov dword ptr [esi+34], 0
			0x5F,                                     // pop edi
			0x8B, 0xC6,                               // mov eax, esi
			0x5E,                                     // pop esi
			0xC2, 0x14, 0x00,                         // ret 14
			0xCC, 0xCC, 0xCC, 0xCC
		};
		WriteHookToProcess((void*)_F_SpecialPowerPublicTimerInit, (void*)SpecialPowerPublicTimerInit, 18);

		//
		hookGameBlock((void*)_F_UpdateSpecialPowerPublicTimerList, (uintptr_t)C_Skirmish_UpdateSpecialPowerPublicTimerListASM);
		WriteHookToProcess((void*)(_F_UpdateSpecialPowerPublicTimerList + 5), (void*)&nop1, 1U);
	}

	void __fastcall C_Subsystem_Initialize(uintptr_t hmodEXE, int isNewSteam) {
		if (isNewSteam){
			_F_SpecialPowerPublicTimerInit = 0xA5309D;
			_F_UpdateSpecialPowerPublicTimerList = 0xA5FC4F;
			_Ret_UpdateSpecialPowerPublicTimerList = 0xA5FC4F + 31;
			_call_SpecialPowerPublicTimerUpdate = (CallFunc_SetSpecialPowerToPublicTimer)0xA53140;
			_call_SpecialPowerPublicTimerRemove = (CallFunc_SetSpecialPowerToPublicTimer)0xA4DFA0;
		}
	}

	__declspec(naked) void __fastcall C_Skirmish_UpdateSpecialPowerPublicTimerListASM()
	{
		__asm {
			mov edi, [esi + 8]
			mov esi, [esi + 4]
			xor ebx, ebx
			cmp esi, edi
			je ReturnOriginal
			align 16
		}
	toLoop:
		__asm {
			mov ecx, [esi]
			// check total
			cmp ebx, 6
			jae checkExist
			call _call_SpecialPowerPublicTimerUpdate
			mov ecx, [esi]
			cmp dword ptr[ecx + 0x30], 0
			je toNoAdd
			inc ebx
			jmp toNoAdd
		}
	checkExist:
		__asm {
			cmp dword ptr[ecx + 0x30], 0
			je toNoAdd
			call _call_SpecialPowerPublicTimerRemove
		}
	toNoAdd:
		__asm {
			add esi, 4
			cmp esi, edi
			jne toLoop
		}
	ReturnOriginal:
		__asm {
			jmp _Ret_UpdateSpecialPowerPublicTimerList
		}
	}

	// end namespace RA3::Core
}
