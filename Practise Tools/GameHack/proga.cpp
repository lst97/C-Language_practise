#include "proga.h"

DWORD GetProgId(const wchar_t* progName) {
	DWORD progId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 progEntry;
		progEntry.dwSize = sizeof(progEntry);

		if (Process32First(hSnap, &progEntry)) {
			do {
				if (!_wcsicmp(progEntry.szExeFile, progName)) {
					progId = progEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &progEntry));
		}
	}
	CloseHandle(hSnap);
	return progId;
}

uintptr_t GetModuleBaseAddress(DWORD progId, const wchar_t* moduleName) {
	uintptr_t moduleBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, progId);

	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);
		if (Module32First(hSnap, &moduleEntry)) {
			do {
				if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
					moduleBaseAddr = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}
			} while (true);
		}
	}
	CloseHandle(hSnap);
	return moduleBaseAddr;
}

uintptr_t FindDMA(HANDLE hProg, uintptr_t ptr, std::vector<unsigned int> offset) {
	uintptr_t addr = ptr;
	for (unsigned int i = 1; i <= offset.size(); i++) {
		printf("D-addr: %08X\n", addr);
		ReadProcessMemory(hProg, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offset[i];
	}
	printf("D-addr: %08X\n\n", addr);
	return addr;
}
