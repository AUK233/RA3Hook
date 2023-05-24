#pragma once

// Injects hook into game process
void WriteHookToProcess(void* addr, void* data, size_t len);
// Update game's original function (Relative Jump)
void hookGameBlock(void* targetAddr, uintptr_t dataAddr);
// Update game's original function with 7 bytes (Absolute Jump)
void hookGameBlock7(void* targetAddr, uintptr_t dataAddr);
// Check specific address to confirm RA3 version
bool checkRA3Address(std::uintptr_t address);
