/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : TwoWayCircularLinkedList.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 05/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Two Way Circular Linked List
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 05062019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>

// [A-Z] 0x41 - 0x5A
struct TwoWayCircularLinkedList{
	struct TwoWayCircularLinkedList* pBefore;
	char cChar;
	struct TwoWayCircularLinkedList* pNext;
};
typedef struct TwoWayCircularLinkedList TWCLL;

int main(void) {
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

	pTWCLL = pTWCLL_Head;
	while (pTWCLL->pNext != pTWCLL_Head){
		printf("%c", pTWCLL->cChar);
		pTWCLL = pTWCLL->pNext;
	}
	printf("%c\n\n", pTWCLL->cChar);

	printf("Please input the length:\n");
	scanf_s("%d", &iUserInput);
	iCounter = iUserInput;

	pTWCLL = pTWCLL_Head;
	if (iCounter > 0) {
		while (iCounter != 1) {
			pTWCLL = pTWCLL->pNext;
			iCounter--;
		}
		pTWCLL_Head = pTWCLL->pNext;

		pTWCLL = pTWCLL_Head;
		while (pTWCLL->pNext != pTWCLL_Head) {
			printf("%c", pTWCLL->cChar);
			pTWCLL = pTWCLL->pNext;
		}
		printf("%c\n\n", pTWCLL->cChar);
	}
	else if ( iCounter < 0) {
		while (iCounter != -1) {
			pTWCLL = pTWCLL->pBefore;
			iCounter++;
		}
		pTWCLL_Head = pTWCLL->pBefore;

		pTWCLL = pTWCLL_Head;
		while (pTWCLL->pNext != pTWCLL_Head) {
			printf("%c", pTWCLL->cChar);
			pTWCLL = pTWCLL->pNext;
		}
		printf("%c\n\n", pTWCLL->cChar);
	}

	system("pause");
	return 0;
}
