#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "String.h"

#ifndef _WINDOWS_
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#endif // !_WINDOWS_

#define SECTION_SIZE 0x28
#define OPTHEADER_SIZE 0x60

// PE Header Offset
#define SECTIONSIZE_OFFSET 0x06
#define OPTHSIZE_OFFSET 0x14
#define OPTH_OFFSET 0x18
#define IFANEW_OFFSET 0x3C
#define IMAGESIZE_OFFSET 0x38

// Section Header Offset
#define RAWSIZE_OFFSET 0x10
#define RAWPTR_OFFSET 0x14
#define VIRTUALADDR_OFFSET 0x0C

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

// Program struct
struct Header {
	HBuffer* pBuffer;
	OPT_HEADER* pOptheader;
	unsigned int e_lfanew;
	unsigned short SizeOfOptionalHeader;
	unsigned short NumberOfSection;
	unsigned int SizeOfHeaders;
	unsigned int SizeOfImage;

	unsigned int sectionTables_offset;
	unsigned int file_size;

	int (*refresh)();
};

FBuffer* fcreate();
int fwrite(unsigned int offset, unsigned int size);
unsigned int FoaToRva(unsigned int foa_addr, unsigned int rva_addr);
struct FileObj {
	Header* pHeader;
	FBuffer* pBuffer;
	FBuffer* (*create)();
	unsigned int (*foa_rva)(unsigned int, unsigned int);
	int (*write)(unsigned int, unsigned int);
	IBuffer* (*expand)();
};

int iwrite(unsigned int offset, unsigned int size);
unsigned int icompress(unsigned int* ptr);
struct ImageObj {
	Header* pHeader;
	IBuffer* pBuffer;
	unsigned int (*rva_foa)(unsigned int, unsigned int);
	IBuffer* (*create)();
	int (*write)(unsigned int, unsigned int);
	unsigned int (*compress)(unsigned int*);
};

bool ispe();
int filecmp(FBuffer* pBuffer);
bool isx86();
bool isx64();
struct Validator {
	bool (*ispe)();
	int (*filecmp)(FBuffer*);
	bool (*isx86);
	bool (*isx64);
};

int fexport(FBuffer* pBuffer, const char* filename, unsigned int size, unsigned int flags);
struct PETool {
	const char* pFilename;
	Header* pHeader;
	FileObj file;
	ImageObj image;
	Validator validator;
	int (*fexport)(FBuffer*, const char*, unsigned int, unsigned int);

};

PETool* PETool_new(const char* filename);
Validator Validator_new();
FileObj File_new();
ImageObj Image_new();
Header* Header_new();