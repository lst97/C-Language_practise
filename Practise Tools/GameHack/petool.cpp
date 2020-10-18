#include "petool.h"

PETool* g_pPETool;

bool ispe() {
	FILE* fp;
	unsigned int ntHeader_offset;
	unsigned int headsize;
	unsigned char* buffer;

	fp = fopen(g_pPETool->pFilename, "rb");
	buffer = (unsigned char*)malloc(0x400);
	fread(buffer, 0x400, 1, fp);
	ntHeader_offset = (unsigned int)*(int*)(buffer + 0x3C);

	headsize = (unsigned int)*(int*)(buffer + ntHeader_offset + OPTH_OFFSET + IFANEW_OFFSET);
	if (headsize != 0x400) {
		free(buffer);
		buffer = (unsigned char*)malloc(headsize);
		rewind(fp);
		fread(buffer, headsize, 1, fp);
	}

	unsigned short mz = (unsigned short)*(short*)buffer;
	unsigned int signature = (unsigned int)*(int*)(buffer + ntHeader_offset);
	if (mz != 0x5A4D && signature != 0x4550) {
		return false;
	}

	free(buffer);
	fclose(fp);

	return true;
}

int hrefresh() {
	Header* pHeader = g_pPETool->pHeader;
	HBuffer* pBuffer = pHeader->pBuffer;
	OPT_HEADER* pOptheader = pHeader->pOptheader;

	unsigned int SizeOfRawData;
	unsigned int PointerToRawData;
	unsigned int optheader_offset;

	FILE* fp;
	fp = fopen(g_pPETool->pFilename, "rb");

	if (pBuffer != (HBuffer*)UNINIT_HEAP) {
		free(pBuffer);
	}
	
	pBuffer = (HBuffer*)malloc(0x400);
	fread(pBuffer, 0x400, 1, fp);

	pHeader->e_lfanew = (unsigned int)*(int*)(pBuffer + 0x3C);
	pHeader->SizeOfHeaders = (unsigned int)*(int*)(pBuffer + pHeader->e_lfanew + OPTH_OFFSET + IFANEW_OFFSET);
	if (pHeader->SizeOfHeaders != 0x400) {
		free(pBuffer);
		pBuffer = (HBuffer*)malloc(pHeader->SizeOfHeaders);
		rewind(fp);
		fread(pBuffer, pHeader->SizeOfHeaders, 1, fp);
	}
	fclose(fp);

	pHeader->SizeOfOptionalHeader = (unsigned short)*(short*)(pBuffer + pHeader->e_lfanew + OPTHSIZE_OFFSET);
	pHeader->sectionTables_offset = pHeader->e_lfanew + OPTH_OFFSET + pHeader->SizeOfOptionalHeader;
	pHeader->NumberOfSection = (unsigned short)*(short*)(pBuffer + pHeader->e_lfanew + SECTIONSIZE_OFFSET);

	PointerToRawData = (unsigned int)*(int*)(pBuffer + pHeader->sectionTables_offset + (SECTION_SIZE * (pHeader->NumberOfSection - 1)) + RAWPTR_OFFSET);
	SizeOfRawData = (unsigned int)*(int*)(pBuffer + pHeader->sectionTables_offset + (SECTION_SIZE * (pHeader->NumberOfSection - 1)) + RAWSIZE_OFFSET);
	pHeader->file_size = PointerToRawData + SizeOfRawData;
	pHeader->SizeOfImage = (unsigned int)*(int*)(pBuffer + pHeader->e_lfanew + OPTH_OFFSET + IMAGESIZE_OFFSET);

	pHeader->pBuffer = pBuffer;

	optheader_offset = pHeader->e_lfanew + OPTH_OFFSET;
	if (pOptheader != (OPT_HEADER*)UNINIT_HEAP) {
		free(pOptheader);
	}
	pOptheader = (OPT_HEADER*)malloc(OPTHEADER_SIZE);
	memcpy(pOptheader, pBuffer + optheader_offset, OPTHEADER_SIZE);
	g_pPETool->pHeader->pOptheader = pOptheader;

	return pHeader->SizeOfHeaders;
}

Header* Header_new() {
	Header* pHeader = (Header*)malloc(sizeof(Header));
	pHeader->refresh = hrefresh;
	return pHeader;
}

int filecmp(FBuffer* pBuffer) {
	return memcmp(pBuffer, g_pPETool->file.pBuffer, g_pPETool->pHeader->file_size);
}
Validator Validator_new() {
	Validator validator;
	validator.ispe = ispe;
	validator.filecmp = filecmp;

	return validator;
}

