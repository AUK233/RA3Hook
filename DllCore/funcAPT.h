#pragma once

namespace RA3::APT {
void __fastcall PushTheRuleTextToOnlineChatBox(ruleDataPointer* pRule, LPCSTR pText);
void __fastcall UpdateToggleRandomCrateOptionsCPP(ruleDataPointer* prule);
void __fastcall UpdateToggleRandomCrateOptionsASM();
void __fastcall UpdateRandomCrateCreateFunctionASM();

void __fastcall HookAptFunctionUpdate();
}
