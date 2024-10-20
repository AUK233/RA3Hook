#include "pch.h"
#include "proxy.h"
#include <Windows.h>
#include <string>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "..\DllCore\commonMain.h"
#include "../DllCore/commonStruct.hpp"

extern inputSettingINFO inputSetting;
extern int InitializationCore;
extern std::wstring CampaignINIDefault;

std::string GamePath = "D:\\1main\\game\\Red Alert 3\\Data\\";

void __fastcall UpdateGameDataPath(uintptr_t targetAddr, uintptr_t dataAddr) {
	uint8_t hookFunction[] = {0xE8, 0x00, 0x00, 0x00, 0x00, 0x90};
	int offset = dataAddr - (targetAddr + 5);
	memcpy(&hookFunction[1], &offset, 4U);

	DWORD oldProtect;
	VirtualProtect((LPVOID)targetAddr, 6U, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((LPVOID)targetAddr, hookFunction, 6U);
	VirtualProtect((LPVOID)targetAddr, 6U, oldProtect, &oldProtect);
}

HCURSOR WINAPI newGameBasePath(LPSTR lpFilename) {
	//MessageBoxA(0, inStr, "test", MB_OK);
	std::string newPath = GamePath + lpFilename;
	return LoadCursorFromFileA(newPath.c_str());
}

void __fastcall UpdateGameBasePath() {
	UpdateGameDataPath(0x50B63F, (uintptr_t)newGameBasePath);
}

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
		ZeroMemory(&inputSetting, sizeof(inputSettingINFO));
		WCHAR iniPath[MAX_PATH];
		GetModuleFileNameW(NULL, iniPath, _countof(iniPath));
		PathRemoveFileSpecW(iniPath);
		wcscat_s(iniPath, L"\\config.ini");
		// read configuration
		inputSetting.CheckBloom = GetPrivateProfileIntW(L"CFASetting", L"NoBloom", 0, iniPath);
		inputSetting.cpuLimit = GetPrivateProfileIntW(L"CFASetting", L"SetCPU", 0, iniPath);
		inputSetting.setDebug = GetPrivateProfileIntW(L"CFASetting", L"SetDebug", 0, iniPath);
		inputSetting.UPto60 = GetPrivateProfileIntW(L"CFASetting", L"DualFPS", 0, iniPath);
		inputSetting.LocalFlag = GetPrivateProfileIntW(L"CFASetting", L"ForceLocal", 0, iniPath);

		// Campaign Flag INI Path
		WCHAR CFiniPath[MAX_PATH];
		GetModuleFileNameW(NULL, CFiniPath, _countof(CFiniPath));
		PathRemoveFileSpecW(CFiniPath);
		wcscat_s(CFiniPath, L"\\CFACampaignFlag.ini");
		CampaignINIDefault = CFiniPath;

		//UpdateGameBasePath();

		mainInjectionSetting(iniPath);
		mainInjectionExecution();
		InitializationCore = 1;
	}
	
	return TRUE;
}
