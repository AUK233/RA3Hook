#pragma once

//
std::vector<std::wstring> v_excludeFile;
std::vector<std::wstring> v_includeFile;
//
std::vector<std::wstring> v_filePath;
//
FILETIME checkTime;
int checkTimeType = 1;
//
struct fileInfoStore
{
	UINT32 fileOffset;
	UINT32 fileSize;
	UINT32 pos;
	UINT32 align;
};

//void __fastcall WriteFileCountToByte(void* bytes, void* value);
//Function to write a string to a char vector
void PushStringToVector(std::string strn, std::vector< BYTE >* bytes);
//
std::string WideToUTF8(const std::wstring& source);
// in asm
// extern "C" 
void __fastcall WriteFileCountToByte(void* bytes, void* value);
//Converts a string to lower case
std::wstring __fastcall ConvertToLower(std::wstring in);
//
int __fastcall CheckExcludeFile(std::wstring in);
//
int __fastcall CheckIncludeFile(std::wstring in);
//
int __fastcall GetFindFile(std::wstring mainPath, std::wstring subPath);
//
void __fastcall SetCheckedFileType(UINT32 in);
//
fileInfoStore __fastcall WriteDataToBuffer(std::wstring mainPath, std::wstring dataPath, std::vector< BYTE >* pathBytes, std::vector< BYTE >* dataBytes);
//
void __fastcall OutBIGFile(std::wstring inPath, std::wstring outPath);

