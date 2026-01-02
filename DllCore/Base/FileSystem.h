#pragma once
#include <vector>


namespace RA3::FS {
	typedef struct MemoryWriteFile_t {
		void* vf_table;
	}*pMemoryWriteFile;

	typedef void*(__fastcall* CallFunc_FileSystemCreateFile)(pMemoryWriteFile pIn, void* callFunc, const char* fileName, int openMode, void* a3);

	typedef struct GameFile_t {
        struct VirtualFunctionTable
        {
            template<typename Ret, typename... Args>
            using Vfp = Ret(__thiscall*)(GameFile_t*, Args...);

            Vfp<void> destructor;
            Vfp<bool, char const*, int> open;
            Vfp<void> close;
            Vfp<int, void*, int> read;
            Vfp<int, void const*, int> write;
            Vfp<int, int, int> seek;
            void* nextLine;
            void* scanInt;
            void* scanReal;
            void* scanString;
            void* print;
            Vfp<int> size;
            Vfp<int> position;
            Vfp<void*> readAllAndClose;
            void* toRamFile;
        };

        VirtualFunctionTable* vf_table;
	}*pGameFile;


	void __fastcall G_FileSystem_Initialize(uintptr_t hmodEXE, int isNewSteam);
    pGameFile __fastcall GetFile(char const* fileName, int openMode);
	std::vector<char> __fastcall ReadToBuffer(const char* fileName);
}
