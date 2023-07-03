#pragma once

struct inputSettingINFO
{
	unsigned char cpuLimit = 0;
	unsigned char CheckBloom = 0;
	unsigned char setDebug = 0;
	unsigned char align = 0;
} inputSetting;

int noBloomSet = 0;

uintptr_t hmodEXE = 0;
// 
uintptr_t _F_SyncSet = 0;
uintptr_t _F_SweepLaser01 = 0;
uintptr_t _F_ActivateLaser = 0;
uintptr_t _Ret_ActivateLaser = 0;
uintptr_t _F_BloomOpen = 0;
uintptr_t _Ret_BloomOpen = 0;
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
