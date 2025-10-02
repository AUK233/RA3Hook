#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Base/SSE.hpp"
#include "C_AttributeModifier.h"

namespace RA3::Core {

	// this is <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
	uintptr_t _F_AttributeModifierBuffState1 = 0x4DAABD;

	uintptr_t _F_AttributeModifierT18CheckBuff = 0x84D5C9;

	void __fastcall C_AttributeModifier_Hook()
	{
		// <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
		hookGameCall((void*)_F_AttributeModifierBuffState1, (uintptr_t)C_AttributeModifier_BuffState1ASM);
		WriteHookToProcess((void*)(_F_AttributeModifierBuffState1 + 5), (void*)&nop1, 1U);

		// use always -1 instead of return value checks
		WriteHookToProcess((void*)_F_AttributeModifierT18CheckBuff, (void*)&nop7, 7U);
		WriteHookToProcess((void*)(_F_AttributeModifierT18CheckBuff + 7 + 5), (void*)&nop6, 6U);
		hookGameCall((void*)(_F_AttributeModifierT18CheckBuff + 7), (uintptr_t)C_AttributeModifier_T18CheckBuffASM);
	}

	void __fastcall C_AttributeModifier_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam) {
			_F_AttributeModifierBuffState1 = hmodEXE + 0x11C4DD;
			_F_AttributeModifierT18CheckBuff = 0x88B609;
		}
	}

	__declspec(naked) void __fastcall C_AttributeModifier_BuffState1ASM()
	{
		__asm {
			// check it is BOUNTY_PERCENTAGE
			cmp dword ptr[esp + 0x24 + 4], 18
			jne returnOld
			// check it is 1.0f
			/*cmp dword ptr [esp + 0x18 + 4], 0x3F800000
			je returnOld*/
			addss xmm0, [esp + 0x18 + 4]
			ret
			// they need +4 because of esp has decreased by 4
		returnOld:
			mulss xmm0, [esp + 0x18 + 4]
			ret
		}
	}

	const float NegativeOne = -1.0f;
	__declspec(naked) void __fastcall C_AttributeModifier_T18CheckBuffASM()
	{
		__asm {
			// they need +4 because of esp has decreased by 4
			movss xmm0, [esp + 0x20 + 4]
			xorps xmm1, xmm1
			addss xmm0, NegativeOne
			maxss xmm0, xmm1
			movss [esp + 0x20 + 4], xmm0
			ret
		}
	}

// end namespace RA3::Core
}
