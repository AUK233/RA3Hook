#pragma once
#include "commonStruct.h"

inputSettingINFO inputSetting;
uintptr_t hmodEXE = 0x00400000;
int InitializationCore = 0;
// 
uintptr_t _F_SyncSet = 0;
uintptr_t _F_SweepLaser01 = 0;
uintptr_t _F_ActivateLaser = 0;
uintptr_t _Ret_ActivateLaser = 0;
uintptr_t _F_BloomOpen = 0;
uintptr_t _Ret_BloomOpen = 0;
uintptr_t _F_ShowAmmo = 0;
uintptr_t _F_WeaponReloadActive = 0;
uintptr_t _F_WeaponReloadTimeCount = 0;
// this is <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
uintptr_t _F_AttributeModifierT18Buff = 0;
// temp
uintptr_t ofs32C8C6 = 0;

// loading file
uintptr_t pPlayerTechStore[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
// load PlayerTechStoreTemplate
uintptr_t _F_PlayerTechStoreL = 0;
uintptr_t _Ret_PlayerTechStoreL = 0;
// read PlayerTechStoreTemplate
uintptr_t _F_PlayerTechStoreR1 = 0;
uintptr_t _F_PlayerTechStoreR2 = 0;
uintptr_t _F_PlayerTechStoreR3 = 0;
