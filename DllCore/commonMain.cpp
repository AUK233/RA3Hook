#include "pch.h"
#include "commonData.h"
#include "commonAddr.h"
#include "utiliy.h"
#include <string>

//#include "commonEnum.hpp"

#include "LuaEngine.h"

#include "funcPlayer.h"
#include "funcWeapon.h"
#include "funcMLaser.h"
#include "funcOther.h"
#include "funcAIUpdate.h"
#include "commonMain.h"

void __fastcall hookFunctionGroup()
{
	//hookGameBlock((void*)(hmodEXE + 0x2E5EC3), (uintptr_t)shdiahdiwaw);
	//
	HookAIUpdateModule();
	RA3::LuaEngine::HookLuaEngine();
	// Load PlayerTechStoreTemplate
	hookGameBlock((void*)_F_PlayerTechStoreL, (uintptr_t)LoadPlayerTechStoreASM);
	// Read PlayerTechStoreTemplate
	uintptr_t pPTS = (uintptr_t)&pPlayerTechStore;
	WriteHookToProcess((void*)_F_PlayerTechStoreR3, &pPTS, 4U);
	// Up to 13 (include 0)
	char numPlayerTechStore = 12;
	WriteHookToProcess((void*)_F_PlayerTechStoreR2, &numPlayerTechStore, 1U);
	// Recount
	hookGameCall((void*)_F_PlayerTechStoreR1, (uintptr_t)ReadPlayerFactionTypeCPP);

	// +2251C7
	hookGameCall((void*)_F_ReadPlayerFactionType, (uintptr_t)ReadPlayerFactionTypeCPP);
	//
	hookGameBlock((void*)_F_ReadPlayerFactionIcon, (uintptr_t)ReadPlayerFactionIconASM);
	hookGameBlock((void*)_F_GetUnitOverlayIconSettings, (uintptr_t)GetUnitOverlayIconSettingsASM);

	// Set no bloom
	hookGameBlock((void*)_F_BloomOpen, (uintptr_t)SetNoBloomASM);

	// ra3_1.12.game+2DDE95
	// Synchronized rendering and logical frames?
	WriteHookToProcess((void*)_F_SyncSet, (void*)&nop6, 6U);

	// ra3_1.12.game+324085
	// Up shift build count to 33!
	/*
	unsigned char ofs324085[] = {
		0x6B, 0xFA, 0x20,            // imul edi, edx, 32
		0x47,                        // inc edi
		0x0F, 0x1F, 0x44, 0x00, 0x00 // nop
	};*/
	unsigned char ofs324085[]= {
		0x8B, 0xCB, // mov ecx, ebx
		0xE8, 0x00, 0x00, 0x00, 0x00, // call
		0x8B, 0xF8	// mov edi, eax
	};
	int ofs324085_3 = (uintptr_t)BuildList_GetNewBuildCountASM - (_F_AddBuildListCount + 7);
	memcpy(&ofs324085[3], &ofs324085_3, 4U);
	WriteHookToProcess((void*)_F_AddBuildListCount, &ofs324085, 9U);

	// Fix when AttachUpdate's Flags has FIND_BEST_PARENT
	unsigned char set37AB71[] = {
		0x83, 0xC4, 0x0C,       // add esp, 0xC
		0x5D, 0x5E, 0x5F, 0x5B, // pop
		0xC3,                   // ret
		0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
	};
	WriteHookToProcess((void*)(_F_AttachUpdateFlagFix01), &set37AB71, 16U);
	unsigned char set37AB5E = 0xC;
	WriteHookToProcess((void*)(_F_AttachUpdateFlagFix01 - 0x11), &set37AB5E, 1U);
	// C*C = 0x90
	int set37AB64 = 0x90;
	WriteHookToProcess((void*)(_F_AttachUpdateFlagFix01 - 0xB), &set37AB64, 4U);

	// LASER!
	// Abandon changes to z-axis for now
	/**/
	unsigned char set32C8C6[] = {
		0x0F, 0x57, 0xDB, // xorps xmm3, xmm3
		0x66, 0x90        // nop 2
	};
	WriteHookToProcess((void*)ofs32C8C6, &set32C8C6, 5U);
	// Allows for vertical sweep, and change sweep angle
	hookGameBlock((void*)_F_SweepLaser01, (uintptr_t)SweepingLaserStateASM1);
	//
	hookGameBlock((void*)_F_SweepLaserActivate, (uintptr_t)SweepingLaserActivateASM);
	// Set laser to activate only when not activated
	hookGameBlock((void*)_F_ActivateLaser, (uintptr_t)ActivateLaserNuggetASM);
	// Fix no laser for attacking a target when DamageDealtAtSelfPosition is true
	unsigned char set34BD77[] = {
		0x83, 0x7E, 0x54, 0x00, // cmp dword ptr [esi+0x54], 0
		0x5F,                   // pop edi
		0x74, 0x25,             // je
		0x85, 0xED,             // test ebp, ebp
		0x75, 0x16,             // jne
		0x85                    // pad
	};
	WriteHookToProcess((void*)_F_ActivateLaserCheck54h, &set34BD77, 12U);

	// Let "ShowsAmmoPips" work
	hookGameCall((void*)_F_ShowAmmo, (uintptr_t)ShowsAmmoPipsASM);
	WriteHookToProcess((void*)(_F_ShowAmmo + 5), (void*)&nop2, 2U);
	// Set up reloading ammunition
	hookGameCall((void*)_F_WeaponReloadActive, (uintptr_t)WeaponReloadActiveASM);
	WriteHookToProcess((void*)(_F_WeaponReloadActive + 5), (void*)&nop1, 1U);
	hookGameCall((void*)_F_WeaponReloadTimeCount, (uintptr_t)WeaponReloadTimeCountASM);
	WriteHookToProcess((void*)(_F_WeaponReloadTimeCount + 5), (void*)&nop1, 1U);

	// <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
	hookGameCall((void*)_F_AttributeModifierT18Buff, (uintptr_t)AttributeModifierNo18BuffASM);
	WriteHookToProcess((void*)(_F_AttributeModifierT18Buff + 5), (void*)&nop1, 1U);

	// Fix weapon scatter radius
	//hookGameBlock((void*)_F_WeaponScatterRadius, (uintptr_t)WeaponScatterRadiusASM);
	//WriteHookToProcess((void*)(_F_WeaponScatterRadius + 5), &nop3, 3U);
	hookGameCall((void*)_F_WeaponScatterRadius1, (uintptr_t)WeaponScatterRadiusFixASM1);
	hookGameCall((void*)_F_WeaponScatterRadius2, (uintptr_t)WeaponScatterRadiusFixASM2);
	WriteHookToProcess((void*)(_F_WeaponScatterRadius2 + 5), (void*)&nop1, 1U);

	// Use the new method
	hookGameBlock((void*)_F_BehaviorUpdate_TiberiumCrystal, (uintptr_t)BehaviorUpdate_TiberiumCrystal);
	/*
	hookGameCall((void*)_F_KillTibCrystalWhenEmpty1, (uintptr_t)KillTibCrystalWhenEmptyASM1);
	WriteHookToProcess((void*)(_F_KillTibCrystalWhenEmpty1 + 5), &nop1, 1U);
	hookGameCall((void*)_F_KillTibCrystalWhenEmpty2, (uintptr_t)KillTibCrystalWhenEmptyASM2);
	*/

	// SecondaryObjectListenerModule
	// original is 0x1C, but now increase its size
	uint8_t ofs3DFF60 = 0x20;
	WriteHookToProcess((void*)_F_SecondaryObjectListenerModuleSize, &ofs3DFF60, 1U);
	// initialize memory
	hookGameBlock((void*)_F_SecondaryObjectListenerModuleInit, (uintptr_t)SecondaryObjectListenerModule_Initialize);
	// It is supported in MARV mode, but note that
	// Do not support a garrison other than your own, so AllowAlliesInside="false"
	// There is no exit function set, so you can't exit
	hookGameBlock((void*)_F_SecondaryObjectListenerModuleUpg, (uintptr_t)SecondaryObjectListenerModule_SetupUpgrade1);
	WriteHookToProcess((void*)(_F_SecondaryObjectListenerModuleUpg + 5), (void*)&nop3, 3U);
}

