#include "pch.h"
#include "commonData.h"
#include "commonAddr.h"
#include "utiliy.h"

#include "funcPlayer.h"
#include "funcWeapon.h"
#include "funcMLaser.h"
#include "funcOther.h"
#include "commonMain.h"

void __fastcall hookFunctionGroup()
{

	// Load PlayerTechStoreTemplate
	hookGameBlock((void*)_F_PlayerTechStoreL, (uintptr_t)LoadPlayerTechStoreASM);
	// Read PlayerTechStoreTemplate
	uintptr_t pPTS = (uintptr_t)&pPlayerTechStore;
	WriteHookToProcess((void*)_F_PlayerTechStoreR3, &pPTS, 4U);
	// Up to 13 (include 0)
	char numPlayerTechStore = 12;
	WriteHookToProcess((void*)_F_PlayerTechStoreR2, &numPlayerTechStore, 1U);
	// Recount
	hookGameCall((void*)_F_PlayerTechStoreR1, (uintptr_t)ReadPlayerTechStoreCPP);

	// Set no bloom
	hookGameBlock((void*)_F_BloomOpen, (uintptr_t)SetNoBloomASM);

	// ra3_1.12.game+2DDE95
	// Synchronized rendering and logical frames?
	WriteHookToProcess((void*)_F_SyncSet, &nop6, 6U);

	// Abandon changes to z-axis for now
	/**/
	unsigned char set32C8C6[] = {
		0x0F, 0x57, 0xDB, // xorps xmm3, xmm3
		0x66, 0x90        // nop 2
	};
	WriteHookToProcess((void*)ofs32C8C6, &set32C8C6, 5U);
	// Allows for vertical sweep, and change sweep angle
	hookGameBlock((void*)_F_SweepLaser01, (uintptr_t)SweepingLaserStateASM1);
	// Set laser to activate only when not activated
	hookGameBlock((void*)_F_ActivateLaser, (uintptr_t)ActivateLaserNuggetASM);

	// Let "ShowsAmmoPips" work
	hookGameCall((void*)_F_ShowAmmo, (uintptr_t)ShowsAmmoPipsASM);
	WriteHookToProcess((void*)(_F_ShowAmmo + 5), &nop2, 2U);
	// Set up reloading ammunition
	hookGameCall((void*)_F_WeaponReloadActive, (uintptr_t)WeaponReloadActiveASM);
	WriteHookToProcess((void*)(_F_WeaponReloadActive + 5), &nop1, 1U);
	hookGameCall((void*)_F_WeaponReloadTimeCount, (uintptr_t)WeaponReloadTimeCountASM);
	WriteHookToProcess((void*)(_F_WeaponReloadTimeCount + 5), &nop1, 1U);

	// <Modifier Type="BOUNTY_PERCENTAGE" Value="50%"/>
	hookGameCall((void*)_F_AttributeModifierT18Buff, (uintptr_t)AttributeModifierNo18BuffASM);
	WriteHookToProcess((void*)(_F_AttributeModifierT18Buff + 5), &nop1, 1U);
}

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
	//
	WriteHookToProcess((void*)(hmodEXE + 0x1FFAD0), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x1FFAD0 + 7), &nop6, 6U);
	//
	WriteHookToProcess((void*)(hmodEXE + 0x20283D), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x20283D + 7), &nop6, 6U);
	//
	WriteHookToProcess((void*)(hmodEXE + 0x216256), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x216256 + 7), &nop6, 6U);
	//
	WriteHookToProcess((void*)(hmodEXE + 0x21B59B), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x21B59B + 7), &nop6, 6U);
	//
	WriteHookToProcess((void*)(hmodEXE + 0x22935D), &setEAXto2, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x22935D + 7), &nop6, 6U);

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
		_F_SyncSet = hmodEXE + 0x2DDE95;
		_F_SweepLaser01 = hmodEXE + 0x3C3ED7;
		ofs32C8C6 = hmodEXE + 0x32C8C6;
		_F_ActivateLaser = hmodEXE + 0x3CF668;
		_Ret_ActivateLaser = hmodEXE + 0x3CF668 + 6;
		_F_ShowAmmo = hmodEXE + 0x128746;
		_F_WeaponReloadActive = hmodEXE + 0x3BE05F;
		_F_WeaponReloadTimeCount = hmodEXE + 0x2DC270;
		_F_AttributeModifierT18Buff = hmodEXE + 0xDAABD;

		// up fps to 60
		if (inputSetting.UPto60) {
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)setFrameTo60, NULL, NULL, NULL);
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
		_F_SyncSet = hmodEXE + 0x31C3D5;
		_F_SweepLaser01 = hmodEXE + 0x402227;
		ofs32C8C6 = hmodEXE + 0x36AE86;
		_F_ActivateLaser = hmodEXE + 0x40D988;
		_Ret_ActivateLaser = hmodEXE + 0x40D988 + 6;
		_F_ShowAmmo = hmodEXE + 0x169D96;
		_F_WeaponReloadActive = hmodEXE + 0x3FC3AF;
		_F_WeaponReloadTimeCount = hmodEXE + 0x31A7E0;
		_F_AttributeModifierT18Buff = hmodEXE + 0x11C4DD;
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
	if (threadsNum >= 8) {
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

		if (inputSetting.CheckBloom) {
			noBloomSet = 1;
		}

		if (inputSetting.setDebug) {
			MessageBox(NULL, L"Injection OK!\n   v2.303", L"Check", MB_OK);
		}
	}
}
