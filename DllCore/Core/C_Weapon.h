#pragma once
#include "C_GameObject.h"

typedef struct C_Weapon_t {
	void* vf_table;
	void* pWeaponTemplate; // yeah
	char pad08[0xC];
	UINT32 WeaponOrdering;
	UINT32 WeaponSlotId; // which slot does it belong in.
	int CurrentAmmoCount;
	UINT32 Timer20; // Maybe it is weapon initialization time.
	int _int24; // If it < 0, firing state will be set to 0.
	int CurrentWeaponLaunchBoneIndex;
	char pad2C[0x14];
	float FirePosNoTarget[3]; // force attack ground
	float FirePosInTarget[3]; // when has target
	char pad58[8];
	bool bIsFiring; // is in firing state
	bool pad61;
	bool bHasTarget; // when fire to a target, is true
	bool bool63; // related to target
	char pad64[8];
	float FireOutPos[3]; // copy from fire pos, maybe is previous
}*pC_Weapon;
#if 1
	static_assert(offsetof(C_Weapon_t, CurrentAmmoCount) == 0x1C);
	static_assert(offsetof(C_Weapon_t, FirePosNoTarget) == 0x40);
	static_assert(offsetof(C_Weapon_t, FirePosInTarget) == 0x4C);
	static_assert(offsetof(C_Weapon_t, bIsFiring) == 0x60);
	static_assert(offsetof(C_Weapon_t, bHasTarget) == 0x62);
	static_assert(offsetof(C_Weapon_t, FireOutPos) == 0x6C);
#endif

enum class WeaponFlagsType : int
{
	NONE,
	SYNC_AMMO_ON_ACTIVATE,
	ORTHOGONAL_SCATTER,
	LENGTH_SCATTER,
	ATTACK_NEEDS_LINE_OF_SIGHT,
	RELOAD_WHEN_ATTACK_STOPS,
	IGNORE_TARGET_AS_OBSTACLE,
	NOT_ATTRACTED_BY_MAGNETS,
	CRUSH_VEHICLE,
	FORCE_EMPTY_ENTIRE_CLIP,
	IGNORE_WALL_RELATIONSHIP,
	IGNORE_ENCLOSURE_CHECK,
	ALL,
};

enum class WeaponPrefireType : int
{
	PER_SHOT = 0,
	PER_CLIP = 1,
	PER_BURST = 2,
	PER_TARGET = 3,
	PER_POSITION = 4,
};

enum class WeaponReAcquireDetailType : int
{
	PRE_SHOT = 0,
	PER_SHOT = 1,
	PER_CLIP = 2,
	PER_ATTACK = 3,
	PRE_FIRE = 4,
	POST_FIRE = 5,
};

enum class WeaponReloadType : int
{
	AUTO = 0,
	NONE = 1,
	RETURN_TO_BASE = 2,
};

typedef struct Data_WeaponTemplate_t {
	baseBinDataHeader_t* typeInfo;
    float AttackRange, MinimumAttackRange;
    float rangeBonusMinHeight, rangeBonus, rangeBonusPerFoot;
    float requestAssistRange;
    float acceptableAimDelta;
    float aimDirection;
    float ScatterRadius, ScatterLength; // ScatterRadius must > 0, otherwise it will not be activated.
    float WeaponSpeed, MinWeaponSpeed, MaxWeaponSpeed;
    float IdleAfterFiringDelaySeconds, HoldAfterFiringDelaySeconds;
	float WeaponRecoil;
	float MinTargetPitch, MaxTargetPitch; // Radians
	char pad4C[0x24];
	int ClipSize;
	int ContinuousFireOne, ContinuousFireTwo;
	float ContinuousFireCoastSeconds;
	float AutoReloadWhenIdleSeconds;
	int ShotsPerBarrel;
	char pad88[0x24];
	std::bitset<(size_t)WeaponFlagsType::ALL> Flags; // WeaponFlagsType
	WeaponPrefireType PreAttackType;
	WeaponReAcquireDetailType ReAcquireDetailType;
	WeaponReloadType AutoReloadsClip;
    char padBC[0x68];
    bool ScatterIndependently, ScaleWeaponSpeed, IgnoresContactPoints, ScaleAttackRangeByAmmoRemaining;
    bool CanBeDodged, HoldDuringReload, CanFireWhileMoving, FiringLoopSoundContinuesDuringReload;
    bool DamageDealtAtSelfPosition, CheckStatusFlagsInRangeChecks, ProjectileSelf, MeleeWeapon;
    bool ChaseWeapon, LeechRangeWeapon, HitStoredTarget, CapableOfFollowingWaypoints;
	bool ShowsAmmoPips, AllowAttackGarrisonedBldgs, PlayFXWhenStealthed, IsAimingWeapon;
	bool NoVictimNeeded, PassengerProportionalAttack, FinishAttackOnceStarted, CannotTargetCastleVictims;
	bool RequireFollowThru, ShareTimers, ShouldPlayUnderAttackEvaEvent, InstantLoadClipOnActivate;
	bool LockWhenUsing, BombardType, UseInnateAttributes, StopFiringOnCanBeInvisible;
	bool ContactWeapon, UseCenterForRangeCheck, RevealShroudOnFire, ShouldPlayTargetDeadEvaEvent;
}*pData_WeaponTemplate;
#if 1
	static_assert(offsetof(Data_WeaponTemplate_t, AttackRange) == 4);
	static_assert(offsetof(Data_WeaponTemplate_t, ScatterRadius) == 0x24);
	static_assert(offsetof(Data_WeaponTemplate_t, ClipSize) == 0x70);
	static_assert(offsetof(Data_WeaponTemplate_t, Flags) == 0xAC);
	static_assert(offsetof(Data_WeaponTemplate_t, AutoReloadsClip) == 0xB8);
	static_assert(offsetof(Data_WeaponTemplate_t, ScatterIndependently) == 0x124);
	static_assert(offsetof(Data_WeaponTemplate_t, DamageDealtAtSelfPosition) == 0x12C);
	static_assert(offsetof(Data_WeaponTemplate_t, ShowsAmmoPips) == 0x134);
	static_assert(offsetof(Data_WeaponTemplate_t, IsAimingWeapon) == 0x137);
	static_assert(offsetof(Data_WeaponTemplate_t, NoVictimNeeded) == 0x138);
	static_assert(sizeof(Data_WeaponTemplate_t) == 0x148);
