#pragma once

// Let "ShowsAmmoPips" work
void __fastcall ShowsAmmoPipsASM();
//
void __fastcall WeaponReloadActiveASM();
//
void __fastcall WeaponReloadTimeCountASM();
// BOUNTY_PERCENTAGE is 18
void __fastcall AttributeModifierNo18BuffASM();
// Fix Weapon Scatter Radius
void __fastcall WeaponScatterRadiusFixASM1();
void __fastcall WeaponScatterRadiusFixASM2();
// Weapon Scatter Radius
void __fastcall WeaponScatterRadiusCPP(float* pRadian, float* pPos);
//
void __fastcall WeaponScatterRadiusASM();
