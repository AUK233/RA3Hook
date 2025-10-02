#pragma once

namespace RA3::Weapon {

// Let "ShowsAmmoPips" work
void __fastcall ShowsAmmoPipsASM();
//
void __fastcall WeaponReloadActiveASM();
//
void __fastcall WeaponReloadTimeCountASM();
// Fix Weapon Scatter Radius
void __fastcall WeaponScatterRadiusFixASM1();
void __fastcall WeaponScatterRadiusFixASM2();
// Weapon Scatter Radius
void __fastcall WeaponScatterRadiusCPP(float* pRadian, float* pPos);
void __fastcall WeaponScatterRadiusASM();
// EMP Time To Unit Shield Damage
bool __fastcall CheckNoEMPInUnitShieldASM(void* pShieldBody);

void __fastcall InitializeHookWeaponFunctionUpdateOrigin(uintptr_t hmodEXE);
void __fastcall HookWeaponFunctionUpdate();
}
