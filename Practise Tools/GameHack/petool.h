#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "String.h"
// if the funtion return value is 0xFFFFFFF: init heap FAIL

#ifndef _WINDOWS_
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#endif // !_WINDOWS_

// size
#define SECTION_SIZE 0x28
#define OPTHEADER_SIZE 0x60
#define DATADIRARR_SIZE 0x80
#define DOSH_SIZE 0x40
#define STUBNECESSARY_SIZE 0x40
#define EXPORTFUNCTION_SIZE 0x0A

// PE Header Offset
#define SECTIONNUM_OFFSET 0x06
#define OPTHSIZE_OFFSET 0x14
#define OPTH_OFFSET 0x18
#define IFANEW_OFFSET 0x3C
#define IMAGESIZE_OFFSET 0x38

// Section Header Offset
#define RAWSIZE_OFFSET 0x10
#define RAWPTR_OFFSET 0x14
#define VIRTUALADDR_OFFSET 0x0C
#define EXE_CHARACTERISTIC 0x60000020
#define MISC_OFFSET 0x08

// Data dir
#define EXPORTTABLE_SIZE 0x28

// Uninit
#define UNINIT_STACK 0xCCCCCCCC
#define UNINIT_HEAP 0xCDCDCDCD

typedef unsigned char Buffer;
typedef Buffer FBuffer;
typedef Buffer IBuffer;
typedef Buffer HBuffer;

// PE struct
struct DATA_DIRECTORY {
	DWORD VirtualAddress;
	DWORD Size;
};

struct DATA_DIRECTORY_ARRAY {
	DATA_DIRECTORY Export;
	DATA_DIRECTORY Import;
	DATA_DIRECTORY Resource;
	DATA_DIRECTORY Exception;
	DATA_DIRECTORY Security;
	DATA_DIRECTORY BaseRelocation;
	DATA_DIRECTORY Debug;
	DATA_DIRECTORY Copyright;
	DATA_DIRECTORY GlobalPtr;
	DATA_DIRECTORY TLS;
	DATA_DIRECTORY LoadConfiguration;
	DATA_DIRECTORY BoundImport;
	DATA_DIRECTORY ImportAddress;
	DATA_DIRECTORY DelayLoadImport;
	DATA_DIRECTORY COM;
	DATA_DIRECTORY Reserved;
};

struct OPT_HEADER {
	WORD Magic;
	BYTE MajorLinkerVersion;
	BYTE MinorLinkerVersion;
	DWORD SizeOfCode;
	DWORD SizeOfInitializedData;
	DWORD SizeOfUninitializedData;
	DWORD AddressOfEntryPoint;
	DWORD BaseOfCode;
	DWORD BaseOfData;
	DWORD ImageBase;
	DWORD SectionAlignment;
	DWORD FileAlignment;
	WORD MajorOperatingSystemVersion;
	WORD MinorOperatingSystemVersion;
	WORD MajorImageVersion;
	WORD MinorImageVersion;
	WORD MajorSubsystemVersion;
	WORD MinorSubsystemVersion;
	DWORD Win32VersionValue;
	DWORD SizeOfImage;
	DWORD SizeOfHeaders;
	DWORD CheckSum;
	WORD Subsystem;
	WORD DllCharacteristics;
	DWORD SizeOfStackReserve;
	DWORD SizeOfStackCommit;
	DWORD SizeOfHeapReserve;
	DWORD SizeOfHeapCommit;
	DWORD LoaderFlags;
	DWORD NumberOfRvaAndSizes;
	DATA_DIRECTORY_ARRAY DataDirArray;
};

struct BASE_RELOCATION {
	DWORD VirtualAddress;
	DWORD SizeOfBlock;
};

