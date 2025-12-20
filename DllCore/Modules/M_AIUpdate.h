#pragma once

typedef bool(__fastcall* CallFunc_AIUpdateVFT274)(void* pIn);

typedef struct M_AIUpdate_VFT_t {
	char pad[0x274];
	CallFunc_AIUpdateVFT274 func274;
} *pM_AIUpdate_VFT;
#if 1
	static_assert(offsetof(M_AIUpdate_VFT_t, func274) == 0x274);
#endif

typedef struct M_AIUpdate_t {
	pM_AIUpdate_VFT vf_table;
	void* pModuleData;
	void* pGameObject;
	unsigned char pad[0x30];
	void* pModuleStateMachine;
} *pM_AIUpdate;
#if 1
	static_assert(offsetof(M_AIUpdate_t, pGameObject) == 8);
	static_assert(offsetof(M_AIUpdate_t, pModuleStateMachine) == 0x3C);
#endif