// 
float buildSpeed = 1.0f / 15.0f;

DWORD WINAPI setFrameTo60() {
	// set delay
	while (!InitializationCore) {
		Sleep(500);
	}
	Sleep(15000);

	int logicIntFrame = 30;
	*(int*)(hmodEXE + 0x8AF9D0) = logicIntFrame;
	//WriteHookToProcess((void*)(hmodEXE + 0x8AF9D0), &logicIntFrame, 4U);
	float logicSpeed = 30.0f * 0.001f;
	*(float*)(hmodEXE + 0x8DBC4C) = logicSpeed;
	float intervalFps = 1000.0f / 60.0f;
	*(float*)(hmodEXE + 0x8DBC1C) = intervalFps;
	float logicFrame = 30.0f;
	*(float*)(hmodEXE + 0x8DBC58) = logicFrame;
	float logicFps = 1.0f / 30.0f;
	*(float*)(hmodEXE + 0x8DBC94) = logicFps;
	float renderFrame = 60.0f;
	*(float*)(hmodEXE + 0x8DBC50) = renderFrame;
	float renderFps = 1.0f / 60.0f;
	*(float*)(hmodEXE + 0x8DBD34) = renderFps;
	int renderIntFps = 1000 / 60;
	*(int*)(hmodEXE + 0x8E176C) = renderIntFps;
	int render500 = 500;
	WriteHookToProcess((void*)(hmodEXE + 0x7B3C50 + 1), &renderIntFps, 4U);
	// shr eax, 1
	// mov dword ptr [CE176C], eax
	// ret 4
	unsigned char setRenderIntFps[] = {
		0xD1, 0xE8,
		0xA3, 0x6C, 0x17, 0xCE, 0x00,
		0xC2, 0x04, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x1B81A6), &setRenderIntFps, 10U);

	// mov eax, 2
	unsigned char setEAXto2[] = {
		0xB8, 0x02, 0x00, 0x00, 0x00
	};
	// coordinates when unit moves
	WriteHookToProcess((void*)(hmodEXE + 0x1FFAD0), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x1FFAD0 + 7), (void*)&nop6, 6U);

	// main logic
	WriteHookToProcess((void*)(hmodEXE + 0x216256), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x216256 + 7), (void*)&nop6, 6U);
	// build speed factor
	uintptr_t pbuildSpeed = (uintptr_t)&buildSpeed;
	WriteHookToProcess((void*)(hmodEXE + 0x2D3D63 + 2), &pbuildSpeed, 4U);

	//
	//WriteHookToProcess((void*)(hmodEXE + 0x20283D), &setEAXto2, 5U);
	//WriteHookToProcess((void*)(hmodEXE + 0x20283D + 7), &nop6, 6U);
	// 
	//WriteHookToProcess((void*)(hmodEXE + 0x21B59B), &setEAXto2, 5U);
	//WriteHookToProcess((void*)(hmodEXE + 0x21B59B + 7), &nop6, 6U);
	//
	//WriteHookToProcess((void*)(hmodEXE + 0x22935D), &setEAXto2, 5U);
	//WriteHookToProcess((void*)(hmodEXE + 0x22935D + 7), &nop6, 6U);

	return 0;
}

