#include "../pch.h"
#include "FileSystem.h"

namespace RA3::FS {
	auto _F_VirtualFileSystem = (pMemoryWriteFile*)0xCD8918;
	auto _call_FileSystemCreateFile = (CallFunc_FileSystemCreateFile)0x4D1810;

	void __fastcall G_FileSystem_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_VirtualFileSystem = (pMemoryWriteFile*)0xCD8898;
			_call_FileSystemCreateFile = (CallFunc_FileSystemCreateFile)0x4D1B50;
		}
	}

	pGameFile __fastcall GetFile(char const* fileName, int openMode)
	{
		auto vfs = *_F_VirtualFileSystem;
		return (pGameFile)_call_FileSystemCreateFile(vfs, 0, fileName, openMode, 0);
	}

	std::vector<char> __fastcall ReadToBuffer(const char* fileName)
	{
		std::vector<char> buffer;
		auto pFile = GetFile(fileName, 0x441);

		if (pFile) {
			int size = pFile->vf_table->size(pFile);
			buffer.resize(size);
			pFile->vf_table->read(pFile, buffer.data(), size);
			pFile->vf_table->close(pFile);
		}

		return buffer;
	}
}
