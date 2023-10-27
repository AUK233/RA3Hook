#pragma once

extern "C" _declspec(dllexport) int DirectGameLaunch(LPCWSTR FileName, LPCWSTR Arguments,
													LPCWSTR DllPath, PVOID dllData, ULONG dataSize,
													LPCWSTR BattleNetPath, PVOID BNlog, ULONG logSize);

extern "C" _declspec(dllexport) int PowerfulGameLaunch(LPCWSTR FileName, LPCWSTR Arguments, LPCWSTR BattleNetPath);