FBuffer* fcreate() {
	unsigned int file_size = g_pPETool->pHeader->file_size;

	FBuffer* pBuffer = (FBuffer*)malloc(file_size);

	FILE* fp;
	fp = fopen(g_pPETool->pFilename, "rb");
	fread(pBuffer, file_size, 1, fp);

	fclose(fp);

	return pBuffer;
}

int fwrite(unsigned int offset, unsigned int size) {
	return 0;
}

IBuffer* icreate() {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pBuffer = g_pPETool->file.pBuffer;
	FBuffer* pSectionTable = pBuffer + pHeader->sectionTables_offset;

	if (g_pPETool->image.pBuffer != (IBuffer*)UNINIT_HEAP) {
		free(g_pPETool->image.pBuffer);
	}
	IBuffer* pImageBuffer = (IBuffer*)calloc(pHeader->SizeOfImage, sizeof(IBuffer));

	memcpy(pImageBuffer, pBuffer, pHeader->SizeOfHeaders);

	unsigned int virtualAddr, sizeOfRawData, pointerToRawData;
	for (unsigned int fecx = 0; fecx < pHeader->NumberOfSection; fecx++) {
		sizeOfRawData = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + RAWSIZE_OFFSET);
		pointerToRawData = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + RAWPTR_OFFSET);
		virtualAddr = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + VIRTUALADDR_OFFSET);

		if (sizeOfRawData) {
			memcpy(pImageBuffer + virtualAddr, pBuffer + pointerToRawData, sizeOfRawData);
		}
	}

	return pImageBuffer;
}

unsigned int FoaToRva(unsigned int foa_addr, unsigned int rva_addr) {
	return 0;
}

FileObj File_new() {
	FileObj file;

	file.pHeader = g_pPETool->pHeader;
	file.create = fcreate;
	file.pBuffer = file.create();
	file.expand = icreate;
	file.write = fwrite;

	return file;
}

int iwrite(unsigned int addr, unsigned int size) {
	return 0;
}

unsigned int icompress(unsigned int* ptr) {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pImageBuffer = g_pPETool->image.pBuffer;
	FBuffer* pSectionTable = pImageBuffer + pHeader->sectionTables_offset;

	FBuffer* pFileBuffer = (FBuffer*)calloc(pHeader->file_size, sizeof(FBuffer));

	memcpy(pFileBuffer, pImageBuffer, pHeader->SizeOfHeaders);

	unsigned int virtualAddr, sizeOfRawData, pointerToRawData;
	for (unsigned int fecx = 0; fecx < pHeader->NumberOfSection; fecx++) {
		sizeOfRawData = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + RAWSIZE_OFFSET);
		pointerToRawData = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + RAWPTR_OFFSET);
		virtualAddr = *(unsigned int*)(pSectionTable + (SECTION_SIZE * fecx) + VIRTUALADDR_OFFSET);

		if (sizeOfRawData) {
			memcpy(pFileBuffer + pointerToRawData, pImageBuffer + virtualAddr, sizeOfRawData);
		}
	}

	*ptr = (unsigned int)pFileBuffer;

	return pHeader->file_size;
}

unsigned int RvaToFoa(unsigned int rva_addr, unsigned int foa_addr) {
	return 0;
}

ImageObj Image_new() {
	ImageObj image;
	image.pHeader = g_pPETool->pHeader;
	image.create = icreate;
	image.pBuffer = image.create();
	image.write = iwrite;
	image.compress = icompress;
	image.rva_foa = RvaToFoa;

	return image;
}

// flags: overwrite(0)
// return: -1 (file exit) 
int fexport(FBuffer* pBuffer, const char* filename, unsigned int size, unsigned int flags) {
	FILE* fp;

	if (flags) {
		fp = fopen(filename, "wb");
		fwrite(pBuffer, size, 1, fp);
		fclose(fp);

		return 0;
	}

	fp = fopen(filename, "r");
	if (fp != NULL) {
		fclose(fp);
		return -1;
	}
	fp = fopen(filename, "wb");
	fwrite(pBuffer, size, 1, fp);
	fclose(fp);

	return 0;
}
PETool* PETool_new(const char* filename) {
	g_pPETool = (PETool*)malloc(sizeof(PETool));
	g_pPETool->pFilename = filename;
	g_pPETool->validator = Validator_new();
	g_pPETool->pHeader = Header_new();
	g_pPETool->pHeader->refresh();
	g_pPETool->file = File_new();
	g_pPETool->image = Image_new();
	g_pPETool->fexport = fexport;

	return g_pPETool;
}