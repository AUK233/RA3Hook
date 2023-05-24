#pragma once

struct inputSettingINFO
{
	unsigned char cpuLimit = 0;
	unsigned char CheckBloom = 0;
	unsigned char align[2];
} inputSetting;

int noBloomSet = 0;

uintptr_t hmodEXE = 0;
// 
uintptr_t _F_SyncSet = 0;
uintptr_t _F_SweepLaser01 = 0;
uintptr_t _F_BloomOpen = 0;
uintptr_t _Ret_BloomOpen = 0;
// temp
uintptr_t ofs32C8C6 = 0;
