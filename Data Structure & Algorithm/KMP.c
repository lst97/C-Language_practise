/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : KMP.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 10/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : KMP algorithm
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 10062019    lst97       1      First release
;*
;* Known Issue       :
;* - Major bug "aaaaax" exception && infinite loop when special cases "aaaa"
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH 32

void fnKMP(char *);

int main(void) {
	char szArray[LENGTH] = { "aaaabcde" };
	char* pszArray = szArray;

	printf("%s\n\n", szArray);
	fnKMP(pszArray);

	system("pause");
	return 0;
}

void fnKMP(char *pszArray) {
	struct KMP_Table {
		char szSearch[LENGTH];
		short iInitialField[LENGTH];
	}MatchTable;

	bool bFoundFlag = false;
	unsigned short usSearchLength = 0, usLoopInside = 0, usGap = 0, usArrayLength = (unsigned short)strlen(pszArray), usStartPoint = 0;
	char* pszSearch = MatchTable.szSearch;
	char cTempChar = 0;

	printf("Searching for:\n");
	while (cTempChar != '\n') {
		cTempChar = getchar();
		*pszSearch = cTempChar;
		pszSearch++;
		usSearchLength++;
	}
	*(pszSearch-1) = '\0';
	pszSearch = MatchTable.szSearch;

	MatchTable.iInitialField[0] = -1;
	MatchTable.iInitialField[1] = 0;
	for (unsigned short usLoopOutside = 2; usLoopOutside < usSearchLength; usLoopOutside++) {
		while (1){
			if (*pszSearch == *(pszSearch + usGap + 1)) {
				pszSearch++;
				usLoopInside++;
				if (usLoopInside == usLoopOutside -1) {
					MatchTable.iInitialField[usLoopOutside] = usLoopInside - usGap;
					usLoopInside = 0;
					pszSearch = MatchTable.szSearch;
					usGap = 0;
					break;
				}
			} else {
				usGap++;
				pszSearch = MatchTable.szSearch;
				usLoopInside = usGap;
			}
			if (usGap == usLoopOutside - 1) {
				MatchTable.iInitialField[usLoopOutside] = 0;
				usLoopInside = 0;
				usGap = 0;
				pszSearch = MatchTable.szSearch;
				break;
			}
		}
	}

	// Matching
	if (usSearchLength -1 <= usArrayLength) {
		for (unsigned short usLoopOutside = 0; usLoopOutside < usArrayLength; usLoopOutside++) {
			for (unsigned short usLoopInside = usStartPoint; usLoopInside <= usSearchLength; usLoopInside++) {
				if (usLoopInside == usSearchLength - 1) {
					printf("Match Found [%02d]\n", usLoopOutside + 1);
					bFoundFlag = true;
					usStartPoint = 0;
					break;
				}
				if (*(pszArray + usLoopOutside + usLoopInside) != MatchTable.szSearch[usLoopInside]) {
					if (MatchTable.iInitialField[usLoopInside] == -1 || MatchTable.iInitialField[usLoopInside] == 0) {
						usStartPoint = 0;
						break;
					}
					else {
						usStartPoint = MatchTable.iInitialField[usLoopInside];
						usLoopOutside--; // Major bug "aaaaax" exception && infinite loop when special cases "aaaa", fix when free (lst97 10062019)
					}
				}
			}
		}
		if (!bFoundFlag) printf("No Found!\n\n");
		else printf("\n");
	}
	else printf("No Found! (Search string oversize)\n\n");
}
