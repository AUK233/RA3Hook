// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <format>
#include <span>
#include <string>
#include <vector>
#include <tlhelp32.h>

#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )

#include "commonData.h"
#include "commonAddr.h"
#include "utiliy.h"

#include "funcPlayer.h"
#include "funcWeapon.h"
#include "funcMLaser.h"
#include "funcOther.h"
//#include "dllmain.h"

void __fastcall hookFunctionGroup()
{
	// Invalid at runtime
	/*
	uintptr_t ofs4gPos = hmodEXE + 0x126;
	DWORD Protect4GB;
	VirtualProtect((void*)ofs4gPos, 1, PAGE_EXECUTE_READWRITE, &Protect4GB);
	char value4GB = *(char*)ofs4gPos;
	value4GB |= 0b00100000;
	if (value4GB != 0x23) {
		MessageBox(NULL, L"wwwwwwwww", L"WARNING", MB_OK);
	}
	*(char*)ofs4gPos = value4GB;
	VirtualProtect((void*)ofs4gPos, 1, Protect4GB, &Protect4GB);
	*/

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
	WriteHookToProcess((void*)(_F_ShowAmmo+5), &nop2, 2U);
	//
	hookGameCall((void*)_F_WeaponReloadActive, (uintptr_t)WeaponReloadActiveASM);
	WriteHookToProcess((void*)(_F_WeaponReloadActive + 5), &nop1, 1U);
	hookGameCall((void*)_F_WeaponReloadTimeCount, (uintptr_t)WeaponReloadTimeCountASM);
	WriteHookToProcess((void*)(_F_WeaponReloadTimeCount + 5), &nop1, 1U);
}

bool __fastcall GetFunctionAddress()
{
	hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

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

		/**/
		int logicIntFrame = 30;
		WriteHookToProcess((void*)(hmodEXE + 0x8AF9D0), &logicIntFrame, 4U);
		float logicSpeed = 7.5f * 0.001f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBC4C), &logicSpeed, 4U);
		float intervalFps = 1000.0f / 60.0f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBC1C), &intervalFps, 4U);
		float logicFrame = 30.0f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBC58), &logicFrame, 4U);
		float logicFps = 1.0f / 30.0f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBC94), &logicFps, 4U);
		float renderFrame = 60.0f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBC50), &renderFrame, 4U);
		float renderFps = 1.0f / 15.0f;
		WriteHookToProcess((void*)(hmodEXE + 0x8DBD34), &renderFps, 4U);
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
	}
	else
	{
		MessageBox(NULL, L"This is not Red Alert 3!", L"Warning", MB_OK);
		return false;
	}

	return true;
}

/*
uintptr_t _F_GameEngine = 0;
__declspec(naked) void __fastcall asmInjectDLLMain()
{
	__asm {
		mov edi, ecx
		call GetFunctionAddress
		call hookFunctionGroup
		xor ebx, ebx
		push ebx
		mov ecx, edi
		jmp _F_GameEngine
	}
}

bool CheckIsRA3()
{
	hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

	if (checkRA3Address(hmodEXE + 0x85B6C4))
	{
		// Steam version
		_F_GameEngine = hmodEXE + 0x227A28 + 6;
		hookGameBlock((void*)(hmodEXE + 0x227A28), (uintptr_t)asmInjectDLLMain);
	}
	else if (checkRA3Address(hmodEXE + 0x86262C))
	{
		return false;
	}
	else
	{
		MessageBox(NULL, L"This is not Red Alert 3!", L"Warning", MB_OK);
		return false;
	}

	return true;
}
*/

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

// copied from EasyHook.h
struct REMOTE_ENTRY_INFO
{
	ULONG HostPID;
	UCHAR* UserData;
	ULONG UserDataSize;
};

extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO * input)
{
	/*
	auto inputPointer = reinterpret_cast<wchar_t const*>(input->UserData);
	std::wstring_view inputString{ inputPointer, input->UserDataSize / sizeof(wchar_t) };
	//std::wstring message = std::format(L"Launcher Process ID: {} Inject Message: {} End Process?", input->HostPID, inputString);
	std::wstring message = L"End Process?";
	if (MessageBoxW(nullptr, message.c_str(), L"Dll1", MB_YESNO) == IDYES)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}*/
	UINT initType = 0;
	if ((uintptr_t)input->UserData < 0x10000 || input->UserDataSize < 4) {
		initType = 1;
	}
	else {
		initType = *(int*)input->UserData;
	}

	if (!initType) {
		memcpy(&inputSetting, input->UserData + 4, 4);
	}

	DWORD idThread = GetCurrentThreadId();
	DWORD idProcess = GetCurrentProcessId();
	std::vector<HANDLE> hEXEGroup;
	THREADENTRY32 te;
	te.dwSize = sizeof(THREADENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (Thread32First(hSnapshot, &te))
	{
		do
		{
			if (idProcess == te.th32OwnerProcessID)
			{
				if (idThread != te.th32ThreadID) {
					hEXEGroup.push_back(OpenThread(THREAD_ALL_ACCESS, 0, te.th32ThreadID));
					SuspendThread(hEXEGroup.back());
					break;
				}
			}
		} while (Thread32Next(hSnapshot, &te));
	}
	CloseHandle(hSnapshot);

	if (initType) {
		WCHAR iniPath[MAX_PATH];
		GetModuleFileNameW(GetModuleHandleW(L"DllCore.dll"), iniPath, _countof(iniPath));
		PathRemoveFileSpecW(iniPath);
		wcscat_s(iniPath, L"\\1Setting.ini");
		// read configuration
		inputSetting.CheckBloom = GetPrivateProfileIntW(L"CFALauncher", L"NoBloom", 0, iniPath);
		inputSetting.cpuLimit = GetPrivateProfileIntW(L"CFALauncher", L"SetCPU", 0, iniPath);
		inputSetting.setDebug = GetPrivateProfileIntW(L"CFALauncher", L"SetDebug", 0, iniPath);
	}

	//
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
			MessageBox(NULL, L"Injection OK!\n   v2.302", L"Check", MB_OK);
		}
	}

	for (size_t i = 0; i < hEXEGroup.size(); i++) {
		ResumeThread(hEXEGroup[i]);
	}
	hEXEGroup.clear();
	hEXEGroup.shrink_to_fit();

	//MessageBox(NULL, L"Test dll injection", L"Test", MB_OK);

	// need
	//FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}
