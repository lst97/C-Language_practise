#include "petool.h"

PETool* g_pPETool;

//
// PE Tool Function
//

// This is tmp algo, the address may not be inorder. 23/10/2020 lst97
unsigned int tofoa(unsigned int rva_addr) {
	Header* pHeader = g_pPETool->pHeader;
	HBuffer* pHBuffer = pHeader->pBuffer;

	// covert logic
	if (rva_addr > pHeader->SizeOfImage) {
		return NULL;
	}
	if (rva_addr <= pHeader->SizeOfHeaders) {
		return rva_addr;
	}

	unsigned int* sections_vaddr = (unsigned int*)malloc(sizeof(int) * pHeader->NumberOfSection);
	unsigned int* sections_faddr = (unsigned int*)malloc(sizeof(int) * pHeader->NumberOfSection);

	// travel section
	for (unsigned int fecx = 0; fecx < pHeader->NumberOfSection; fecx++) {
		*(sections_vaddr + fecx) = *(unsigned int*)(pHBuffer + pHeader->sectionTables_offset + SECTION_SIZE * fecx + VIRTUALADDR_OFFSET);
		*(sections_faddr + fecx) = *(unsigned int*)(pHBuffer + pHeader->sectionTables_offset + SECTION_SIZE * fecx + RAWPTR_OFFSET);
	}

	unsigned int section_id = 0;
	unsigned int foa_offset;
	for (unsigned int fecx = pHeader->NumberOfSection; fecx > 0; --fecx) {
		if (rva_addr >= *(sections_vaddr + fecx)) {
			section_id = fecx;
			foa_offset = rva_addr - *(sections_vaddr + fecx);
			break;
		}
	}

	unsigned int rvalue = *(sections_faddr + section_id) + foa_offset;
	//free(sections_vaddr);
	//free(sections_faddr);

	return rvalue;
}

unsigned int torva(unsigned int foa_addr) {
	Header* pHeader = g_pPETool->pHeader;
	HBuffer* pHBuffer = pHeader->pBuffer;
	unsigned int* sections_vaddr = (unsigned int*)malloc(sizeof(int) * pHeader->NumberOfSection);
	unsigned int* sections_faddr = (unsigned int*)malloc(sizeof(int) * pHeader->NumberOfSection);

	// travel section
	for (unsigned int fecx = 0; fecx < pHeader->NumberOfSection; fecx++) {
		*(sections_vaddr + fecx) = *(unsigned int*)(pHBuffer + pHeader->sectionTables_offset + SECTION_SIZE * fecx + VIRTUALADDR_OFFSET);
		*(sections_faddr + fecx) = *(unsigned int*)(pHBuffer + pHeader->sectionTables_offset + SECTION_SIZE * fecx + RAWPTR_OFFSET);
	}

	// covert logic
	if (foa_addr > pHeader->file_size) {
		return NULL;
	}
	unsigned int section_id = 0;
	unsigned int rva_offset;
	for (unsigned int fecx = pHeader->NumberOfSection; fecx > 0; --fecx) {
		if (foa_addr >= *(sections_faddr + fecx)) {
			section_id = fecx;
			rva_offset = foa_addr - *(sections_faddr + fecx);
			break;
		}
	}

	return *(sections_vaddr + section_id) + rva_offset;
}

int PETool_free(PETool* pPetool) {
	free(pPetool->pHeader->pBuffer);
	free(pPetool->pHeader->pOptheader);
	free(pPetool->pHeader);

	free(pPetool->file.pBuffer);

	free(pPetool->image.pBuffer);

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
	pOptheader = (OPT_HEADER*)malloc(OPTHEADER_SIZE + DATADIRARR_SIZE);
	if (pOptheader == NULL) {
		return NULL;
	}
	memcpy(pOptheader, pBuffer + optheader_offset, OPTHEADER_SIZE + DATADIRARR_SIZE);
	g_pPETool->pHeader->pOptheader = pOptheader;

	return pHeader->SizeOfHeaders;
}

