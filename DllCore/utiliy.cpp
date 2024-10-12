#include "pch.h"
#include <format>

#include "utiliy.h"

// Injects hook into game process
void WriteHookToProcess(void* addr, void* data, size_t len) {
	DWORD oldProtect;
	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, data, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
}

// Update game's original functions with 5 bytes
void hookGameBlock(void* targetAddr, uintptr_t dataAddr) {
	uint8_t hookFunction[] = {
		0xE9, 0x00, 0x00, 0x00, 0x00
	};
	int offset = dataAddr - ((uintptr_t)targetAddr + 5);
	memcpy(&hookFunction[1], &offset, 4U);

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// Update game's original functions with 7 bytes
void hookGameBlock7(void* targetAddr, uintptr_t dataAddr) {
	uint8_t hookFunction[] = {
		0xB8, 0x00, 0x00, 0x00, 0x00, // mov eax, offset
		0xFF, 0xE0                    // jmp eax
	};
	memcpy(&hookFunction[1], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// Check specific address to confirm RA3 version
bool checkRA3Address(std::uintptr_t address)
{
	return std::strncmp(reinterpret_cast<char const*>(address), "RedAlert", 8) == 0;
}

void hookGameCall(void* targetAddr, uintptr_t dataAddr)
{
	uint8_t hookFunction[] = {
		0xE8, 0x00, 0x00, 0x00, 0x00
	};
	int offset = dataAddr - ((uintptr_t)targetAddr + 5);
	memcpy(&hookFunction[1], &offset, 4U);

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}
