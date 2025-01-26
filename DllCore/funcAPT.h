#pragma once

namespace RA3::APT {
//
void __fastcall UpdateCheckAPTHashA31A11ASM();
//
void __fastcall PushTheRuleTextToOnlineChatBox(ruleDataPointer* pRule, LPCSTR pText);
void __fastcall UpdateToggleRandomCrateOptionsCPP(ruleDataPointer* prule);
void __fastcall UpdateToggleRandomCrateOptionsASM();
void __fastcall UpdateRandomCrateCreateFunctionASM();
//
void __fastcall UpdateGameObjectRadarColorASM();


void __fastcall InitializeHookAptFunctionUpdateOrigin(uintptr_t hmodEXE);
void __fastcall HookAptFunctionUpdate();
}
