#include "petool.h"

PETool* g_pPETool;

//
// Validate Function Declare
//
bool ispe() {
	FILE* fp;
	unsigned int ntHeader_offset;
	unsigned int headsize;
	unsigned char* buffer;

	fp = fopen(g_pPETool->pFilename, "rb");
	if (!fp) {
		return false;
	}
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

int filecmp(FBuffer* pBuffer) {
	return memcmp(pBuffer, g_pPETool->file.pBuffer, g_pPETool->pHeader->file_size);
}

//
// Header Function Deaclare
//
int hrefresh() {
	Header* pHeader = g_pPETool->pHeader;
	HBuffer* pBuffer = pHeader->pBuffer;
	OPT_HEADER* pOptheader = pHeader->pOptheader;

	unsigned int SizeOfRawData;
	unsigned int PointerToRawData;
	unsigned int optheader_offset;
	bool isRecreation = false;

	FILE* fp;
	fp = fopen(g_pPETool->pFilename, "rb");

	if (pBuffer != (HBuffer*)UNINIT_HEAP) {
		free(pBuffer);
		isRecreation = true;
	}

	pBuffer = (HBuffer*)malloc(0x400);
	if (pBuffer == NULL) {
		return NULL;
	}

	if (isRecreation == false) {
		fread(pBuffer, 0x400, 1, fp);
	} else {
		memcpy(pBuffer, g_pPETool->file.pBuffer, 0x400);
	}

	pHeader->e_lfanew = (unsigned int)*(int*)(pBuffer + 0x3C);
	pHeader->SizeOfHeaders = (unsigned int)*(int*)(pBuffer + pHeader->e_lfanew + OPTH_OFFSET + IFANEW_OFFSET);
	if (pHeader->SizeOfHeaders != 0x400) {
		free(pBuffer);
		pBuffer = (HBuffer*)malloc(pHeader->SizeOfHeaders);
		if (pBuffer == NULL) {
			return NULL;
		}
		if (isRecreation = false) {
			rewind(fp);
			fread(pBuffer, pHeader->SizeOfHeaders, 1, fp);
		} else {
			memcpy(pBuffer, g_pPETool->file.pBuffer, pHeader->SizeOfHeaders);
		}
	}
	fclose(fp);

	pHeader->SizeOfOptionalHeader = (unsigned short)*(short*)(pBuffer + pHeader->e_lfanew + OPTHSIZE_OFFSET);
	pHeader->sectionTables_offset = pHeader->e_lfanew + OPTH_OFFSET + pHeader->SizeOfOptionalHeader;
	pHeader->NumberOfSection = (unsigned short)*(short*)(pBuffer + pHeader->e_lfanew + SECTIONNUM_OFFSET);

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
	if (pOptheader == NULL) {
		return NULL;
	}
	memcpy(pOptheader, pBuffer + optheader_offset, OPTHEADER_SIZE);
	g_pPETool->pHeader->pOptheader = pOptheader;

	return pHeader->SizeOfHeaders;
}

//
// File function declare
//
FBuffer* fcreate() {
	unsigned int file_size = g_pPETool->pHeader->file_size;

	FBuffer* pBuffer = (FBuffer*)malloc(file_size);
	if (pBuffer == NULL) {
		return pBuffer;
	}
	FILE* fp;
	fp = fopen(g_pPETool->pFilename, "rb");
	fread(pBuffer, file_size, 1, fp);

	fclose(fp);

	return pBuffer;
}

unsigned int FoaToRva(unsigned int foa_addr) {
	return 0;
}

int fwrite(unsigned int offset, unsigned int size) {
	return 0;
}

// return 0: success; -1 faile
int newsection(const char* section_name, char* bcode, unsigned int bcode_size, unsigned int characteristics) {
	// Calculate if have enough size.
	Header* pHeader = g_pPETool->pHeader;
	int remain_size = pHeader->SizeOfHeaders - pHeader->sectionTables_offset - SECTION_SIZE * pHeader->NumberOfSection;
	if (remain_size < 0x50) {
		return -1;
	}

	// assign value for the new section table.
	SECTION_HEADER sheader = { 0 };
	int name_size = strlen(section_name);
	if (name_size <= 8) {
		memcpy(&sheader.Name, section_name, name_size);
	}
	memcpy(&sheader.Name, section_name, 0x8);
	
	// new section algo
	FBuffer* pBuffer = g_pPETool->file.pBuffer;
	*(short*)(pBuffer + pHeader->e_lfanew + SECTIONNUM_OFFSET) += 1;

	sheader.PointerToRawData = pHeader->file_size;
	sheader.VirtualAddress = pHeader->SizeOfImage;
	unsigned int remainder = bcode_size / pHeader->pOptheader->FileAlignment + 1;
	sheader.SizeOfRawData = pHeader->pOptheader->FileAlignment * remainder;
	sheader.Misc.VirtualSize = sheader.SizeOfRawData;
	sheader.Characteristics = characteristics;

	remainder = sheader.Misc.VirtualSize / pHeader->pOptheader->SectionAlignment + 1;
	*(unsigned int*)(pBuffer + pHeader->e_lfanew + OPTH_OFFSET + IMAGESIZE_OFFSET) += pHeader->pOptheader->SectionAlignment * remainder;

	FBuffer* pBuffer_new = (FBuffer*)calloc(sheader.PointerToRawData + pHeader->pOptheader->FileAlignment * remainder, 1);
	memcpy(pBuffer_new + sheader.PointerToRawData, bcode, bcode_size);
	memcpy(pBuffer_new, pBuffer, pHeader->file_size);
	memcpy(pBuffer_new + pHeader->sectionTables_offset + SECTION_SIZE * pHeader->NumberOfSection, &sheader, SECTION_SIZE);

	free(pBuffer);
	g_pPETool->file.pBuffer = pBuffer_new;
	if (!g_pPETool->pHeader->refresh()) {
		return -1;
	}
	g_pPETool->image.pBuffer = g_pPETool->file.expand();
	if (g_pPETool->image.pBuffer == NULL) {
		return -1;
	}

	return 0;
}

int inject(const char* section_name, char* bcode) {

	return 0;
}

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

//
// Image function declare
//
IBuffer* icreate() {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pBuffer = g_pPETool->file.pBuffer;
	FBuffer* pSectionTable = pBuffer + pHeader->sectionTables_offset;

	if (g_pPETool->image.pBuffer != (IBuffer*)UNINIT_HEAP) {
		free(g_pPETool->image.pBuffer);
	}

	IBuffer* pImageBuffer = (IBuffer*)calloc(pHeader->SizeOfImage, 1);

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

unsigned int RvaToFoa(unsigned int rva_addr) {
	return 0;
}

//
// Struct declare
//
Validator Validator_new() {
	Validator validator;
	validator.ispe = ispe;
	validator.filecmp = filecmp;

	return validator;
}

Header* Header_new() {
	Header* pHeader = (Header*)malloc(sizeof(Header));
	if (pHeader == NULL) {
		return pHeader;
	}
	pHeader->refresh = hrefresh;
	return pHeader;
}

FileObj File_new() {
	FileObj file;

	file.pHeader = g_pPETool->pHeader;
	file.create = fcreate;
	file.pBuffer = file.create();
	file.expand = icreate;
	file.write = fwrite;
	file.newsection = newsection;
	file.inject = inject;

	return file;
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

PETool* PETool_new(const char* filename) {
	g_pPETool = (PETool*)malloc(sizeof(PETool));
	if (g_pPETool == NULL) {
		return g_pPETool;
	}
	g_pPETool->pFilename = filename;
	g_pPETool->validator = Validator_new();
	if (!g_pPETool->validator.ispe()) {
		return NULL;
	}
	g_pPETool->pHeader = Header_new();
	g_pPETool->pHeader->refresh();
	g_pPETool->file = File_new();
	g_pPETool->image = Image_new();
	g_pPETool->fexport = fexport;

	return g_pPETool;
}
