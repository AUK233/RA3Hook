#include "pch.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <codecvt>
#include <filesystem>
#include <windows.h>
#include <shellapi.h>

#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )

#include "easyhook.h"
#pragma comment(lib, "EasyHook32.lib")

#include "dllmain.h"

#pragma warning(disable:4996);


BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" int DirectGameLaunch(LPCWSTR FileName, LPCWSTR Arguments,
	LPCWSTR DllPath, PVOID dllData, ULONG dataSize,
	LPCWSTR BattleNetPath, PVOID BNlog, ULONG logSize)
{
	if (std::filesystem::exists(FileName)) {
		std::wstring appRun = FileName;
		appRun += L" ";
		appRun += Arguments;

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		Sleep(400);
		CreateProcessW(NULL, (LPWSTR)appRun.c_str(),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si, &pi);
		//
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		//
		NTSTATUS nt = RhInjectLibrary(pi.dwProcessId, 0, 0, (WCHAR*)DllPath, NULL, dllData, dataSize);
		if (nt != 0) {
			MessageBoxW(NULL, L"Startup failed, please launch RA3 again.", L"error", MB_OK);
			return 1;
		}

		if (std::wstring(BattleNetPath) != L"") {
			Sleep(3000);
			nt = RhInjectLibrary(pi.dwProcessId, 0, 0, (WCHAR*)BattleNetPath, NULL, BNlog, logSize);
			if (nt != 0) {
				MessageBoxW(NULL, L"BattleNet failed to load, please launch RA3 again.", L"error", MB_OK);
				return 1;
			}
		}

		Sleep(500);
		return 0;
	}
	else {
		MessageBoxW(NULL, L"Have you installed Command and Conquer Red Alert 3?", L"Warning!", MB_OK);
		return 1;
	}
}

extern "C" int PowerfulGameLaunch(LPCWSTR FileName, LPCWSTR Arguments, LPCWSTR BattleNetPath)
{
	std::wstring appRun;
	if (std::filesystem::exists(FileName)) {
		WCHAR newFilePath[MAX_PATH];
		GetModuleFileNameW(NULL, newFilePath, _countof(newFilePath));
		PathRemoveFileSpecW(newFilePath);
		wcscat_s(newFilePath, L"\\mapping\\ra3_1.12.game");

		std::ifstream originalFile(FileName, std::ios::binary | std::ios::in | std::ios::ate);
		std::streamsize size = originalFile.tellg();
		originalFile.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		if (originalFile.read(buffer.data(), size)) {
			buffer[0x126] |= 0b00100000;

			// Check file exists
			if (std::filesystem::exists(newFilePath)) {
				std::ifstream newFile(newFilePath, std::ios::binary | std::ios::in | std::ios::ate);
				std::streamsize newSize = newFile.tellg();
				newFile.seekg(0, std::ios::beg);
				std::vector<char> newBuffer(newSize);
				newFile.read(newBuffer.data(), newSize);
				if (newBuffer != buffer) {
					newBuffer.clear();
					newFile.close();
					goto writeFile;
				}
				else {
					newBuffer.clear();
					newFile.close();
				}
			}
			else
			{
			writeFile:
				std::ofstream newFile(newFilePath, std::ios::binary | std::ios::out | std::ios::ate);
				for (size_t i = 0; i < buffer.size(); i++)
				{
					newFile << buffer[i];
				}
				newFile.close();
			}

			buffer.clear();
		}
		originalFile.close();
		appRun = newFilePath;
	}
	else {
		MessageBoxW(NULL, L"Have you installed Command and Conquer Red Alert 3?", L"Warning!", MB_OK);
		return 1;
	}

	// ready to start
	//MessageBoxW(NULL, FileName, L"test", MB_OK);
	//MessageBoxW(NULL, Arguments, L"test", MB_OK);
	//MessageBoxW(NULL, BattleNetPath, L"test", MB_OK);
	//MessageBoxW(NULL, appRun.c_str(), L"test", MB_OK);
	Sleep(400);
	appRun += L" ";
	appRun += Arguments;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessW(NULL, (LPWSTR)appRun.c_str(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si, &pi);
	//
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	// active BattleNet
	std::wstring BNPath = BattleNetPath;
	if (BNPath != L"") {
		WCHAR newLogPath[MAX_PATH];
		GetModuleFileNameW(NULL, newLogPath, _countof(newLogPath));
		PathRemoveFileSpecW(newLogPath);
		wcscat_s(newLogPath, L"\\logs");

		std::ofstream logFile(std::wstring(newLogPath)+L"\\NativeDll.dll.txt", std::ios::binary | std::ios::out | std::ios::ate);
		logFile.close();

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string BNlog = conv.to_bytes(newLogPath);
		/*
		char* utf8LogPath = new char[MAX_PATH];
		memset(utf8LogPath, 0, MAX_PATH);
		int toUTF8ret = WideCharToMultiByte(CP_UTF8, 0, newLogPath, -1, utf8LogPath, MAX_PATH, NULL, FALSE);
		std::string BNlog;
		if (toUTF8ret > 0) {
			BNlog = utf8LogPath;
		}
		else {
			BNlog = "logs";
		}
		delete[]utf8LogPath;
		*/
		Sleep(3000);
		NTSTATUS nt = RhInjectLibrary(pi.dwProcessId, 0, 0, (WCHAR*)BattleNetPath, NULL, (PVOID)BNlog.c_str(), (BNlog.size() + 1));
		if (nt != 0) {
			MessageBoxW(NULL, L"BattleNet failed to load, please launch RA3 again.", L"error", MB_OK);
			return 1;
		}
	}

	Sleep(500);
	return 0;
}
