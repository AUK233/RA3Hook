#include "../pch.h"

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"

#include "C_ThingTemplate.h"

namespace RA3::Core {


	uintptr_t _F_ttCheckKindOfOnSelection = 0xA86A98;
	uintptr_t _Ret_ttCheckKindOfOnSelection = 0xA86A98 + 49;

	void __fastcall C_ThingTemplate_Hook()
	{
		hookGameBlock((void*)_F_ttCheckKindOfOnSelection, (uintptr_t)C_ThingTemplate_CheckKindOfOnSelectionASM);
		WriteHookToProcess((void*)(_F_ttCheckKindOfOnSelection + 5), (void*)&nop1, 1U);
	}

	void __fastcall C_ThingTemplate_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_ttCheckKindOfOnSelection = 0xA1B5C8;
			_Ret_ttCheckKindOfOnSelection = 0xA1B5C8 + 49;
		}
	}

	__declspec(naked) void __fastcall C_ThingTemplate_CheckKindOfOnSelectionASM()
	{
		__asm {
			mov ecx, esi
			call C_ThingTemplate_CheckKindOfOnSelectionCPP
			test al, al
			jz NoSelect
			jmp _Ret_ttCheckKindOfOnSelection
		}

	NoSelect:
		__asm {
			pop edi
			pop esi
			pop ebp
			//xor al, al
			pop ebx
			ret
		}
	}

	bool __fastcall C_ThingTemplate_CheckKindOfOnSelectionCPP(pC_ThingTemplate pIn)
	{
		if (pIn->KindOf[(int)KindOfType::FS_BASE_DEFENSE]) {
			return true;
		}

		if (pIn->KindOf[(int)KindOfType::CYCLE_SELECTION]) {
			return true;
		}

		if (pIn->KindOf[(int)KindOfType::STRUCTURE]) {
			return false;
		}

		return true;
	}

// end namespace RA3::Core
}