bool __fastcall GetFunctionAddress()
{
	//hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

	if (checkRA3Address(hmodEXE + 0x85B6C4))
	{
		// Steam version
		_F_BloomOpen = hmodEXE + 0x1FB9DB;
		_Ret_BloomOpen = hmodEXE + 0x1FB9DB + 19;
		_F_PlayerTechStoreL = hmodEXE + 0x682992;
		_Ret_PlayerTechStoreL = hmodEXE + 0x682992 + 5;
		_F_PlayerTechStoreR1 = hmodEXE + 0x69ABA6 - 10;
		_F_PlayerTechStoreR2 = hmodEXE + 0x69ABA6 - 3;
		_F_PlayerTechStoreR3 = hmodEXE + 0x69ABA6 + 3;
		_F_ReadPlayerFactionType = hmodEXE + 0x2251C7;
		_F_ReadPlayerFactionIcon = hmodEXE + 0x69732E;
		_Ret_ReadPlayerFactionIcon = hmodEXE + 0x697353;
		_F_GetUnitOverlayIconSettings = hmodEXE + 0x11C609;
		_F_SyncSet = hmodEXE + 0x2DDE95;
		_F_AttachUpdateFlagFix01 = hmodEXE + 0x37AB71;
		_F_SweepLaser01 = hmodEXE + 0x3C3ED7;
		_F_SweepLaserActivate = hmodEXE + 0x2E3759;
		_Ret_SweepLaserActivate = hmodEXE + 0x2E3759 + 0xC;
		ofs32C8C6 = hmodEXE + 0x32C8C6;
		_F_ActivateLaser = hmodEXE + 0x3CF668;
		_Ret_ActivateLaser = hmodEXE + 0x3CF668 + 6;
		_F_ActivateLaserCheck54h = hmodEXE + 0x34BD77;
		_F_ShowAmmo = hmodEXE + 0x128746;
		_F_WeaponReloadActive = hmodEXE + 0x3BE05F;
		_F_WeaponReloadTimeCount = hmodEXE + 0x2DC270;
		_F_AttributeModifierT18Buff = hmodEXE + 0xDAABD;
		// now it is useless
		//_F_WeaponScatterRadius = hmodEXE + 0x3140CB;
		//_Ret_WeaponScatterRadius = hmodEXE + 0x3140F0;
		_F_CallRandomRadius = hmodEXE + 0x200B10;
		_F_WeaponScatterRadius1 = hmodEXE + 0x35AA6B;
		_F_WeaponScatterRadius2 = hmodEXE + 0x35AA8A;

		_F_KillTibCrystalWhenEmpty1 = hmodEXE + 0x422717;
		_F_KillTibCrystalWhenEmpty2 = hmodEXE + 0x42278D;
		_F_CallKillGameObject = hmodEXE + 0x39EA50;
		_F_BehaviorUpdate_TiberiumCrystal = hmodEXE + 0x305A26;
		_Ret_BehaviorUpdate_TiberiumCrystal = hmodEXE + 0x305A26 + 5;

		_F_AddBuildListCount = hmodEXE + 0x324085;

		_F_SecondaryObjectListenerModuleSize = hmodEXE + 0x3DFF60+1;
		_F_SecondaryObjectListenerModuleInit = hmodEXE + 0x2E4BC3;
		_F_SecondaryObjectListenerModuleUpg = hmodEXE + 0x3B19E3;
		_F_Call00792EF0 = hmodEXE + 0x392EF0;
		_F_Call00779650 = hmodEXE + 0x379650;

		/*
		initializeEnumStringType();
		uintptr_t pCFlag = (uintptr_t)&g_CampaignFlag;
		WriteHookToProcess((void*)(hmodEXE + 0x2D8A + 1), &pCFlag, 4U);
		//WriteHookToProcess((void*)(hmodEXE + 0x22C6 + 1), &pCFlag, 4U);
		pCFlag += sizeof(g_CampaignFlag);
		WriteHookToProcess((void*)(hmodEXE + 0x2D85 + 1), &pCFlag, 4U);
		WriteHookToProcess((void*)(hmodEXE + 0x2D97 + 1), &pCFlag, 4U);
		char numCFlag = 2;
		WriteHookToProcess((void*)(hmodEXE + 0x174A2C + 2), &numCFlag, 1U);
		//WriteHookToProcess((void*)(hmodEXE + 0x22C4 + 1), &numCFlag, 1U);
		*/

		//
		RA3::LuaEngine::InitializeLuaEngineSteam(hmodEXE);
		// up fps to 60
		if (inputSetting.UPto60) {
			HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)setFrameTo60, NULL, NULL, NULL);
			if (tempHND) {
				CloseHandle(tempHND);
			}
		}
	}
	else if (checkRA3Address(hmodEXE + 0x86262C))
	{
		// Origin version
		_F_BloomOpen = hmodEXE + 0x23A96B;
		_Ret_BloomOpen = hmodEXE + 0x23A96B + 19;
		_F_PlayerTechStoreL = hmodEXE + 0x6173C2;
		_Ret_PlayerTechStoreL = hmodEXE + 0x6173C2 + 5;
		_F_PlayerTechStoreR1 = hmodEXE + 0x62F666 - 10;
		_F_PlayerTechStoreR2 = hmodEXE + 0x62F666 - 3;
		_F_PlayerTechStoreR3 = hmodEXE + 0x62F666 + 3;
		_F_ReadPlayerFactionType = hmodEXE + 0x2640F7;
		_F_ReadPlayerFactionIcon = hmodEXE + 0x62BDAE;
		_Ret_ReadPlayerFactionIcon = hmodEXE + 0x62BDD3;
		_F_GetUnitOverlayIconSettings = hmodEXE + 0x15DDF9;
		_F_SyncSet = hmodEXE + 0x31C3D5;
		_F_AttachUpdateFlagFix01 = hmodEXE + 0x3B8FD1;
		_F_SweepLaser01 = hmodEXE + 0x402227;
		_F_SweepLaserActivate = hmodEXE + 0x321BC9;
		_Ret_SweepLaserActivate = hmodEXE + 0x321BC9 + 0xC;
		ofs32C8C6 = hmodEXE + 0x36AE86;
		_F_ActivateLaser = hmodEXE + 0x40D988;
		_Ret_ActivateLaser = hmodEXE + 0x40D988 + 6;
		_F_ActivateLaserCheck54h = hmodEXE + 0x38A397;
		_F_ShowAmmo = hmodEXE + 0x169D96;
		_F_WeaponReloadActive = hmodEXE + 0x3FC3AF;
		_F_WeaponReloadTimeCount = hmodEXE + 0x31A7E0;
		_F_AttributeModifierT18Buff = hmodEXE + 0x11C4DD;
		_F_CallRandomRadius = hmodEXE + 0x23F990;
		_F_WeaponScatterRadius1 = hmodEXE + 0x3990AB;
		_F_WeaponScatterRadius2 = hmodEXE + 0x3990CA;
		_F_KillTibCrystalWhenEmpty1 = hmodEXE + 0x4608A7;
		_F_KillTibCrystalWhenEmpty2 = hmodEXE + 0x46091D;
		_F_CallKillGameObject = hmodEXE + 0x3DCDF0;
		_F_BehaviorUpdate_TiberiumCrystal = hmodEXE + 0x343E96;
		_Ret_BehaviorUpdate_TiberiumCrystal = hmodEXE + 0x343E96 + 5;
		_F_AddBuildListCount = hmodEXE + 0x362485;
		_F_SecondaryObjectListenerModuleSize = hmodEXE + 0x41E280+1;
		_F_SecondaryObjectListenerModuleInit = hmodEXE + 0x323033;
		_F_SecondaryObjectListenerModuleUpg = hmodEXE + 0x3EFD03;
		_F_Call00792EF0 = hmodEXE + 0x3D1260;
		_F_Call00779650 = hmodEXE + 0x3B7B10;

		//
		RA3::LuaEngine::InitializeLuaEngineOrigin(hmodEXE);
	}
	else
	{
		MessageBox(NULL, L"This is not Red Alert 3!", L"Warning", MB_OK);
		return false;
	}

	return true;
}