// Section struct
struct SECTION_HEADER {
	BYTE Name[8];
	union {
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	WORD  NumberOfRelocations;
	WORD  NumberOfLinenumbers;
	DWORD Characteristics;
};

//
// Data Dir struct
//
// Export struct
struct EXPORT_TABLE {
	DWORD Characteristics;
	DWORD TimeDateStamp;
	WORD MajorVersion;
	WORD MinorVersion;
	DWORD Name;
	DWORD Base;
	DWORD NumberOfFunctions;
	DWORD NumberOfName;
	DWORD AddressOfFunctions;
	DWORD AddressOfNames;
	DWORD AddressOfNameOrdinals;
};
// Import struct
struct IMPORT_DESCRIPTOR {
	union {
		DWORD Characteristics;
		DWORD OriginalFirstThunk;
	};
	DWORD TimeDateStamp;
	DWORD ForwarderChain;
	DWORD Name;
	DWORD FirstThunk;
};

// Program struct
struct EXPORT_FUNCTION {
	unsigned short ordinal;
	char* pName;
	unsigned int function_addr;
};

struct RELOC_BLOCK {
	BASE_RELOCATION reloc;
	unsigned short* pData;
};

struct IMPORT_FUNCTION_NAME {
	WORD Hit;
	union {
		char* pName;
		unsigned int ordinary;
	};
};

struct IMPORT_FUNCTIONS {  // Require a free function 26/10/2020 lst97;
	char* pDllName;
	IMPORT_FUNCTION_NAME* pImportFunctionNames;
};

struct BOUND_TABLE {
	unsigned int TimeDataStamp;
	char* pName;
	unsigned short NumberOfModuleForwarderRefs;
};

EXPORT_FUNCTION* getExportFunctions();
RELOC_BLOCK* getRelocation();
IMPORT_FUNCTIONS* getImportFunctionNames();
BOUND_TABLE* getBoundTableInfo();
struct Header {
	unsigned short SizeOfOptionalHeader;
	unsigned short NumberOfSection;
	HBuffer* pBuffer;
	OPT_HEADER* pOptheader;
	unsigned int e_lfanew;
	unsigned int SizeOfHeaders;
	unsigned int SizeOfImage;

	unsigned int sectionTables_offset;
	unsigned int file_size;

	int (*refresh)();
	EXPORT_FUNCTION* (*getExportFunctions)();
	RELOC_BLOCK* (*getRelocation)();
	IMPORT_FUNCTIONS* (*getImportFunctionNames)();
	BOUND_TABLE* (*getBoundTableInfo)();

};

FBuffer* fcreate();
//int fwrite(unsigned int offset, unsigned int size);
int newsection(const char* section_name, unsigned int bcode_size, unsigned int characteristics);
//int inject(const char* section_name, char* bcode);
unsigned int falignmentcalc(unsigned int size);
int fmodify(unsigned int offset, unsigned int size, unsigned char* data);
int expandsection(unsigned int size);
//int mergesection();
struct FileObj {
	Header* pHeader;
	FBuffer* pBuffer;
	FBuffer* (*create)();
	IBuffer* (*expand)();
	int (*newsection)(const char*, unsigned int, unsigned int);
	int (*expandsection)(unsigned int size);
	//int (*mergesection)();
	//int (*inject)(const char*, char*);
	int (*modify)(unsigned int, unsigned int, unsigned char*);
	unsigned int (*alignmentcalc)(unsigned int);
};

int iwrite(unsigned int offset, unsigned int size);
unsigned int icompress(unsigned int* ptr);
unsigned int ialignmentcalc(unsigned int size);
struct ImageObj {
	Header* pHeader;
	IBuffer* pBuffer;
	IBuffer* (*create)();
	int (*write)(unsigned int, unsigned int);
	unsigned int (*compress)(unsigned int*);
	unsigned int (*alignmentcalc)(unsigned int);
};

bool ispe();
int filecmp(FBuffer* pBuffer);
bool isx86();
bool isx64();
struct Validator {
	bool (*ispe)();
	bool (*isx86);
	bool (*isx64);
	int (*filecmp)(FBuffer*);
};

// flags: overwrite(0); return: -1 (file exit)
int fexport(char* filename, unsigned int flags);
unsigned int tofoa(unsigned int rva_addr);
unsigned int torva(unsigned int foa_addr);
unsigned int dllInjection(char* dll_name, char* function_name);
struct PETool {
	char* pFilename;
	int (*fexport)(char*, unsigned int);
	unsigned int (*tofoa)(unsigned int);
	unsigned int (*torva)(unsigned int);
	unsigned int (*dllInjection)(char*, char*);
	int (*pefree)(PETool*);
	Header* pHeader;
	Validator validator;
	FileObj file;
	ImageObj image;
};
int PETool_free(PETool* pPetool);

// Constructor
PETool* PETool_new(char* filename);
Header* Header_new();
Validator Validator_new();
FileObj File_new();
ImageObj Image_new();
