#pragma once

// ptr is pointer to PlayerTechStoreTemplate
// pptr is this pointer's pointer
uintptr_t __fastcall LoadPlayerTechStoreCPP(uintptr_t ptr, uintptr_t pptr);
void __fastcall LoadPlayerTechStoreASM();
// Get index by faction name
UINT32 __fastcall ReadPlayerFactionTypeCPP(uintptr_t ptr);
//
void __fastcall ReadPlayerFactionIconASM();
uintptr_t __fastcall LoadNewPlayerFactionIconCPP(void* ptr);
void __fastcall GetUnitOverlayIconSettingsASM();
