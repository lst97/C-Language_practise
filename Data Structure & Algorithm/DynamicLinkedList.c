/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : DynamicLinkedList.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 04/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Dynamic Linked List and 2 Ways of middle search
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 04062019    lst97       1      First release
;*
;* Known Issue       :
;* No input validate and exception handle
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>

#define LENGTH 10

typedef struct DynamicLinkedList DLL;
struct DynamicLinkedList {
	char cData;
	struct DynamicLinkedList* pNext;
};

DLL* fnCreateDLL_TailInsertion(void);
DLL* fnCreateDLL_HeadInsertion(void);

void fnPrintDLL(DLL *);

void fnPrintMiddle_Quick(DLL *);
void fnPrintMiddle_Slow(DLL *);

void fnFreeDLL(DLL *);

int main(void) {
	DLL* pTailInsertion = 0, * pHeadInsertion = 0;
	int iUserInput = -1;

	printf("= = = = = Menu = = = = =\n1. Create Linked List\n2. View Linked List\n3. Value of the middle\n0. Exit\n\n");
	while (1) {
		scanf_s("%d", &iUserInput);
		switch (iUserInput) {
		case 0:
			fnFreeDLL(pTailInsertion);
			fnFreeDLL(pHeadInsertion);
			break;
		case 1:;
			pTailInsertion = fnCreateDLL_TailInsertion();
			pHeadInsertion = fnCreateDLL_HeadInsertion();
			continue;
		case 2:
			fnPrintDLL(pTailInsertion);
			fnPrintDLL(pHeadInsertion);
			continue;
		case 3:
			fnPrintMiddle_Quick(pTailInsertion);
			fnPrintMiddle_Slow(pHeadInsertion);
			continue;
		default:
			printf("Invalid input!\n\n");
			continue;
		}
		break;
	}
	system("pause");
	return 0;
}

DLL* fnCreateDLL_TailInsertion(void) {
	DLL *pDLL_Head, *pDLL = (DLL*)malloc(sizeof(DLL)), *pTempDLL = 0;

	pDLL_Head = pDLL;
	pDLL->cData = 0 + 0x30;
	pDLL->pNext = 0;
	for (int iLoop = 1; iLoop < LENGTH; iLoop++) {
		pTempDLL = (DLL*)malloc(sizeof(DLL));
		pTempDLL->cData = iLoop + 0x30;
		pTempDLL->pNext = 0;
		pDLL->pNext = pTempDLL;
		pDLL = pTempDLL;
	}
	printf("Creat Linked List (Tail Inserstion) - SUCCESS!\n");
	return pDLL_Head;
}

DLL* fnCreateDLL_HeadInsertion(void) {
	DLL*pDLL = 0, *pTempDLL = 0;

	for (int iLoop = 0; iLoop < LENGTH; iLoop++) {
		pTempDLL = (DLL*)malloc(sizeof(DLL));
		pTempDLL->cData = iLoop + 0x30;
		pTempDLL->pNext = pDLL;
		pDLL = pTempDLL;
	}

	printf("Creat Linked List (Head Inserstion) - SUCCESS!\n");
	return pDLL;
}

void fnPrintDLL(DLL *pDLL) {
	while (pDLL->pNext != 0) {
		printf("%c", pDLL->cData);
		pDLL = pDLL->pNext;
	}
	printf("%c\n", pDLL->cData);
}

// O(L/2)
void fnPrintMiddle_Quick(DLL* pDLL) {
	DLL* pQuick = pDLL, *pSlow = pDLL;

	while (pQuick != 0) {
		if (pQuick->pNext != 0 && pQuick->pNext > 0) {
			pQuick = pQuick->pNext->pNext;
			pSlow = pSlow->pNext;
		} else break;
	}
	printf("%c\n", pSlow->cData);
}

// O(3L/2)
void fnPrintMiddle_Slow(DLL* pDLL) {
	int iCounter = 0, iLoop = 0;
	DLL* pTemp = 0, *pStart = pDLL;

	while (pDLL->pNext != 0) {
		pTemp = pDLL->pNext;
		pDLL = pTemp;
		iCounter++;
	}
	iCounter++;

	pDLL = pStart;
	while (iLoop != iCounter / 2) {
		pTemp = pDLL->pNext;
		pDLL = pTemp;
		iLoop++;
	}
	printf("%c\n", pDLL->cData);
}

void fnFreeDLL(DLL* pDLL){
	DLL* pTemp = 0;

	while (pDLL->pNext != 0) {
		pTemp = pDLL->pNext;
		free(pDLL);
		pDLL = pTemp;
	}
	free(pDLL);
	printf("Free SUCCESS!\n");
}
