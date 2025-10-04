#pragma once

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

typedef void(__fastcall* void_fastcallNoParameter)(void*);
typedef int(__fastcall* int_fastcallNoParameter)(void*);

typedef void(__fastcall* void_thiscallHasParameter1)(void* ecx, int, int a1);
typedef void(__fastcall* void_thiscallHasParameter2)(void* ecx, int, int a1, int a2);

typedef void(__fastcall* void_thiscallHas1Float)(void* ecx, int, float scale);
