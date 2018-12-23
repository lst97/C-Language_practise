/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : QuickShort.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 22/12/2018
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : QuickShort algorithm. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 22122018    lst97       1      First release
;* 24122018    lst97       2      Now user can input the array size they what using malloc()
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/
#include <stdio.h>
#include <stdlib.h>

//There are some different way for doing quickshort.
#define TRUE 0x01
#define FALSE 0x00
#define NULL 0
#define SPACEBAR 0x0020

extern int CALC(void);

_Bool pCOMMAND(void);
_Bool pDEBUG(void);
_Bool pEND(void);

void swap(int*, int*);
unsigned short int partition(int*, int, int);
void quickShort(int *, int, int );

_Bool debugSwitch = FALSE;
int main(void) {
	//MAX amount of integer that user can input.
	unsigned short int numberOfInteger;
	_Bool loopSwitch = TRUE;
	int *DMarrayList = NULL;
	unsigned short int userInputArraySize;

	printf("[*SHELL] Please enter a array size:\n");
	scanf("%hu", &userInputArraySize);
	DMarrayList = (int *)malloc(userInputArraySize * sizeof(int));

	//Get users' integer
	printf("[*SHELL] Please enter a group of integer:\n");
	for(unsigned short int accumla = 0; loopSwitch == TRUE; accumla++){
		char checkEnd;
		if(accumla <= userInputArraySize -1){
			if(scanf("%d", &DMarrayList[accumla]) != 1){
				printf("[*SHELL] Syntax ERROR! - invalid number detected\n");
				for(unsigned short int accumula; accumula <= userInputArraySize; accumula++){DMarrayList[accumula] = NULL;}
				while(getchar() != 0x000A);
				accumla = -1;numberOfInteger = 0;
				continue;
			}
			checkEnd = getchar();
			if(checkEnd == 0x000A){numberOfInteger = accumla;loopSwitch = FALSE;
			}else if(checkEnd != 0x0020){				//Input validator
				printf("[*SHELL] Syntax ERROR! - no space detected between integer\n");
				for(unsigned short int accumula; accumula <= userInputArraySize; accumula++){DMarrayList[accumula] = NULL;}
				while(getchar() != 0x000A);
				accumla = -1;numberOfInteger = 0;
			}
		}else{
			printf("[*SHELL] Syntax ERROR! - out of array size\n");
			for(unsigned short int accumula; accumula <= userInputArraySize; accumula++){DMarrayList[accumula] = NULL;}
			while(getchar() != 0x000A);
			accumla = -1;numberOfInteger = 0;
		}
	}

	quickShort(DMarrayList, 0, numberOfInteger);
	for(unsigned short int accumula = 0; accumula <= numberOfInteger; accumula++){printf("%d ", DMarrayList[accumula]);}
	free(DMarrayList);
	pCOMMAND();
	printf("Program terminated normally!");
	return 0;
}

unsigned short int partition(int *arrayList, int headSerial, int tailSerial){
	int pivot = arrayList[tailSerial];
	int swapSerial = headSerial;

	for(unsigned short int accumla = headSerial; accumla < tailSerial; accumla++){
		if(arrayList[accumla] <= pivot){
			swap(&arrayList[swapSerial], &arrayList[accumla]);
			swapSerial++;
		}
	}
	swap(&arrayList[tailSerial], &arrayList[swapSerial]);
	return swapSerial; //Now it is partitionIndex for quickShort()
}

void swap(int *numA, int *numB){
	int temp;
	temp = *numB;*numB = *numA;*numA = temp;
}

void quickShort(int *arrayList, int headSerial, int tailSerial){
	if(headSerial < tailSerial){
		int partitionIndex = partition(arrayList, 0, tailSerial);
		quickShort(arrayList, headSerial, partitionIndex -1);
		quickShort(arrayList, partitionIndex +1, tailSerial);
	}
}

_Bool pCOMMAND(void){
	char userCommand = NULL;
	char Validator = NULL;
	char loopSwitch = TRUE;

	printf("\n[*SHELL] Please enter a command:\n");
	while(loopSwitch){
		loopSwitch = FALSE;
		userCommand = NULL;
		Validator = NULL;
		scanf("%c%c", &userCommand, &Validator);
		if(Validator == 0x000A || Validator == 0x0020){
			switch(userCommand){
			case 'q': pEND(); break;
			case 'd': pDEBUG(); loopSwitch = TRUE;break;
			case 'c': CALC(); loopSwitch = TRUE;printf("[*SHELL] Please enter a command:\n");while(getchar() != 0x000A);continue;
			default: printf("%c: is not a command!\n", userCommand); loopSwitch = TRUE; while(getchar() != 0x000A); continue;
			}
		}else if(Validator != NULL){
			printf("%c%c", userCommand, Validator);
			while(Validator != 0x0020 && Validator != 0x000A){
				scanf("%c", &Validator);
				if(Validator != 0x0020 && Validator != 0x000A) printf("%c", Validator);
			}
			printf(": is not a command!\n");
			if(Validator != 0x000A) while(getchar() != 0x000A);
			loopSwitch = TRUE;
		}
	}
	return 0;
}

_Bool pDEBUG(void){
	if(debugSwitch == TRUE)debugSwitch = FALSE;
	else debugSwitch = TRUE;
	printf("[*SHELL] Debug Mode = %u\n", debugSwitch);
	return 0;
}

_Bool pEND(void){
	return 0;
}
