/*
;|**********************************************************************;
;* Project           : Practise Tools (C Language)
;*
;* Program name      : Hex_to_Binary.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 19/08/2019
;*
;* Version           : internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Practise Hex to Binary calculation with human brain.
;*
;* Revision History  :
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 19082019    lst97       1      First release
;*
;* Known Issue       :
;*
;* TODO              :
;* mixed mode
;*
;|**********************************************************************;
*/

#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void showMenue();
char collectInput();	// Collect user input in menue page
bool isValid(char);		// Menu input validation
bool basicMode();		// return true = user wants to quit
signed bIsValid(char userInput);	// return -1 = user pressed 'Q' | false = Invalid input | true = Valid input
void mixedMode();		// In development
void processAns(unsigned, char [], unsigned);
void showAns(unsigned);

int main() {
	char userInput = 0;
	showMenue();

	while (true) {
		userInput = collectInput();
		switch (userInput) {
		case '1':
			do {
				system("cls");
			} while (!basicMode());
			break;
		case '2':
			mixedMode();
			break;
		}
		system("cls");
		showMenue();
	}
	return 0;
}

void showMenue() {
	printf("Hex to Binary practise tool\n\n1. Basic Mod\n2. Mixed Mod (Developing)\n\n>");		// 19082019
}

char collectInput() {
	char selectedNum = 0;
	selectedNum = getchar();
	if (!isValid(selectedNum)) {
		printf("Invalid input!\n\n>");
		return 0;
	}

	return selectedNum;
}

bool isValid(char selectedNum) {
	while (getchar() != '\n')

	if (selectedNum != '1' || selectedNum != '2')
		return false;
	return true;
}

bool basicMode() {
	char userInput[4];
	signed returnValue = 0;

	srand((unsigned int)time(NULL));
	unsigned rNum = (unsigned)rand() % 15;

	printf("Press 'Q' to exit\n\n%X\n\n>", rNum);

	for (unsigned counter = 0; counter < 4; counter++) {
		userInput[counter] = getchar();
		returnValue = bIsValid(userInput[counter]);
		if (returnValue == -1)
			return true;
		else if (returnValue == false) {
			printf("Invalid Input!\n");
			while (getchar() != '\n');
			Sleep(500);
			return false;
		}
	}

	processAns(rNum, userInput, 1);
	while (getchar() != '\n');
	return false;
}

signed bIsValid(char userInput) {
	if (userInput == '0' || userInput == '1')
		return true;
	else if (userInput == 'q' || userInput == 'Q')
		return -1;
	else
		return false;
}

void mixedMode() {

}

void processAns(unsigned rNum, char userInput[], unsigned mode) {
	unsigned tempDec = 0;
	switch (mode) {
	case 1:
		for (signed counter = 3; counter >= 0; counter--)
			tempDec += (userInput[counter] - 48) * (unsigned)pow(2, -(counter - 3));
		if (rNum == tempDec) {
			printf("Correct!\n");
			Sleep(500);
			return;
		}
		printf("Incorrect! (");
		showAns(rNum);
		Sleep(500);
		break;
	case 2:
		break;
	}

	return;
}

void showAns(unsigned rNum) {
	char *charStack = NULL;
	charStack = (char *)malloc(sizeof(char) * 4);

	for (signed counter = 4; counter > 0; counter--) {
		if (rNum & 1)
			*charStack = 0x31;
		else
			*charStack = 0x30;
		charStack++;
		rNum >>= 1;
	}

	for (unsigned counter = 4; counter > 0; counter--) {
		printf("%c", *(charStack -1));
		charStack--;
	}
	printf(")\n");

	free(charStack);
	return;
}
