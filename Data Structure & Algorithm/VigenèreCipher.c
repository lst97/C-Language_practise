/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : VigenèreCipher.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 06/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Vigenère Cipher
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 06062019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// [A-Z] 0x41 - 0x5A
struct TwoWayCircularLinkedList {
	struct TwoWayCircularLinkedList* pBefore;
	char cChar;
	struct TwoWayCircularLinkedList* pNext;
};
typedef struct TwoWayCircularLinkedList TWCLL;

TWCLL* fnReferenceTable(void);
void fnVigenèreCipher(void);

int main(void) {
	
	fnVigenèreCipher();

	system("pause");
	return 0;
}

TWCLL* fnReferenceTable(void) {
	TWCLL* pTWCLL = 0, *pTempTWCLL = 0, *pTWCLL_Head = 0;;
	int iUserInput = 0, iCounter = 0;
	for (int iLoop = 0; iLoop < 26; iLoop++) {
		if (iLoop == 0) {
			pTWCLL = (TWCLL*)malloc(sizeof(TWCLL));
			pTWCLL->cChar = 0x41 + iLoop;
			pTWCLL->pBefore = pTWCLL;
			pTWCLL->pNext = pTWCLL;
			pTWCLL_Head = pTWCLL;
			pTempTWCLL = pTWCLL;
			continue;
		}
		pTWCLL = (TWCLL*)malloc(sizeof(TWCLL));
		pTWCLL->cChar = 0x41 + iLoop;
		pTempTWCLL->pNext = pTWCLL;
		pTWCLL->pBefore = pTempTWCLL;
		pTWCLL->pNext = pTWCLL_Head;
		pTWCLL_Head->pBefore = pTWCLL;
		pTempTWCLL = pTWCLL;
	}

	return pTWCLL_Head;
}

void fnVigenèreCipher(void) {
	TWCLL* pTWCLL_RefTable_PlaintText = 0,*pTWCLL_RefTable_Key = 0, *pTWCLL_RefTable_Head = 0;
	char szPlainText[32], szKey[32], cTemp = 0;
	int iCounter = 0, iLoop_PlainText = 0, iLoop_KeyOffset = 0;

	pTWCLL_RefTable_Head = pTWCLL_RefTable_Key = pTWCLL_RefTable_PlaintText = fnReferenceTable();

	printf("Please input plain text [A-Z]: (NO SPACEBAR ALLOWED)\n");
	scanf_s("%s", szPlainText, 31);
	
	printf("Please input the Key [A-Z]: (NO SPACEBAR ALLOWED)\n");
	scanf_s("%s", szKey, 31);

	printf("\n");

	if (strlen(szKey) == strlen(szPlainText)) {
		while (szPlainText[iLoop_PlainText] != '\0') {
			// find the place in the reftable;
			if (toupper(szPlainText[iLoop_PlainText]) != pTWCLL_RefTable_PlaintText->cChar) {
				pTWCLL_RefTable_PlaintText = pTWCLL_RefTable_PlaintText->pNext;
				continue;
			} else {
				// Encrypt with the key (get the offset)
				while (1){
					if (toupper(szKey[iLoop_PlainText]) != pTWCLL_RefTable_Key->cChar) {
						pTWCLL_RefTable_Key = pTWCLL_RefTable_Key->pNext;
						iLoop_KeyOffset++;
					} else break;
				}
				// Move the plain text according to the offset. 
				// https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher
				while (iLoop_KeyOffset != 0){
					pTWCLL_RefTable_PlaintText = pTWCLL_RefTable_PlaintText->pNext;
					iLoop_KeyOffset--;
				}
				printf("%c", pTWCLL_RefTable_PlaintText->cChar);
				iLoop_KeyOffset = 0;
				pTWCLL_RefTable_Key = pTWCLL_RefTable_PlaintText = pTWCLL_RefTable_Head;
			}
			iLoop_PlainText++;
		}
		printf("\n\n");
	}
	else printf("Invalid Key!\n\n");
}
