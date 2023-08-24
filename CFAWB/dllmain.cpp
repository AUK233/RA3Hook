#include "pch.h"
#include "proxy.h"
#include <Windows.h>
#include <string>

#include "..\DllCore\utiliy.h"
#include "..\DllCore\commonEnum.hpp"

uintptr_t test1 = 0x004454A6;
uintptr_t call1 = 0x004D6D50;
uintptr_t call2 = 0x00401600;

__declspec(naked) void __fastcall test() {
	__asm {
			xor edi, edi
		returnThis:
			push 0xAE1B8C
			push edi
			call call1
			add esp, 8
			push eax
			push -1
			mov ecx, esi
			call call2
			add edi, 1
			cmp edi, 2
			jl returnThis
			jmp test1
	}
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

		// test
		initializeEnumStringType();

		uintptr_t hmodEXE = 0x00400000;
		uintptr_t pPTS = (uintptr_t)&g_CampaignFlag;
		WriteHookToProcess((void *)(hmodEXE + 0xD6D5A + 1), &pPTS, 4U);
		pPTS += sizeof(g_CampaignFlag);
		WriteHookToProcess((void *)(hmodEXE + 0xD6D55 + 1), &pPTS, 4U);
		WriteHookToProcess((void *)(hmodEXE + 0xD6D67 + 1), &pPTS, 4U);
		hookGameBlock((void *)(hmodEXE + 0x4548D), (uintptr_t)test);
	}
	
	return TRUE;
}