#endif

typedef struct C_WeaponTemplate_t {
	void* vf_table;
	pData_WeaponTemplate pWeaponData; // static data, cannot be modified.
	void* pNuggetList;
}*pC_WeaponTemplate;
#if 1
	static_assert(offsetof(C_WeaponTemplate_t, pNuggetList) == 8);
#endif

typedef struct WeaponFireInStack_t {
	pC_WeaponTemplate pWeaponTemplate;
	pC_GameObject pSelf;
	float* targetPos;
	int padC[3];
	pC_Weapon pWeapon;
	pC_GameObject pTarget;
	int pad20;
	int isForcedAttack; // yeah, it is 4-byte, but it only 0 and 1.
	int pad28;
	float* realTargetPos; // FireFX will use this.
}*pWeaponFireInStack;
#if 1
	static_assert(offsetof(WeaponFireInStack_t, pSelf) == 4);
	static_assert(offsetof(WeaponFireInStack_t, targetPos) == 8);
	static_assert(offsetof(WeaponFireInStack_t, pWeapon) == 0x18);
	static_assert(offsetof(WeaponFireInStack_t, pTarget) == 0x1C);
	static_assert(offsetof(WeaponFireInStack_t, isForcedAttack) == 0x24);
	static_assert(offsetof(WeaponFireInStack_t, realTargetPos) == 0x2C);
#endif

typedef float(__fastcall* CallFunc_GetCurrentWeaponAttackRange)(pC_WeaponTemplate pIn, void* callFunc, void* pGameObject, float negativeHeight);
typedef float*(__fastcall* CallFunc_GetWeaponNuggetScatter)(pC_WeaponTemplate pIn, void* callFunc, float* outPos, pC_GameObject pSelf, pC_GameObject pTarget, pC_Weapon pWeapon, float* inPos);
typedef float(__fastcall* CallFunc_GetTargetScatterRadiusVsType)(pC_WeaponTemplate pIn, void* callFunc, pC_GameObject pGO);

typedef bool(__fastcall* CallFunc_WeaponTemplateFireNugget0C)(int* pIn, void* callFunc, void* WeaponFireInStack, float* pos);
typedef void(__fastcall* CallFunc_WeaponTemplateFireNugget20)(int* pIn, void* callFunc, void* WeaponFireInStack);

namespace RA3::Core {
	void __fastcall C_Weapon_Hook();
	void __fastcall C_Weapon_Initialize(uintptr_t hmodEXE, int isNewSteam);

	// Let "ShowsAmmoPips" work
	void __fastcall C_Weapon_ShowsAmmoPipsASM();
	void __fastcall C_Weapon_SetupPreAttackWeaponPosASM();

	void __fastcall C_WeaponTemplate_SetAttackPositionASM();
	void __fastcall C_WeaponTemplate_SetAttackPositionToRangeCPP(pC_WeaponTemplate pIn, PBYTE pESI, float* targetPos);

	void __fastcall C_WeaponTemplate_FireNuggetsASM();
	void __fastcall C_WeaponTemplate_FireNuggetsCPP(void* psp, pWeaponFireInStack pFire);

	float __fastcall C_WeaponTemplate_GetCurrentWeaponAttackRange(pC_WeaponTemplate pIn, int useless, void* pGameObject, float negativeHeight);
	float* __fastcall C_WeaponTemplate_GetWeaponNuggetScatter(pC_WeaponTemplate pIn, int useless, float* outPos, pC_GameObject pSelf, pC_GameObject pTarget, pC_Weapon pWeapon, float* inPos);
	float __fastcall C_WeaponTemplate_GetTargetScatterRadiusVsType(pC_WeaponTemplate pIn, int useless, pC_GameObject pGO);
}