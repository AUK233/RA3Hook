// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <format>
#include <span>

uintptr_t hmodEXE;

void WriteHookToProcess(void* addr, void* data, size_t len) {
	DWORD oldProtect;
	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, data, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
}

void hookGameBlock(void* targetAddr, uintptr_t dataAddr) {
	uint8_t hookFunction[] = {
		0xE9, 0x00, 0x00, 0x00, 0x00
	};
	int offset = dataAddr - ((int)targetAddr + 5);
	memcpy(&hookFunction[1], &offset, 4U);

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// need 7 bytes!
void hookGameBlock7(void* targetAddr, uintptr_t dataAddr) {
	uint8_t hookFunction[] = {
		0xB8, 0x00, 0x00, 0x00, 0x00, // mov eax, offset
		0xFF, 0xE0                    // jmp eax
	};
	memcpy(&hookFunction[1], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

void __fastcall test111(int ptr)
{
	// Target Position
	float opx = *(float*)(ptr + 0x3C);
	float opy = *(float*)(ptr + 0x40);
	float opz = *(float*)(ptr + 0x44);
	// The difference between the target and its own position.
	float spx = opx - *(float*)(ptr + 0x30);
	float spy = opy - *(float*)(ptr + 0x34);
	// Read EndOffset's z as radian.
	int posOffsetPtr = *(int*)(*(int*)(ptr + 4) + 0x24);
	if (posOffsetPtr)
	{
		float ofsRadian = *(float*)(posOffsetPtr + 8);
		if (ofsRadian) {
			float dx = cos(ofsRadian) * spx - sin(ofsRadian) * spy;
			float dy = cos(ofsRadian) * spy + sin(ofsRadian) * spx;
			spx = dx;
			spy = dy;
			opz -= ofsRadian;
			opz += 0.01f;
			*(float*)(ptr + 0x44) = opz;
		}
	}

	float spz = opz - *(float*)(ptr + 0x38);

	float fr = (spz * spz) + (spy * spy) + (spx * spx);
	float DeltaX;
	float DeltaY;
	float DeltaZ;
	if (fr <= 0.0001f)
	{
		DeltaX = 1.0f;
		DeltaY = 0.0f;
		DeltaZ = 0.0f;
	}
	else
	{
		float scale;
		if (fr == 0.0f)
		{
			scale = 0.0f;
		}
		else
		{
			scale = 1.0f / sqrt(fr);
		}
		DeltaX = spx * scale;
		DeltaY = spy * scale;
		DeltaZ = spz * scale;
	}
	
	float Radius = *(float*)(*(int*)(ptr + 4) + 0x2C);
	float ofsx;
	float ofsy;
	float ofsz;
	if (Radius < -0.1f) {
		// vertical sweep, one way
		*(char*)(ptr + 0xBC) = 0;
		ofsx = DeltaX * Radius;
		ofsy = DeltaY * Radius;
		ofsz = 0.0f;
	}
	else {
		//float ofsz = ((DeltaX * 0.0f) - (DeltaY * 0.0f)) * Radius;
		// horizontal sweep, both directions
		ofsx = DeltaY * Radius;
		ofsy = -DeltaX * Radius;
		ofsz = 0.0f;
	}

	// sweep end position
	*(float*)(ptr + 0x98) = opx - ofsx;
	*(float*)(ptr + 0x9C) = opy - ofsy;  
	*(float*)(ptr + 0xA0) = opz - ofsz;
	// sweep start position
	*(float*)(ptr + 0xA4) = opx + ofsx;
	*(float*)(ptr + 0xA8) = opy + ofsy;
	*(float*)(ptr + 0xAC) = opz + ofsz;
}

// void __fastcall ReadInt32(int ptr);
__declspec(naked) void __fastcall ReadInt32(int ptr)
{
	__asm {
		mov ecx, esi
		call test111
		pop edi
		pop esi
		add esp, 0x18
		ret 0x8
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
	std::wstring message = std::format(L"启动器进程 ID：{}；注入消息：{}，是否结束进程？", input->HostPID, inputString);
	if (MessageBoxW(nullptr, message.c_str(), L"Dll1", MB_YESNO) == IDYES)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}
	MessageBox(NULL, L"Test dll injection", L"Test", MB_OK);
	*/
	hmodEXE = (uintptr_t)GetModuleHandleW(NULL);

	// ra3_1.12.game+2DDE95
	unsigned char beeAccuracy[] = {
		0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2DDE95), &beeAccuracy, 6U);

	unsigned char nop3[] = { 0x90, 0x90, 0x90 };
	hookGameBlock((void*)(hmodEXE + 0x3C3ED7), (uintptr_t)ReadInt32);
	//WriteHookToProcess((void*)(hmodEXE + 0x3C3ED7 + 7), &nop3, 3U);

	// need
	//FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}