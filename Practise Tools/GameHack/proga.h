#pragma once
#include "vector"
#include "Windows.h"
#include "TlHelp32.h"

DWORD GetProgId(const wchar_t* progName);
uintptr_t GetModuleBaseAddress(DWORD progId, const wchar_t* moduleName);
uintptr_t FindDMA(HANDLE hProg, uintptr_t ptr, std::vector<unsigned int> offset);
