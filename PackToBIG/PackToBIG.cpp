#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <codecvt>

#include <shlwapi.h>
#include "PackToBIG.h"

#pragma comment( lib, "shlwapi.lib" )

//Function to write a string to a char vector
void PushStringToVector(std::string strn, std::vector< BYTE >* bytes)
{
	for (int i = 0; i < strn.size(); i++)
	{
		bytes->push_back(strn[i]);
	}
	//Zero terminate
	bytes->push_back(0x0);
}

std::string WideToUTF8(const std::wstring& source)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(source);
}

//Converts a string to lower case
std::wstring __fastcall ConvertToLower(std::wstring in)
{
	std::wstring out;
	for (int i = 0; i < in.size(); ++i)
	{
		out += towlower(in[i]);
	}
	return out;
}

int __fastcall CheckExcludeFile(std::wstring in)
{
	for (size_t i = 0; i < v_excludeFile.size(); i++) {
		if (in == v_excludeFile[i])
		{
			return 1;
		}
	}
	return 0;
}

int __fastcall CheckIncludeFile(std::wstring in)
{
	for (size_t i = 0; i < v_includeFile.size(); i++) {
		if (in == v_includeFile[i])
		{
			return 1;
		}
	}
	return 0;
}

int __fastcall GetFindFile(std::wstring mainPath, std::wstring subPath)
{
	std::wstring inputPath = mainPath;
	if (subPath != L"") {
		inputPath += L"\\";
		inputPath += subPath;
	}

	WIN32_FIND_DATA fileData;
	HANDLE hFind = FindFirstFileW((inputPath + L"\\*").c_str(), &fileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::wcout << L"No find files!\n";
		return 0;
	}

	do
	{
		//Check if it is a control path:
		if (!std::wcscmp(fileData.cFileName, L".") || !std::wcscmp(fileData.cFileName, L".."))
		{
			continue;
		}

		//std::wstring filepath = inputPath + L"\\";
		std::wstring filepath = subPath;
		if (filepath != L"") {
			filepath += L"\\";
		}
		filepath += fileData.cFileName;

		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// exclude "assets"
			if (std::wcscmp(fileData.cFileName, L"assets")) {
				GetFindFile(mainPath, filepath.c_str());
			}
		}
		else {
			if (v_excludeFile.size() > 0) {
				std::wstring fileName = fileData.cFileName;
				size_t lastindex = fileName.find_last_of(L".");
				if (lastindex != std::wstring::npos)
				{
					std::wstring extension = fileName.substr(lastindex + 1, fileName.size() - lastindex);
					extension = ConvertToLower(extension);

					if (CheckExcludeFile(extension)) {
						continue;
					}
				}
			}
			else if (v_includeFile.size() > 0) {
				std::wstring fileName = fileData.cFileName;
				size_t lastindex = fileName.find_last_of(L".");
				if (lastindex != std::wstring::npos)
				{
					std::wstring extension = fileName.substr(lastindex + 1, fileName.size() - lastindex);
					extension = ConvertToLower(extension);

					if (!CheckIncludeFile(extension)) {
						continue;
					}
				}
			}

			if (checkTime.dwHighDateTime && checkTime.dwLowDateTime) {
				// fileData.ftLastWriteTime < checkTime
				if ((CompareFileTime(&fileData.ftLastWriteTime, &checkTime) * checkTimeType) < 0) {
					continue;
				}
			}

			v_filePath.push_back(filepath);
		}
	} while (FindNextFileW(hFind, &fileData) != 0);

	FindClose(hFind);

	return 1;
}

void __fastcall SetCheckedFileType(UINT32 in)
{
	switch (in)
	{
	case 2:
		v_includeFile.push_back(L"cdata");
		return;
	case 1:
		v_excludeFile.push_back(L"asset");
		return;
	default:
		return;
	}
}

fileInfoStore __fastcall WriteDataToBuffer(std::wstring mainPath, std::wstring dataPath, std::vector<BYTE>* pathBytes, std::vector<BYTE>* dataBytes)
{
	fileInfoStore out;
	out.pos = pathBytes->size();
	// write to file index
	for (int i = 0; i < 8; i++) {
		pathBytes->push_back(0);
	}
	PushStringToVector(WideToUTF8(dataPath), pathBytes);

	out.fileOffset = dataBytes->size();
	// write to data
	std::ifstream file(mainPath + dataPath, std::ios::binary | std::ios::ate | std::ios::in);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size)) {
		for (size_t i = 0; i < buffer.size(); i++) {
			dataBytes->push_back(buffer[i]);
		}
	}
	file.close();

	// get info
	out.fileSize = size;

	return out;
}

