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
;* Purpose           : Simulate how stack works in memory.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 07062019    lst97       1      First release
;* 08062019    lst97       2      Add Reverse Polish Notation && Basic Calculator
;*
;* Known Issue       :
;* - Check base not 100% correct.
;* - Not a dynamic stack.
;* - Do not support negative number -1+4 should be 4-1. try to fix when free. (lst97 08062019)
;*
;|**********************************************************************;
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define STACKSIZE 64

int* fnCreateStack(void);
int* fnPush(int*, int);
int* fnPop(int*);
void fnBinaryToDec(int*);
void fnBinaryToOct(int*);
void fnBinaryToHex(int*);
void fnReversePolishNotation(char*);
void fnBasicCalc(void);

int* piStack_Base = 0, *piStack_Top = 0;

int main(void) {
	int iUserNumber = 0;
	char szExpression[32], cTemp = 0;
	char* pcExpression = szExpression;
	piStack_Top = piStack_Base = fnCreateStack();

	printf("= = = = = = = = = = = =\n1.Push\n2.Pop\n3.Binary to Decima\n4.Binary to Octal\n5.Binary tp Hexadecima\n6.Reset\n7.Reverse Polish Notation\n8.Basic Calculator\n-1.Exit\n\n");
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
		case 7:
			while (getchar() != '\n');
			printf("Please input the notation:\n");
			while (cTemp != '\n'){
				cTemp = getchar();
				*pcExpression = cTemp;
				pcExpression++;
			}
			cTemp = 0;
			*(--pcExpression) = '\0';
			pcExpression = szExpression;

			fnReversePolishNotation(pcExpression);
			
			int iTempCounter = strlen(szExpression);	// Clear buffer
			for (int iLoop = 0; iLoop <= iTempCounter; iLoop++) {
				szExpression[iLoop] = '\0';
			}
			continue;
		case 8:
			fnBasicCalc();
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

int* fnPush(int* piStack, int iNumber) {
	*piStack = iNumber;
	return ++piStack;
}

int* fnPop(int* piStack) {
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

void fnBinaryToDec(int* piStack) {
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

void fnBinaryToOct(int* piStack) {
	unsigned short iDigit = 0;
	signed short iCounter = 0;
	int* piTempStack = 0;

	printf("Please enter a Binary number:\n");
	while (getchar() != '\n');

	iCounter = fnCheckValid();
	if (iCounter == -1) return;

	piTempStack = (int*)malloc(sizeof(int) * ((iCounter / 3) + 1));
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
	while (1) {
		piTempStack--;
		if (*piTempStack >= 0 && *piTempStack <= 8) { // Bug - not 100% correct (lst97 07062019)
			printf("%d", *piTempStack);
		}
		else break;
	}
	printf("\n\n");
	free(++piTempStack);
}

void fnBinaryToHex(int* piStack) {
	unsigned short iDigit = 0;
	short iCounter = 0;
	int* piTempStack = 0;

	printf("Please enter a Binary number:\n");
	while (getchar() != '\n');

	iCounter = fnCheckValid();
	if (iCounter == -1) return;

	piTempStack = (int*)malloc(sizeof(int) * ((iCounter / 4) + 1));
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

void fnReversePolishNotation(char *pszExpression) {
	char lfBuffer[STACKSIZE];
	short iCounter = 0;
	double lfTempLeft = 0, lfTempRight = 0;
	double *plfStack_Base = 0, *plfStack_Top = 0;

	plfStack_Base = plfStack_Top = (double*)malloc(sizeof(double) * STACKSIZE / 4);
	while (*pszExpression != '\0'){
		while (isdigit(*pszExpression) || *pszExpression == '.') {
			lfBuffer[iCounter] = *pszExpression;
			pszExpression++;
			if (*pszExpression == ' ') {
				lfBuffer[iCounter + 1] = '\0';
				*plfStack_Top = atof(lfBuffer);
				for (int iLoop = 0; iLoop <= iCounter; iLoop++) lfBuffer[iCounter] = '\0'; // reset buffer
				
				iCounter = 0;
				plfStack_Top++;
				pszExpression++;
				break;
			}
			iCounter++;
		}

		switch (*pszExpression) {
		case '+':
		case '-':
		case '*':
		case '/':
			plfStack_Top--;
			lfTempRight = *plfStack_Top;
			plfStack_Top--;
			lfTempLeft = *plfStack_Top;
			if (*pszExpression == '/' && lfTempRight == 0) {
				printf("Math ERROR!\n\n");
				free(plfStack_Base);
				return;
			}
			if (*pszExpression == '+')* plfStack_Top = lfTempLeft + lfTempRight;
			else if(*pszExpression == '-')* plfStack_Top = lfTempLeft - lfTempRight;
			else if(*pszExpression == '*')* plfStack_Top = lfTempLeft * lfTempRight;
			else *plfStack_Top = lfTempLeft / lfTempRight;
			plfStack_Top++;
			pszExpression++;
			break;
		case ' ':
			pszExpression++;
			break;
		}
	}
	printf("= %lf\n\n", *plfStack_Base);
	free(plfStack_Base);
}

void fnBasicCalc(void) {
	char szFormular[STACKSIZE], szConvertBuffer[STACKSIZE], cTemp = 0;
	char *pszFormular = szFormular, *pszConvertBuffer = szConvertBuffer, *pcSymbol_Base = 0, *pcSymbol_Top = 0;

	pcSymbol_Base = pcSymbol_Top = (char*)malloc(sizeof(char) * STACKSIZE / 4);

	while (getchar() != '\n');
	printf("Please input formular:\n");
	while (cTemp != '\n') {
		cTemp = getchar();
		*pszFormular = cTemp;
		pszFormular++;
	}
	*(pszFormular - 1) = '\0'; // Replace '\n'
	pszFormular = szFormular;

	// Convert to Postfix Expression than execute fnReversePolishNotation function.
	while (*pszFormular != '\0') {
		while (isdigit(*pszFormular) || *pszFormular == '.') {
			*pszConvertBuffer = *pszFormular;
			pszConvertBuffer++;
			pszFormular++;
			if (*pszFormular != '.' && !isdigit(*pszFormular)) {
				*pszConvertBuffer = ' ';
				pszConvertBuffer++;
			}
		}

		switch (*pszFormular) {
		case '+':
		case '-':
			while (*(pcSymbol_Top - 1) == '*' || *(pcSymbol_Top - 1) == '/') { // Possible short the code? as it duplicate with the if.
				*pszConvertBuffer = *(pcSymbol_Top - 1);
				pszConvertBuffer++;
				*pszConvertBuffer = ' ';
				pszConvertBuffer++;
				pcSymbol_Top--;
			}
			if (*(pcSymbol_Top - 1) == '+' || *(pcSymbol_Top - 1) == '-') {
				*pszConvertBuffer = *(pcSymbol_Top - 1);
				pszConvertBuffer++;
				*pszConvertBuffer = ' ';
				pszConvertBuffer++;
				pcSymbol_Top--;
			}
			if (*pszFormular == '+') *pcSymbol_Top = '+';
			else *pcSymbol_Top = '-';
			break;
		case '*':
		case '/':
			if (*pszFormular == '*') *pcSymbol_Top = '*';
			else *pcSymbol_Top = '/';
			break;
		case '(':
			*pcSymbol_Top = '(';
			break;
		case ')':
			pcSymbol_Top--;
			while (*pcSymbol_Top != '(') {
				*pszConvertBuffer = *pcSymbol_Top;
				pszConvertBuffer++;
				*pszConvertBuffer = ' ';
				pszConvertBuffer++;
				pcSymbol_Top--;
				pszFormular++;
			}
			continue;
		default:
			if (*pszFormular) {
				printf("Maths ERROR!\n\n");
				free(pcSymbol_Base);
				return;
			}
			break;
		}
		if (*pszFormular) { // End of the formular it will not run.
			pcSymbol_Top++;
			pszFormular++;
		}
	}

	// Pop the rest of the symbols;
	while (pcSymbol_Top != pcSymbol_Base) {
		*pszConvertBuffer = *(pcSymbol_Top-1);
		pszConvertBuffer++;
		*pszConvertBuffer = ' ';
		pszConvertBuffer++;
		pcSymbol_Top--;
	}
	*(pszConvertBuffer-1) = '\0';
	printf("%s\n", szConvertBuffer);

	pszConvertBuffer = szConvertBuffer;
	fnReversePolishNotation(pszConvertBuffer);

	free(pcSymbol_Base);
}
