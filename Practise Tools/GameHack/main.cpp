/*
;|**********************************************************************;
;* Project           : CPP-Language_practise
;*
;* Program name      : GameHack.cpp
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 18/10/2020
;*
;* Version           : internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Game hack && PE tool
;*
;* Revision History  :
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 18102020    lst97       1      First release
;* 19102020    lst97       2      Add newsection Function
;* 20102020    lst97       3      add other way to inject table, free function
;* 21102020    lst97       4      add expand section function
;* 24102020    lst97       5      can view export table and its' functions
;* 25102020    lst97       6      can view relocation table info
;* 26102020    lst97       7      can view import table info
;* 28102020    lst97       8      add inject dll function
;*
;* Known Issue       :
;*
;* TODO              :
;*
;|**********************************************************************;
*/

#include "proga.h"
#include "petool.h"
#include "time.h"

int main() {
	// PE Tool Logic
	printf("[4] File & Image buffer\n");
	PETool* pPetool = PETool_new((char*)"C:\\Users\\DEBUG\\Desktop\\C_Debug.exe");
	if (pPetool == NULL) {
		printf("File not found / Heap Init FAILE...\n\n");
		getchar();
		return 0;
	}
	FBuffer* pFileBuffer = NULL;
	unsigned int memory_size = pPetool->image.compress((unsigned int*)&pFileBuffer);

	if (pPetool->validator.filecmp(pFileBuffer)) {
		printf("Compress FAIL...\n\n");
		getchar();
		return 0;
	}
	printf("Compress PASS...\n");
	if (pPetool->fexport((char*)"C_Debug (cpy).exe", 0) == -1) {
		printf("%s already exist!\n\n", "C_Debug (cpy).exe");
	} else {
		printf("Create SUCCESS!\n\n");
	}


	/*printf("[5] Insert section\n");
	char bcode[] = { 0x12, 0x13 };
	if (pPetool->file.newsection(".inject", bcode, 2, EXE_CHARACTERISTIC) != -1) {
		printf("Inject PASS...\n");

		memory_size = pPetool->image.compress((unsigned int*)&pFileBuffer);
		if (pPetool->fexport(pFileBuffer, (char*)"C_Debug (injected).exe", memory_size, 0) == -1) {
			printf("%s already exist!\n", "C_Debug (injected).exe");
		} else {
			printf("Create SUCCESS!\n\n");
			free(pFileBuffer);
		}
	} else {
		printf("Inject FAIL!\n\n");
	}*/

	/*printf("[6] Expand last section\n");
	if (pPetool->file.expandsection(0x1000) != NULL) {
		memory_size = pPetool->image.compress((unsigned int*)&pFileBuffer);
		if (pPetool->fexport(pFileBuffer, (char*)"C_Debug (expanded).exe", memory_size, 0) == -1) {
			printf("%s already exist!\n\n", "C_Debug (expanded).exe");
		} else {
			printf("Create SUCCESS!\n\n");
			free(pFileBuffer);
		}
	} else {
		printf("Expand FAIL!\n\n");
	}*/

	/*printf("[7] Export Table\n");
	PETool* pPetool_export_test = PETool_new((char*)"C:\\Users\\DEBUG\\Desktop\\DebugTools\\ReClass.NET\\x86\\NativeCore.dll");
	EXPORT_FUNCTION* pExportFunctions = pPetool_export_test->pHeader->getExportFunctions();
	if (pExportFunctions != 0) {
		unsigned int stop_flag;
		unsigned int wecx = 0;
		while (true) {
			stop_flag = (pExportFunctions + wecx)->function_addr + (pExportFunctions + wecx)->ordinal + (unsigned int)(pExportFunctions + wecx)->pName;
			if (stop_flag == 0) {
				break;
			}

			if ((pExportFunctions + wecx)->pName != 0) {
				printf("Function[%d]:\nAddress:\t%08X\nOrdinal:\t%04X\nName:\t\t%s\n\n", \
					wecx, \
					(pExportFunctions + wecx)->function_addr, \
					(pExportFunctions + wecx)->ordinal, \
					(pExportFunctions + wecx)->pName
				);
			} else {
				printf("Function[%d]:\nAddress:\t%08X\nOrdinal:\t%04X\nName:\t\t-\n\n", \
					wecx, \
					(pExportFunctions + wecx)->function_addr, \
					(pExportFunctions + wecx)->ordinal
				);
			}
			wecx++;
		}
	} else {
		printf("No export table found!\n\n");
	}
	pPetool_export_test->pefree(pPetool_export_test);*/

	/*printf("[8] Base Relocation Table\n\n");
	RELOC_BLOCK* pRelocBlock = pPetool->pHeader->getRelocation();
	unsigned int wecx = 0;
	unsigned int address_count;
	unsigned int remainder;
	unsigned short data;
	while (true) {
		if ((pRelocBlock + wecx)->pData == 0) {
			break;
		}
		address_count = ((pRelocBlock + wecx)->reloc.SizeOfBlock - 0x08) / 2;
		remainder = ((pRelocBlock + wecx)->reloc.SizeOfBlock - 0x08) % 2;
		if (remainder) {
			address_count++;
		}
		printf("Block_Address[%08X]\n", (pRelocBlock + wecx)->reloc.VirtualAddress);
		for (unsigned int fecx = 0; fecx < address_count; fecx++) {
			data = *((pRelocBlock + wecx)->pData + fecx);
			printf("%04X,\t%d\n", data & 0x0FFF, ((data & (unsigned short)0xF000)) >> 12);
		}
		free((pRelocBlock + wecx)->pData);
		printf("\n");
		wecx++;
	}
	free(pRelocBlock);*/

	/*printf("[10] Import table\n\n");
	IMPORT_FUNCTIONS* pImport_Functions = pPetool->pHeader->getImportFunctionNames();
	IMPORT_FUNCTION_NAME* pImportFunctionNames;
	unsigned int wecx = 0;
	unsigned int wedx = 0;
	while ((pImport_Functions + wecx)->pDllName != 0) {
		printf("%s\n", (pImport_Functions + wecx)->pDllName);
		pImportFunctionNames = (pImport_Functions + wecx)->pImportFunctionNames;
		while ((pImportFunctionNames + wedx)->pName != 0) {
			if ((pImportFunctionNames + wedx)->Hit == 0) {
				printf("Ordinary:\t%05X\n", (pImportFunctionNames + wedx)->ordinary);
			} else {
				printf("FnName:\t[%05X] - %s\n", (pImportFunctionNames + wedx)->Hit, (pImportFunctionNames + wedx)->pName);
			}
			wedx++;
		}
		printf("\n");
		wedx ^= wedx;
		wecx++;
	}*/

	/*
	// Code did not tested yet 27/10/2020
	printf("[11] Bound table\n\n");
	char buffer[26] = { 0 };
	time_t tm_info;

	BOUND_TABLE* pBoundTable = pPetool->pHeader->getBoundTableInfo();
	if (pBoundTable != 0) {
		unsigned int table_length = pBoundTable->NumberOfModuleForwarderRefs + 1;
		for (unsigned int fecx = 0; fecx < table_length; fecx++) {
			tm_info = (pBoundTable + fecx)->TimeDataStamp;
			time(&tm_info);
			strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", localtime(&tm_info));
			printf("DLL Name:\t%s\nTime:\t%s\n\n", (pBoundTable + fecx)->pName, buffer);
		}
	} else {
		printf("Bound table not found!\n\n");
	}*/

	printf("[12] Dll Injection\n\n");
	pPetool->dllInjection((char*)"inject.dll", (char*)"fnInject");
	if (pPetool->fexport((char*)"C_Debug (dllinject).exe", 0) == -1) {
		printf("%s already exist!\n\n", "C_Debug (dllinject).exe");
	} else {
		printf("Create SUCCESS!\n\n");
	}

	//// memory leak test
	//while (1) {
	//	pPetool->pefree(pPetool);
	//	pPetool = PETool_new((char*)"C:\\Users\\DEBUG\\Desktop\\C_Debug.exe");
	//}

	// Game Hack Logic
	const wchar_t progName[] = L"ac_client.exe";
	DWORD progId = GetProgId(progName);
	if (progId == 0) {
		printf("Program no found!");
		getchar();
		return 0;
	}
	uintptr_t imageBase = GetModuleBaseAddress(progId, progName);
	HANDLE hProg = OpenProcess(PROCESS_ALL_ACCESS, NULL, progId);

	std::vector<unsigned int> offset = { 0x384, 0x14, 0};
	uintptr_t targetAddr = FindDMA(hProg, imageBase + 0x109B74, offset);
	printf("Image Base: %08X\nTarget Address: %08X\n\n", imageBase, targetAddr);

	int newValue = 999;
	WriteProcessMemory(hProg, (BYTE*)targetAddr, &newValue, sizeof(newValue), nullptr);

	printf("New Value: %d\n\nTrainer END...", newValue);

	getchar();
	return 0;
}