void __fastcall OutBIGFile(std::wstring inPath, std::wstring outPath)
{
	if (!GetFindFile(inPath, L"")) {
		goto returnFunc;
	}

	{
		UINT32 fileCount = v_filePath.size();

		// set header
		std::vector< BYTE > headerBytes(0x10);
		// header is "BIG4"
		headerBytes[0] = 0x42;
		headerBytes[1] = 0x49;
		headerBytes[2] = 0x47;
		headerBytes[3] = 0x34;
		// 0x4 is EOF
		// write file count
		WriteFileCountToByte(&headerBytes[8], &fileCount);
		// 0xC is path EOF


		UINT32 countFactor = 20;
		if (fileCount > 999) {
			countFactor = 100;
		}
		// get data
		std::wstring mainPath = inPath + L"\\";
		std::vector< BYTE > pathBytes;
		std::vector< BYTE > dataBytes;
		std::vector< fileInfoStore > v_fileInfo;
		for (size_t i = 0; i < fileCount; i++) {
			v_fileInfo.push_back(WriteDataToBuffer(mainPath, v_filePath[i], &pathBytes, &dataBytes));

			if (i > 0 && (i % countFactor == 0)) {
				std::wcout << i;
				std::wcout << L" files has been done.\n";
			}
		}

		std::wcout << fileCount;
		std::wcout << L" files has been done.\n";

		// push 8-bytes to file index
		// 4C 32 31 39 00 00 00 00
		pathBytes.push_back(0x4C);
		pathBytes.push_back(0x32);
		pathBytes.push_back(0x31);
		pathBytes.push_back(0x39);
		pathBytes.push_back(0);
		pathBytes.push_back(0);
		pathBytes.push_back(0);
		pathBytes.push_back(0);

		// update header
		UINT32 pathEOF = pathBytes.size() + 0x10;
		WriteFileCountToByte(&headerBytes[0xC], &pathEOF);
		UINT32 fileEOF = pathEOF + dataBytes.size();
		memcpy(&headerBytes[4], &fileEOF, 4U);

		// update file index info
		for (size_t i = 0; i < v_fileInfo.size(); i++) {
			UINT32 indexPos = v_fileInfo[i].pos;

			UINT32 fileOffset = v_fileInfo[i].fileOffset + pathEOF;
			WriteFileCountToByte(&pathBytes[indexPos], &fileOffset);

			UINT32 fileSize = v_fileInfo[i].fileSize;
			WriteFileCountToByte(&pathBytes[indexPos + 4], &fileSize);
		}

		// out file
		std::ofstream newFile(outPath, std::ios::binary | std::ios::out | std::ios::ate);

		for (size_t i = 0; i < headerBytes.size(); i++)
		{
			newFile << headerBytes[i];
		}

		for (size_t i = 0; i < pathBytes.size(); i++)
		{
			newFile << pathBytes[i];
		}

		for (size_t i = 0; i < dataBytes.size(); i++)
		{
			newFile << dataBytes[i];
		}

		newFile.close();

		headerBytes.clear();
		pathBytes.clear();
		dataBytes.clear();
		std::wcout << outPath + L" has been packed.\n";
	}

returnFunc:
	// end clear
	v_filePath.clear();
	v_excludeFile.clear();
	v_includeFile.clear();
}

int wmain(int argc, wchar_t* argv[])
{
	checkTime.dwLowDateTime = 0;
	checkTime.dwHighDateTime = 0;
    if (argc < 2) {
		/*
		wchar_t testTime[] = L"2023/7/28";
		SYSTEMTIME sysTime;
		memset(&sysTime, 0, sizeof(SYSTEMTIME));
		swscanf_s(testTime, L"%hd/%hd/%hd", &sysTime.wYear, &sysTime.wMonth, &sysTime.wDay);
		SystemTimeToFileTime(&sysTime, &checkTime);
		
		//LocalFileTimeToFileTime(&testdwad, &testdwad2);
		//checkTimeType = -1;
		*/

		WCHAR fpath[MAX_PATH];
		GetModuleFileNameW(NULL, fpath, _countof(fpath));
		PathRemoveFileSpecW(fpath);
		//std::wcout << fpath;
		//std::wcout << L"\n";

		//v_excludeFile.push_back(L"user");
		//v_includeFile.push_back(L"filters");

		if (GetFindFile(fpath, L"")) {
			for (size_t i = 0; i < v_filePath.size(); i++) {
				std::wcout << L"find file: ";
				std::wcout << v_filePath[i].c_str();
				std::wcout << L"\n";
			}
			v_filePath.clear();
		}

		system("pause");
	}
	else if (argc > 5 && !lstrcmpW(argv[1], L"-outfile")) {
		SetCheckedFileType(std::stoi(argv[4]));

		if (lstrcmpW(argv[5], L"none")) {
			if (argc > 6) {
				SYSTEMTIME sysTime;
				memset(&sysTime, 0, sizeof(SYSTEMTIME));
				swscanf_s(argv[6], L"%hd/%hd/%hd", &sysTime.wYear, &sysTime.wMonth, &sysTime.wDay);
				SystemTimeToFileTime(&sysTime, &checkTime);

				if (!lstrcmpW(argv[5], L"+")) {
					checkTimeType = 1;
				}
				else if (!lstrcmpW(argv[5], L"-")) {
					checkTimeType = -1;
				}
			}
		}

		OutBIGFile(std::wstring(argv[2]), std::wstring(argv[3]));
		//system("pause");
	}

	// p1 is type
	// p2 is input path
	// p3 is out path
	// p4 is check file type
	// p5 is check time type
	// p6 is check time

	return 0;
}
