/*
;|**********************************************************************;
;* Project           : Data Structure & Algorithm
;*
;* Program name      : StackSimulator.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 07/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Simulate how stack works in memory && Binary converter.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 07062019    lst97       1      First release
;*
;* Known Issue       :
;* - Check base not 100% correct.
;* - Not a dynamic stack.
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define STACKSIZE 32

int* fnCreateStack(void);
int* fnPush(int *, int);
int* fnPop(int *);
void fnBinaryToDec(int *);
void fnBinaryToOct(int *);
void fnBinaryToHex(int*);

int* piStack_Base = 0, *piStack_Top = 0;

int main(void) {
	int iUserNumber = 0;

	piStack_Top = piStack_Base = fnCreateStack();

	printf("= = = = = = = = = = = =\n1.Push\n2.Pop\n3.Binary to Decima\n4.Binary to Octal\n5.Binary tp Hexadecima\n6.Reset\n-1.Exit\n\n");
	while (1) {
		printf("@StackSimulator>");
		scanf_s("%d", &iUserNumber);
		switch (iUserNumber) {
		case -1:
			printf("\n");
			system("pause");
			return 0;
		case 1: // PUSH
			printf("What number you wish to push:\n");
			scanf_s("%d", &iUserNumber);
			piStack_Top = fnPush(piStack_Top, iUserNumber);
			printf("\n");
			continue;
		case 2: // POP
			piStack_Top = fnPop(piStack_Top);
			printf("\n");
			continue;
		case 3:
			fnBinaryToDec(piStack_Top);
			continue;
		case 4:
			fnBinaryToOct(piStack_Top);
			continue;
		case 5:
			fnBinaryToHex(piStack_Top);
			continue;
		case 6:
			piStack_Top = piStack_Base;
			printf("Reset - SUCCESS!\n\n");
			continue;
		default:
			printf("Invalide Command!\n\n");
			while (getchar() != '\n');
			continue;
		}
	}
}

int* fnCreateStack(void) {
	return (int*)malloc(sizeof(int) * STACKSIZE);
}

int * fnPush(int *piStack, int iNumber) {
	*piStack = iNumber;
	return ++piStack;
}

int * fnPop(int *piStack) {
	piStack--;
	printf("[%d]\n", *piStack);
	return piStack;
}

int fnCheckValid(void) {
	int iDigit = 0, iCounter = 0;
	while (1) {
		iDigit = getchar();
		iDigit -= 0x30;
		if (iDigit == 0 || iDigit == 1) {
			piStack_Top = fnPush(piStack_Top, iDigit);
			iCounter++;
		}
		else if (iDigit == -38) break;
		else {
			printf("Invalid Binary!\n\n");
			while (getchar() != '\n');
			piStack_Top -= iCounter;
			return -1;
		}
	}
	return iCounter;
}

void fnBinaryToDec(int * piStack) {
	unsigned short iDigit = 0;
	signed short iCounter = 0;
	int iTempAns = 0;

	printf("Please enter a Binary number:\n");
	while (getchar() != '\n');

	iCounter = fnCheckValid();
	if (iCounter == -1) return;

	for (int iLoop = 0; iCounter > 0; iLoop++, iCounter--) {
		piStack_Top = fnPop(piStack_Top);
		iTempAns += (int)pow(2, iLoop) * *piStack_Top;
	}
	printf("= %d\n\n", iTempAns);
}

void fnBinaryToOct(int *piStack) {
	unsigned short iDigit = 0;
	signed short iCounter = 0;
	int* piTempStack = 0;

	printf("Please enter a Binary number:\n");
	while (getchar() != '\n');

	iCounter = fnCheckValid();
	if (iCounter == -1) return;

	piTempStack = (int*)malloc(sizeof(int) * ((iCounter/3) + 1));
	while (iCounter != 0) {
		for (int iLoop = 0; iLoop < 3 && iCounter != 0; iLoop++, iCounter--) { // pop 3 digit to tempstack after calc
			piStack_Top = fnPop(piStack_Top);
			*piStack_Top <<= iLoop;
			iDigit |= *piStack_Top;
		}
		*piTempStack = iDigit;
		piTempStack++;
	}
	printf("= ");
	while (1){
		piTempStack--;
		if (*piTempStack >=0 && *piTempStack <= 8) { // Bug - not 100% correct (lst97 07062019)
			printf("%d", *piTempStack);
		} else break;
	}
	printf("\n\n");
	free(++piTempStack);
}

void fnBinaryToHex(int* piStack) {
	unsigned short iDigit = 0;
	signed short iCounter = 0;
	int* piTempStack = 0;

	printf("Please enter a Binary number:\n");
	while (getchar() != '\n');

	iCounter = fnCheckValid();
	if (iCounter == -1) return;

	piTempStack = (int*)malloc(sizeof(int) * ((iCounter/4) + 1));
	while (iCounter != 0) {
		iDigit = 0;
		for (unsigned short iLoop = 0; iLoop < 4 && iCounter != 0; iLoop++, iCounter--) {
			piStack_Top = fnPop(piStack_Top);
			*piStack_Top <<= iLoop;
			iDigit |= *piStack_Top;
		}
		*piTempStack = iDigit;
		piTempStack++;
	}
	printf("= ");
	while (1) {
		piTempStack--;
		if (*piTempStack >= 0 && *piTempStack <= 15) {
			printf("%X", *piTempStack);
		}
		else break;
	}
	printf("\n\n");
	free(++piTempStack);
}