EXPORT_FUNCTION* getExportFunctions() {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pFBuffer = g_pPETool->file.pBuffer;
	EXPORT_TABLE export_table;

	unsigned int exporttable_offset = g_pPETool->tofoa(pHeader->pOptheader->DataDirArray.Export.VirtualAddress);
	if (exporttable_offset == 0) {
		return NULL;
	}

	memcpy(&export_table, pFBuffer + exporttable_offset, EXPORTTABLE_SIZE);
	unsigned int foaOfFunctions = g_pPETool->tofoa(export_table.AddressOfFunctions);
	unsigned int foaOfNames = g_pPETool->tofoa(export_table.AddressOfNames);
	unsigned int foaOfNameOrdinals = g_pPETool->tofoa(export_table.AddressOfNameOrdinals);

	unsigned int struct_count;
	if (export_table.NumberOfName >= export_table.NumberOfFunctions) {
		struct_count = export_table.NumberOfName;
	} else {
		struct_count = export_table.NumberOfFunctions;
	}
	EXPORT_FUNCTION* pFunction_info = (EXPORT_FUNCTION*)calloc((struct_count + 1), sizeof(EXPORT_FUNCTION));

	unsigned short funame_offset = 0;
	for (unsigned short fecx = 0; fecx < export_table.NumberOfFunctions; fecx++) {
		(pFunction_info + fecx)->function_addr = *(unsigned int*)(pFBuffer + foaOfFunctions + fecx * 0x04);
		(pFunction_info + fecx)->ordinal = fecx;
		if ((pFunction_info + fecx)->function_addr != 0) {
			// travel ordinals table
			for (unsigned int fedx = 0; fedx < export_table.NumberOfName; fedx++) {
				funame_offset = *(unsigned short*)(pFBuffer + foaOfNameOrdinals + fecx * 0x02);
				if (funame_offset == fecx) {
					// get function name ptr
					(pFunction_info + fecx)->pName = (char*)((unsigned int)pFBuffer) + g_pPETool->tofoa(*(unsigned int*)(pFBuffer + foaOfNames + funame_offset * 0x04));
					break;
				}
			}
			if ((pFunction_info + fecx)->pName == (char*)UNINIT_HEAP) {
				// function has no name
				(pFunction_info + fecx)->pName = 0;
			}
		}
	}
	return pFunction_info;
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

int expandsection(unsigned int size) {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pFBuffer = g_pPETool->file.pBuffer;

	unsigned int laststable_offset = pHeader->sectionTables_offset + SECTION_SIZE * (pHeader->NumberOfSection - 1);
	unsigned int expend_size = g_pPETool->file.alignmentcalc(size - 1);

	SECTION_HEADER sheader;
	memcpy(&sheader, pFBuffer + laststable_offset, SECTION_SIZE);

	sheader.SizeOfRawData += expend_size;
	sheader.Misc.VirtualSize += expend_size;
	unsigned int virtualsize_total = pHeader->SizeOfImage - sheader.VirtualAddress;
	memcpy(pFBuffer + laststable_offset, &sheader, SECTION_SIZE);

	unsigned int remainder = sheader.Misc.VirtualSize / virtualsize_total;
	unsigned int newImageSize = pHeader->SizeOfImage;
	if (remainder > 0) {
		newImageSize += pHeader->pOptheader->SectionAlignment * remainder;
	}
	*(unsigned int*)(pFBuffer + pHeader->e_lfanew + OPTH_OFFSET + IMAGESIZE_OFFSET) = newImageSize;

	if (pHeader->refresh() != NULL) {
		free(g_pPETool->image.pBuffer);
		g_pPETool->image.pBuffer = g_pPETool->file.expand();

		return (int)expend_size;
	}

	return NULL;
}

//// Lack of idea 21/10/2020
//int mergesection() {
//	Header* pHeader = g_pPETool->pHeader;
//	FBuffer* pFBuffer = g_pPETool->file.pBuffer;
//
//	unsigned int totalsection_size = pHeader->SizeOfImage - (unsigned int)*(int*)(pFBuffer + pHeader->sectionTables_offset + VIRTUALADDR_OFFSET);
//	
//	*(pFBuffer + pHeader->e_lfanew + SECTIONNUM_OFFSET) = 1;
//	*(pFBuffer + pHeader->sectionTables_offset + MISC_OFFSET) = totalsection_size;
//	*(pFBuffer + pHeader->sectionTables_offset + RAWSIZE_OFFSET) = totalsection_size;
//
//	return 0;
//}

//
// Image function declare
//
IBuffer* icreate() {
	Header* pHeader = g_pPETool->pHeader;
	FBuffer* pBuffer = g_pPETool->file.pBuffer;
	FBuffer* pSectionTable = pBuffer + pHeader->sectionTables_offset;

	IBuffer* pImageBuffer = (IBuffer*)calloc(pHeader->SizeOfImage, 1);
	if (pImageBuffer == NULL) {
		return pImageBuffer;
	}
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
	pHeader->getExportFunctions = getExportFunctions;

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
	file.expandsection = expandsection;

	return file;
}

ImageObj Image_new() {
	ImageObj image;
	image.pHeader = g_pPETool->pHeader;
	image.create = icreate;
	image.pBuffer = image.create();
	image.write = iwrite;
	image.compress = icompress;
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
	g_pPETool->tofoa = tofoa;
	g_pPETool->torva = torva;
	g_pPETool->fexport = fexport;
	g_pPETool->pefree = PETool_free;

	return g_pPETool;
}
