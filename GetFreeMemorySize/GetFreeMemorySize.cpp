
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include "GetFreeMemorySize.h"
#include <string>

// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

void GetFreeMemorySize()
{
    HANDLE process = nullptr;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bResult = Process32FirstW(hSnapshot, &pe32);
    while (bResult)
    {
        if (std::wstring(pe32.szExeFile) == L"ra3_1.12.game") {
            std::wcout << L"Find ra3_1.12.game!\n";
            process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            break;
        }

        bResult = Process32NextW(hSnapshot, &pe32);
    }
    CloseHandle(hSnapshot);

    UINT32 freeMemSize = 0;
    UINT32 useMemSize = 0;
    if (process) {
        MEMORY_BASIC_INFORMATION memInfo;
        ZeroMemory(&memInfo, sizeof(MEMORY_BASIC_INFORMATION));

        char* memAddr = (char*)0x10000;
        while (VirtualQueryEx(process, (void*)memAddr, &memInfo, sizeof(MEMORY_BASIC_INFORMATION))) {
            if (memInfo.State == MEM_FREE) {
                freeMemSize += memInfo.RegionSize;
            }
            else if (memInfo.State == MEM_COMMIT) {
                useMemSize += memInfo.RegionSize;
            }

            memAddr = (char*)memInfo.BaseAddress + memInfo.RegionSize;
        }

        CloseHandle(process);
        freeMemSize /= (1024 * 1024);
        useMemSize /= (1024 * 1024);
    }

    std::wcout << L"Free memory is: ";
    std::wcout << freeMemSize;
    std::wcout << L"MiB.\nCommit memory is: ";
    std::wcout << useMemSize;
    std::wcout << L"MiB.\n";
}

void GetCPUInformation()
{
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pCPUInfo = 0;
    DWORD lCPUInfo = 0;
    GetLogicalProcessorInformation(pCPUInfo, &lCPUInfo);
    pCPUInfo = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(lCPUInfo);
    if (pCPUInfo) {
        DWORD byteOffset = 0;
        DWORD logicalProcessorCount = 0;
        DWORD numaNodeCount = 0;
        DWORD processorCoreCount = 0;
        DWORD processorL1CacheCount = 0;
        DWORD processorL2CacheCount = 0;
        DWORD processorL3CacheCount = 0;
        DWORD processorPackageCount = 0;
        PCACHE_DESCRIPTOR Cache;

        GetLogicalProcessorInformation(pCPUInfo, &lCPUInfo);
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptrCPU = pCPUInfo;
        while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= lCPUInfo) {
            switch (ptrCPU->Relationship) {
            case RelationNumaNode:
                // Non-NUMA systems report a single record of this type.
                numaNodeCount++;
                break;

            case RelationProcessorCore:
                processorCoreCount++;

                // A hyperthreaded core supplies more than one logical processor.
                logicalProcessorCount += CountSetBits(ptrCPU->ProcessorMask);
                break;

            case RelationCache:
                // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
                Cache = &ptrCPU->Cache;
                if (Cache->Level == 1)
                {
                    processorL1CacheCount++;
                }
                else if (Cache->Level == 2)
                {
                    processorL2CacheCount++;
                }
                else if (Cache->Level == 3)
                {
                    processorL3CacheCount++;
                }
                break;

            case RelationProcessorPackage:
                // Logical processors share a physical package.
                processorPackageCount++;
                break;

            default:
                break;
            }

            byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
            ptrCPU++;
        }

        std::wcout << L"\nGetLogicalProcessorInformation results:";
        std::wcout << L"\nNumber of NUMA nodes: " + std::to_wstring(numaNodeCount);
        std::wcout << L"\nNumber of physical processor packages: " + std::to_wstring(processorPackageCount);
        std::wcout << L"\nNumber of processor cores: " + std::to_wstring(processorCoreCount);
        std::wcout << L"\nNumber of logical processors: " + std::to_wstring(logicalProcessorCount);
        std::wcout << L"\nNumber of processor L1 caches:" + std::to_wstring(processorL1CacheCount);
        std::wcout << L"\nNumber of processor L2 caches:" + std::to_wstring(processorL2CacheCount);
        std::wcout << L"\nNumber of processor L3 caches:" + std::to_wstring(processorL3CacheCount);
        std::wcout << L"\n\n";

        free(pCPUInfo);
    }
}

int wmain(int argc, wchar_t* argv[])
{
    GetFreeMemorySize();

    //GetCPUInformation();

    system("pause");
}
