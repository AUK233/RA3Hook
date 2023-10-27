#pragma once
#include "commonStruct.h"

inputSettingINFO inputSetting;
uintptr_t hmodEXE = 0x00400000;
int InitializationCore = 0;
// 
uintptr_t _F_SyncSet = 0;
uintptr_t _F_SweepLaser01 = 0;
uintptr_t _F_SweepLaserActivate = 0;
uintptr_t _Ret_SweepLaserActivate = 0;
uintptr_t _F_ActivateLaser = 0;
uintptr_t _Ret_ActivateLaser = 0;
uintptr_t _F_BloomOpen = 0;
uintptr_t _Ret_BloomOpen = 0;
uintptr_t _F_ShowAmmo = 0;
uintptr_t _F_WeaponReloadActive = 0;
uintptr_t _F_WeaponReloadTimeCount = 0;
// this is <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
uintptr_t _F_AttributeModifierT18Buff = 0;
// Fix weapon scatter radius (now it is useless)
uintptr_t _F_WeaponScatterRadius = 0;
uintptr_t _Ret_WeaponScatterRadius = 0;
uintptr_t _F_CallRandomRadius = 0;
// Fix weapon scatter radius
uintptr_t _F_WeaponScatterRadius1 = 0;
uintptr_t _F_WeaponScatterRadius2 = 0;
// thiscall KillGameObject(void* GameObject, int unk1, UINT deathType, int unk2)
uintptr_t _F_CallKillGameObject = 0;
uintptr_t _F_KillTibCrystalWhenEmpty1 = 0;
uintptr_t _F_KillTibCrystalWhenEmpty2 = 0;
// temp
uintptr_t ofs32C8C6 = 0;

// loading file
uintptr_t pPlayerTechStore[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
UINT32 playerNameHashToIndex[9][2] = {
	{2293878703U, 4},  // 0Allies1
	{2382707485U, 5},  // 2Allies9
	{2389174180U, 6},  // 3Allies
	{3111316061U, 7},  // 1Soviet4
	{3154887896U, 8},  // 0Soviet5
	{3237873227U, 9},  // 3Soviet7
	{1758239350U, 10}, // 3Japan2
	{1895103435U, 11}, // 1Japan2
	{1959656780U, 12}  // Japan3
};
UINT32 playerTechStoreToIndex[9][2] = {
	{3267362061U, 4},  // PlayerTechStore_Allied1
	{2350054027U, 5},  // PlayerTechStore_Allied2
	{1022500346U, 6},  // PlayerTechStore_Allied3
	{1606882289U, 7},  // PlayerTechStore_Soviet1
	{55352523U,   8},  // PlayerTechStore_Soviet2
	{3838998804U, 9},  // PlayerTechStore_Soviet3
	{3587428384U, 10}, // PlayerTechStore_Japan1
	{3494380189U, 11}, // PlayerTechStore_Japan2
	{1268198533U, 12}  // PlayerTechStore_Japan3
};
// load PlayerTechStoreTemplate
uintptr_t _F_PlayerTechStoreL = 0;
uintptr_t _Ret_PlayerTechStoreL = 0;
// read PlayerTechStoreTemplate
uintptr_t _F_PlayerTechStoreR1 = 0;
uintptr_t _F_PlayerTechStoreR2 = 0;
uintptr_t _F_PlayerTechStoreR3 = 0;
// read player faction icon
uintptr_t pPlayerFactionIcon[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
uintptr_t _F_ReadPlayerFactionType = 0;
uintptr_t _F_ReadPlayerFactionIcon = 0;
uintptr_t _Ret_ReadPlayerFactionIcon = 0;
uintptr_t _F_GetUnitOverlayIconSettings = 0;
