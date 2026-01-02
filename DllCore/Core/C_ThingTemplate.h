#pragma once
#include "../Base/GlobalParameter.hpp"

typedef struct C_ThingTemplate_t {
	void* vf_table;
	char pad04[0xBC];
	std::bitset<(size_t)KindOfType::all_count> KindOf;
}*pC_ThingTemplate;
#if 1
static_assert(offsetof(C_ThingTemplate_t, KindOf) == 0xC0);
#endif

namespace RA3::Core {
	void __fastcall C_ThingTemplate_Hook();
	void __fastcall C_ThingTemplate_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall C_ThingTemplate_CheckKindOfOnSelectionASM();
	// 0 is no select, 1 is select
	bool __fastcall C_ThingTemplate_CheckKindOfOnSelectionCPP(pC_ThingTemplate pIn);
}
