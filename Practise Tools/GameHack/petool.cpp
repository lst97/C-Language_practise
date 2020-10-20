#include "petool.h"

PETool* g_pPETool;

//
// PE Tool Function
//
int PETool_free(PETool* pPetool) {
	free(g_pPETool->pHeader->pBuffer);
	free(g_pPETool->pHeader->pOptheader);
	free(g_pPETool->pHeader);

	free(g_pPETool->file.pBuffer);

	free(g_pPETool->image.pBuffer);

	free(pPetool);
	return 0;
}

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

	if (g_pPETool->file.pBuffer != (FBuffer*)UNINIT_HEAP) {
		free(g_pPETool->file.pBuffer);
	}

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

// this function hidden from struct
int shiftnew() {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pBuffer = g_pPETool->file.pBuffer;
	*(unsigned int*)(pBuffer + IFANEW_OFFSET) = DOSH_SIZE + STUBNECESSARY_SIZE;

	memcpy(pBuffer + DOSH_SIZE + STUBNECESSARY_SIZE, pBuffer + pHeader->e_lfanew, pHeader->SizeOfHeaders - pHeader->e_lfanew);
	unsigned int end_offset = pHeader->sectionTables_offset + SECTION_SIZE * pHeader->NumberOfSection;
	memset(pBuffer + end_offset - (pHeader->e_lfanew - DOSH_SIZE - STUBNECESSARY_SIZE), 0, pHeader->e_lfanew - DOSH_SIZE - STUBNECESSARY_SIZE);

	if (!g_pPETool->pHeader->refresh()) {
		return -1;
	}

	return 0;
}

// return 0: success; -1 faile
int newsection(const char* section_name, char* bcode, unsigned int bcode_size, unsigned int characteristics) {
	// Calculate if have enough size.
	Header* pHeader = g_pPETool->pHeader;

	unsigned int total = 0;
	unsigned int headend_offset = pHeader->sectionTables_offset + SECTION_SIZE * pHeader->NumberOfSection;
	for (int fecx = 0; fecx < 0x50; fecx++) {
		total += *(pHeader->pBuffer + headend_offset + fecx);
	}
	if (total != 0) {
		// Method 2
		unsigned int dosstub_size = g_pPETool->pHeader->e_lfanew - DOSH_SIZE;
		if (dosstub_size > 0x50) {
			shiftnew();
		} else {
			return -1;
		}
	}

	// assign value for the new section table.
	SECTION_HEADER sheader = { 0 };
	int name_size = strlen(section_name);
	if (name_size <= 8) {
		memcpy(&sheader.Name, section_name, name_size);
	}
	memcpy(&sheader.Name, section_name, 0x8);
	
	// new section algo
	HBuffer* pHBuffer = pHeader->pBuffer;
	*(short*)(pHBuffer + pHeader->e_lfanew + SECTIONNUM_OFFSET) += 1;

	sheader.PointerToRawData = pHeader->file_size;
	sheader.VirtualAddress = pHeader->SizeOfImage;
	sheader.SizeOfRawData = g_pPETool->file.alignmentcalc(bcode_size);
	sheader.Misc.VirtualSize = sheader.SizeOfRawData;
	sheader.Characteristics = characteristics;

	*(unsigned int*)(pHBuffer + pHeader->e_lfanew + OPTH_OFFSET + IMAGESIZE_OFFSET) += g_pPETool->image.alignmentcalc(bcode_size);

	FBuffer* pFBuffer = g_pPETool->file.pBuffer;
	FBuffer* newpFBuffer = (FBuffer*)calloc(sheader.PointerToRawData + g_pPETool->file.alignmentcalc(bcode_size) , 1);
	memcpy(newpFBuffer + sheader.PointerToRawData, bcode, bcode_size); // append injected code
	memcpy(newpFBuffer, pFBuffer, pHeader->file_size); // original to new
	memcpy(newpFBuffer, pHBuffer, pHeader->SizeOfHeaders); // 
	memcpy(newpFBuffer + pHeader->sectionTables_offset + SECTION_SIZE * pHeader->NumberOfSection, &sheader, SECTION_SIZE);

	free(g_pPETool->file.pBuffer);
	g_pPETool->file.pBuffer = newpFBuffer;
	if (!g_pPETool->pHeader->refresh()) {
		return -1;
	}
	IBuffer* pIBuffer = g_pPETool->file.expand();
	if (pIBuffer == NULL) {
		return -1;
	}
	g_pPETool->image.pBuffer = pIBuffer;

	return 0;
}

int inject(const char* section_name, char* bcode) {

	return 0;
}

int fexport(FBuffer* pBuffer, char* filename, unsigned int size, unsigned int flags) {
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

unsigned int falignmentcalc(unsigned int size) {
	return g_pPETool->pHeader->pOptheader->FileAlignment * (size / g_pPETool->pHeader->pOptheader->FileAlignment + 1);
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

unsigned int ialignmentcalc(unsigned int size) {
	return g_pPETool->pHeader->pOptheader->SectionAlignment * (size / g_pPETool->pHeader->pOptheader->SectionAlignment + 1);
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
	file.alignmentcalc = falignmentcalc;

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
	image.alignmentcalc = ialignmentcalc;

	return image;
}

PETool* PETool_new(char* filename) {
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
	g_pPETool->pefree = PETool_free;

	return g_pPETool;
}
