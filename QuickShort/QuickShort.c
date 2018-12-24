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
;* 24122018    lst97       3      Performance improve, using calloc() instead of for loop to initialize memory
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/
#include <stdio.h>
#include <stdlib.h> //calloc()

//There are some different way for doing quickshort.
#define TRUE 0x01
#define FALSE 0x00
#define NULL 0

void swap(int*, int*);
unsigned short int partition(int*, int, int);
void quickShort(int *, int, int );

extern _Bool debugSwitch;
int QUICKSHORT(void) {
	//MAX amount of integer that user can input.
	printf("Loading QUICKSHORT()...\n");
	unsigned short int numberOfInteger;
	_Bool loopSwitch = TRUE;
	int *DMarrayList = NULL;
	int userInputArraySize;
	printf("SUCCESS!\n");

	printf("[*QUICKSHORT] Please enter a array size (1 ~ 65535):\n");
	do{
		if(scanf("%d", &userInputArraySize) != 1 || (userInputArraySize <= 0 || userInputArraySize >= 0xFFFF || getchar() != 0x000A)){
			printf("[*QUICKSHORT] Syntax ERROR! - invalid number detected\n");
			while(getchar() != 0x000A);
			continue;
		}
		break;
	}while(TRUE);
	DMarrayList = (int *)calloc(userInputArraySize, sizeof(int));

	//Get users' integer
	printf("[*QUICKSHORT] Please enter a group of integer:\n");
	for(unsigned short int accumla; loopSwitch == TRUE; accumla++){
		char checkEnd;
		if(accumla <= userInputArraySize -1){
			if(scanf("%d", &DMarrayList[accumla]) != 1){
				printf("[*QUICKSHORT] Syntax ERROR! - invalid number detected\n");
				free(DMarrayList);
				DMarrayList = (int *)calloc(userInputArraySize, sizeof(int));
				while(getchar() != 0x000A);
				accumla = -1;numberOfInteger = NULL;
				continue;
			}
			checkEnd = getchar();
			if(checkEnd == 0x000A){numberOfInteger = accumla;loopSwitch = FALSE;
			}else if(checkEnd != 0x0020){				//Input validator
				printf("[*QUICKSHORT] Syntax ERROR! - no space detected between integer\n");
				free(DMarrayList);
				DMarrayList = (int *)calloc(userInputArraySize, sizeof(int));
				while(getchar() != 0x000A);
				accumla = -1;numberOfInteger = NULL;
			}
		}else{
			printf("[*QUICKSHORT] Syntax ERROR! - out of array size\n");
			free(DMarrayList);
			DMarrayList = (int *)calloc(userInputArraySize, sizeof(int));
			while(getchar() != 0x000A);
			accumla = -1;numberOfInteger = 0;
		}
	}

	quickShort(DMarrayList, 0, numberOfInteger);
	for(unsigned short int accumula = 0; accumula <= numberOfInteger; accumula++){printf("%d ", DMarrayList[accumula]);}
	free(DMarrayList);
	printf("\nEXIT QUICKSHORT()...\nSUCCESS!\n");
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
