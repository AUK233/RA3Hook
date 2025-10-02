#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct C_Weapon_t {
	void* vf_table;
	void* pWeaponTemplate; // yeah
	char pad08[0x38];
	float FirePosNoTarget[3]; // force attack ground
	float FirePosInTarget[3]; // when has target
	char pad58[0xA];
	bool bHasTarget; // when fire to a target, is true
	char pad63[9];
	float FireOutPos[3]; // copy from fire pos, maybe is previous
}*pC_Weapon;

namespace RA3::Core {
	void __fastcall C_Weapon_Hook();
	void __fastcall C_Weapon_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall C_Weapon_SetupPreAttackWeaponPosASM();
}