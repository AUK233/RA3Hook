// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <format>
#include <span>
#include <string>
#include <vector>
#include <filesystem>
#include <tlhelp32.h>

#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )

#include "commonMain.h"
#include "commonStruct.hpp"

extern inputSettingINFO inputSetting;
extern int InitializationCore;
extern std::wstring CampaignINIDefault;

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
	ZeroMemory(&inputSetting, sizeof(inputSettingINFO));
	UINT initType = 0;
	if ((uintptr_t)input->UserData < 0x10000 || input->UserDataSize < 4) {
		initType = 1;
	}
	else {
		initType = *(int*)input->UserData;
	}

	if (!initType) {
		memcpy(&inputSetting, input->UserData + 4, sizeof(inputSettingINFO));
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
		inputSetting.LocalFlag = GetPrivateProfileIntW(L"CFALauncher", L"ForceLocal", 0, iniPath);
	}

	// Campaign Flag INI Path
	WCHAR CFiniPath[MAX_PATH];
	GetModuleFileNameW(GetModuleHandleW(L"DllCore.dll"), CFiniPath, _countof(CFiniPath));
	PathRemoveFileSpecW(CFiniPath);
	wcscat_s(CFiniPath, L"\\mapping\\CFACampaignFlag.ini");
	CampaignINIDefault = CFiniPath;

	//
	//
	//
	WCHAR SettingPath[MAX_PATH];
	GetModuleFileNameW(GetModuleHandleW(L"DllCore.dll"), SettingPath, _countof(SettingPath));
	PathRemoveFileSpecW(SettingPath);
	wcscat_s(SettingPath, L"\\2Setting.ini");
	if (std::filesystem::exists(SettingPath)) {
		mainInjectionSetting(SettingPath);
	}
	//
	mainInjectionExecution();
	//

	for (size_t i = 0; i < hEXEGroup.size(); i++) {
		ResumeThread(hEXEGroup[i]);
	}
	hEXEGroup.clear();
	hEXEGroup.shrink_to_fit();

	InitializationCore = 1;

	// need
	//FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}
