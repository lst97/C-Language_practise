/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : BinarySearch.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 09/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Binary Search using recursive.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 09062019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>

int fnBinarySearch(int*, int*, int);
int iArray[10] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };

int main(void) {
	int iSearch = 0, iAnswer = 0;
	int* piArray_Low = iArray, *piArray_High = iArray + 9;

	printf("Search for:\n");
	scanf_s("%d", &iSearch);

	iAnswer = fnBinarySearch(piArray_Low, piArray_High, iSearch);
	if (iAnswer > 0) printf("[%d]\n\n", iAnswer);
	else printf("No Found!\n\n");

	system("pause");
	return 0;
}

int fnBinarySearch(int* piMin, int* piMax, int iSearch) {
	int iOffset = (piMax - piMin) / 2, iGuess = *(piMin + iOffset), iTempOffset = 0;

	if (iOffset == 0) return -1;

	if (iGuess < iSearch) piMin = piMin + iOffset + 1;
	else if (iGuess > iSearch) piMax = piMin + iOffset - 1;
	else return -(iArray - (piMin + iOffset) - 1);
	return fnBinarySearch(piMin, piMax, iSearch);
}
