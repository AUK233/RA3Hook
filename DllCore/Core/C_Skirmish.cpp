#include "../pch.h"

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "C_Skirmish.h"

namespace RA3::Core {

	uintptr_t _F_SetSkirmishStartCash = 0x850577;
	uintptr_t _F_SetSkirmishAdditionalPlayers = 0x8686CC;

	void __fastcall C_Skirmish_Hook()
	{

		hookGameCall((void*)_F_SetSkirmishStartCash, (uintptr_t)C_Skirmish_SetStartCashASM);
		WriteHookToProcess((void*)(_F_SetSkirmishStartCash + 5), (void*)&nop1, 1U);

		// ==============================================
		BYTE SetSkirmishAdditionalPlayers1[] = {
			0x89, 0xEA, // mov edx, ebp
			0x89, 0xF1  // mov ecx, esi
		};
		BYTE SetSkirmishAdditionalPlayers2[] = {
			0xEB, 0x18,
			0x90
		};
		WriteHookToProcess((void*)_F_SetSkirmishAdditionalPlayers, SetSkirmishAdditionalPlayers1, 4);
		hookGameCall((void*)(_F_SetSkirmishAdditionalPlayers + 4), (uintptr_t)C_Skirmish_SetAdditionalPlayersCPP);
		WriteHookToProcess((void*)(_F_SetSkirmishAdditionalPlayers + 4 + 5), SetSkirmishAdditionalPlayers2, 3);
		// ==============================================
	}

	void __fastcall C_Skirmish_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_SetSkirmishStartCash = 0x88E5B7;
			_F_SetSkirmishAdditionalPlayers = 0x8A61FC;
		}
	}

	__declspec(naked) void __fastcall C_Skirmish_SetStartCashASM()
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

	int __fastcall C_Skirmish_SetAdditionalPlayersCPP(const char* inPlayerName, const int inSize)
	{
		int curSize = inSize - 10;
		if (curSize) {
			return curSize;
		}

		static const char* vstr_PlyrCreep[5] = {
			"PlyrCreeps",
			"PlyrCreep1", "PlyrCreep2",
			"PlyrCreep3", "PlyrCreep4"
		};

		int result = -2;
		for (int i = 0; i < 5; i++) {
			result = memcmp(inPlayerName, vstr_PlyrCreep[i], 10);
			if (!result) {
				return 0;
			}
		}

		return result;
	}

// end namespace RA3::Core
}
