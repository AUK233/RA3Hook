#pragma once
#include <bitset>

struct baseBinDataHeader_t {
	int ModuleHash, IDHash;
};

struct baseCommonModule_t {
	void* vf_table;
	void* pModuleData; // static data, cannot be modified.
	void* pGameObject;
	void* PAD0C; // always GetRefCount(void)?
	void* vft_module;
	int i_count14, i_count18, i_status, i_0x20;
};

struct Matrix3D_t{
	float m0[3], x;
	float m1[3], y;
	float m2[3], z;
};

typedef void(__fastcall* void_fastcallNoParameter)(void*);
typedef int(__fastcall* int_fastcallNoParameter)(void*);

typedef void(__fastcall* void_thiscallHasParameter1)(void* ecx, void* callFunc, int a1);
typedef void(__fastcall* void_thiscallHasParameter2)(void* ecx, void* callFunc, int a1, int a2);

typedef void(__fastcall* void_thiscallHas1Float)(void* ecx, void* callFunc, float scale);

// note: this ecx is in ra3_1.12.game+3E308B
typedef void*(__fastcall* CallFunc_InitializeModule)(void* pECX, void* callFunc, void* pA1, void* pModuleData);