void SetCPUAffinity()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	// This is the number of logical processors, not the number of physical cores
	int threadsNum = sysInfo.dwNumberOfProcessors;
	//std::wstring threads_mum = std::to_wstring(threadsNum);
	//MessageBoxW(NULL, threads_mum.c_str(), L"core", MB_OK);
	HANDLE gameProcess = GetCurrentProcess();
	if (threadsNum >= 12) {
		SetProcessAffinityMask(gameProcess, 0b010101010101);
	}
	else if (threadsNum >= 8) {
		SetProcessAffinityMask(gameProcess, 0b01010101);
	}
	else if (threadsNum >= 6) {
		SetProcessAffinityMask(gameProcess, 0b010101);
	}
	else if (threadsNum >= 4) {
		SetProcessAffinityMask(gameProcess, 0b0101);
	}
}

void mainInjectionExecution()
{
	bool isRA3 = GetFunctionAddress();
	if (isRA3) {
		//Sleep(1000);
		hookFunctionGroup();

		if (inputSetting.cpuLimit) {
			SetCPUAffinity();
		}

		if (inputSetting.setDebug) {
			MessageBox(NULL, L"Injection OK!\n   v2.403", L"Check", MB_OK);
		}
	}
}

void mainInjectionSetting(LPCWSTR iniPath)
{
	if (GetPrivateProfileIntW(L"PlayerTemplates", L"Customization", 0, iniPath)) {
		playerNameHashToIndex[0][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction4", playerNameHashToIndex[0][0], iniPath);
		playerNameHashToIndex[1][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction5", playerNameHashToIndex[1][0], iniPath);
		playerNameHashToIndex[2][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction6", playerNameHashToIndex[2][0], iniPath);
		playerNameHashToIndex[3][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction7", playerNameHashToIndex[3][0], iniPath);
		playerNameHashToIndex[4][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction8", playerNameHashToIndex[4][0], iniPath);
		playerNameHashToIndex[5][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction9", playerNameHashToIndex[5][0], iniPath);
		playerNameHashToIndex[6][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction10", playerNameHashToIndex[6][0], iniPath);
		playerNameHashToIndex[7][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction11", playerNameHashToIndex[7][0], iniPath);
		playerNameHashToIndex[8][0] = GetPrivateProfileIntW(L"PlayerTemplates", L"Faction12", playerNameHashToIndex[8][0], iniPath);
	}

	if (GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Customization", 0, iniPath)) {
		playerTechStoreToIndex[0][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction4", playerTechStoreToIndex[0][0], iniPath);
		playerTechStoreToIndex[1][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction5", playerTechStoreToIndex[1][0], iniPath);
		playerTechStoreToIndex[2][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction6", playerTechStoreToIndex[2][0], iniPath);
		playerTechStoreToIndex[3][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction7", playerTechStoreToIndex[3][0], iniPath);
		playerTechStoreToIndex[4][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction8", playerTechStoreToIndex[4][0], iniPath);
		playerTechStoreToIndex[5][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction9", playerTechStoreToIndex[5][0], iniPath);
		playerTechStoreToIndex[6][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction10", playerTechStoreToIndex[6][0], iniPath);
		playerTechStoreToIndex[7][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction11", playerTechStoreToIndex[7][0], iniPath);
		playerTechStoreToIndex[8][0] = GetPrivateProfileIntW(L"PlayerTechStoreTemplates", L"Faction12", playerTechStoreToIndex[8][0], iniPath);
	}
}
