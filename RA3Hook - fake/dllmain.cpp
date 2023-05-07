#include "pch.h"
#include "proxy.h"
#include <Windows.h>
#include <string>

uintptr_t hmodEXE;

void WriteHookToProcess(void* addr, void* data, size_t len) {
	DWORD oldProtect;
	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, data, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
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

		hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

		// ra3_1.12.game+2DDE95
		unsigned char beeAccuracy[] = {
			0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00
		};
		WriteHookToProcess((void*)(hmodEXE + 0x2DDE95), &beeAccuracy, 6U);

		MessageBox(NULL, L"Test dll injection", L"Test", MB_OK);
	}
	
	return TRUE;
}
