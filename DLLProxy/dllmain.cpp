#include "pch.h"
#include "proxy.h"
#include <Windows.h>
#include <string>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "..\DllCore\commonMain.h"

extern struct inputSettingINFO {
	unsigned char cpuLimit = 0;
	unsigned char CheckBloom = 0;
	unsigned char setDebug = 0;
	unsigned char UPto60 = 0;
	unsigned char align[4];
} inputSetting;

extern int InitializationCore;

BOOL WINAPI DllMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstModule);

		// proxy dll
		wchar_t modulePath[MAX_PATH];
		if (!GetWindowsDirectoryW(modulePath, _countof(modulePath))) {
			DWORD dwError = GetLastError();
			return FALSE;
		}
		wcscat_s(modulePath, L"\\System32\\winmm.dll");
		auto moduleHandle = LoadLibraryW(modulePath);
		if (moduleHandle != nullptr)
		{
			for (auto i = 0; i < FUNCTION_COUNT; i++)
			{
				auto address = GetProcAddress(moduleHandle, functionNames[i].c_str());
				functions[i] = reinterpret_cast<uintptr_t>(address);
			}
		}
		// proxy dll end

		// initialization
		WCHAR iniPath[MAX_PATH];
		GetModuleFileNameW(NULL, iniPath, _countof(iniPath));
		PathRemoveFileSpecW(iniPath);
		wcscat_s(iniPath, L"\\config.ini");
		// read configuration
		inputSetting.CheckBloom = GetPrivateProfileIntW(L"CFASetting", L"NoBloom", 0, iniPath);
		inputSetting.cpuLimit = GetPrivateProfileIntW(L"CFASetting", L"SetCPU", 0, iniPath);
		inputSetting.setDebug = GetPrivateProfileIntW(L"CFASetting", L"SetDebug", 0, iniPath);
		inputSetting.UPto60 = GetPrivateProfileIntW(L"CFASetting", L"DualFPS", 0, iniPath);

		mainInjectionExecution();
		InitializationCore = 1;
	}
	
	return TRUE;
}
