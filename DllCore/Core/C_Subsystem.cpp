#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_Subsystem.h"

namespace RA3::Core {
	uintptr_t _F_SetSkirmishStartCash = 0x850577;

	void __fastcall C_Subsystem_Hook() {

		hookGameCall((void*)_F_SetSkirmishStartCash, (uintptr_t)C_Subsystem_SetSkirmishStartCashASM);
		WriteHookToProcess((void*)(_F_SetSkirmishStartCash + 5), (void*)&nop1, 1U);
	}

	void __fastcall C_Subsystem_Initialize(uintptr_t hmodEXE, int isNewSteam) {
		if (isNewSteam){
			_F_SetSkirmishStartCash = 0x88E5B7;
		}
	}

	__declspec(naked) void __fastcall C_Subsystem_SetSkirmishStartCashASM()
	{
		// ebx is current cash
		__asm {
			imul edx, [edi + 8], 10 // edx = ChoiceStepAmount * 10
			cmp ebx, edx
			jge HighAdd
			add ebx, [edi + 8]
			cmp ebx, [edi + 4]
			ret
		}
		// If it exceeds 10x, it will increase by 10x.
	HighAdd:
		__asm {
			add ebx, edx
			cmp ebx, [edi + 4]
			ret
		}
	}

	// end namespace RA3::Core
}
