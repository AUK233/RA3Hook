// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <format>
#include <span>
#include <string>

#include "commonAddr.h"
#include "utiliy.h"

#include "funcMLaser.h"
#include "funcOther.h"
//#include "dllmain.h"

void hookFunctionGroup()
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

	// ra3_1.12.game+2DDE95
	// Synchronized rendering and logical frames?
	unsigned char nop6[] = {
		0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00
	};
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

	// Allows for vertical sweep, and change sweep angle
	hookGameBlock((void*)_F_BloomOpen, (uintptr_t)SetNoBloomASM);
}

bool GetFunctionAddress()
{
	hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

	if (checkRA3Address(hmodEXE + 0x85B6C4))
	{
		// Steam version
		_F_BloomOpen = hmodEXE + 0x1FB9DB;
		_Ret_BloomOpen = hmodEXE + 0x1FB9DB + 19;
		_F_SyncSet = hmodEXE + 0x2DDE95;
		_F_SweepLaser01 = hmodEXE + 0x3C3ED7;
		ofs32C8C6 = hmodEXE + 0x32C8C6;
	}
	else if (checkRA3Address(hmodEXE + 0x86262C))
	{
		// Origin version
		_F_BloomOpen = hmodEXE + 0x23A96B;
		_Ret_BloomOpen = hmodEXE + 0x23A96B + 19;
		_F_SyncSet = hmodEXE + 0x31C3D5;
		_F_SweepLaser01 = hmodEXE + 0x402227;
		ofs32C8C6 = hmodEXE + 0x36AE86;
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

	memcpy(&inputSetting, input->UserData, 4);
	
	// 
	Sleep(1000);
	bool isRA3 = GetFunctionAddress();
	if (isRA3) {
		Sleep(1000);
		hookFunctionGroup();

		if (inputSetting.cpuLimit) {
			SetCPUAffinity();
		}

		if (inputSetting.CheckBloom) {
			noBloomSet = 1;
		}
	}

	/*
	if (inputSetting.CheckBloom) {
		MessageBox(NULL, L"Test dll injection", L"Test", MB_OK);
	}*/

	//MessageBox(NULL, L"Test dll injection", L"Test", MB_OK);

	// need
	//FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}
