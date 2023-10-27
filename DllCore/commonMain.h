#pragma once

void __fastcall hookFunctionGroup();
DWORD WINAPI setFrameTo60();
bool __fastcall GetFunctionAddress();
void SetCPUAffinity();

void mainInjectionExecution();
void mainInjectionSetting(LPCWSTR iniPath);
