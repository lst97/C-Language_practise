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
;*
;* Known Issue       :
;*
;* TODO              :
;*
;|**********************************************************************;
*/

#include "proga.h"
#include "petool.h"

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

	if (pPetool->fexport(pFileBuffer, (char*)"C_Debug (cpy).exe", memory_size, 0) == -1) {
		printf("%s already exist!\n\n", "C_Debug (cpy).exe");
		
	} else {
		printf("Create SUCCESS!\n\n");
		free(pFileBuffer);
	}

	printf("[7] Export Table\n");
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
			}
			else {
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
	pPetool_export_test->pefree(pPetool_export_test);

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
