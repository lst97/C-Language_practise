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

#define SECTION_SIZE 0x28
#define OPTHEADER_SIZE 0x60

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

// Uninit
#define UNINIT_STACK 0xCCCCCCCC
#define UNINIT_HEAP 0xCDCDCDCD

typedef unsigned char Buffer;
typedef Buffer FBuffer;
typedef Buffer IBuffer;
typedef Buffer HBuffer;

// PE struct
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

// Program struct
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
};

FBuffer* fcreate();
int fwrite(unsigned int offset, unsigned int size);
unsigned int FoaToRva(unsigned int foa_addr);
int newsection(const char* section_name, char* bcode, unsigned int bcode_size, unsigned int characteristics);
int inject(const char* section_name, char* bcode);
struct FileObj {
	Header* pHeader;
	FBuffer* pBuffer;
	FBuffer* (*create)();
	IBuffer* (*expand)();
	int (*newsection)(const char*, char*, unsigned int, unsigned int);
	int (*inject)(const char*, char*);
	int (*write)(unsigned int, unsigned int);
	unsigned int (*foa_rva)(unsigned int);
};

int iwrite(unsigned int offset, unsigned int size);
unsigned int icompress(unsigned int* ptr);
unsigned int RvaToFoa(unsigned int rva_addr);
struct ImageObj {
	Header* pHeader;
	IBuffer* pBuffer;
	IBuffer* (*create)();
	int (*write)(unsigned int, unsigned int);
	unsigned int (*rva_foa)(unsigned int);
	unsigned int (*compress)(unsigned int*);
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
int fexport(FBuffer* pBuffer, const char* filename, unsigned int size, unsigned int flags);
struct PETool {
	const char* pFilename;
	int (*fexport)(FBuffer*, const char*, unsigned int, unsigned int);
	Header* pHeader;
	Validator validator;
	FileObj file;
	ImageObj image;

};

PETool* PETool_new(const char* filename);
Header* Header_new();
Validator Validator_new();
FileObj File_new();
ImageObj Image_new();
