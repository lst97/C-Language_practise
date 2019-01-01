/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : linkList_HeadInsertion.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 31/12/2018
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : A template showing Link List - Head Insertion. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 31122018    lst97       1      First release
;*
;* Known Issue       :
;*
;* 1. Straight forward process, no return.
;*
;* Reference:
;*
;* 1. https://www.bilibili.com/video/av27744141/?p=45
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //exit()

#define TRUE 0x01
#define FALSE 0x00
#define NULL 0
#define SPACEBAR 0x0020

struct Contact{
	char email[32];
	char phone[16];
	struct Contact *nextContactPtr;
};

void createContact(struct Contact **);
void userContactInput(struct Contact *);
void printContact(struct Contact *);
void cleanContact(struct Contact *);

_Bool pCOMMAND(void);
_Bool pDEBUG(void);
_Bool pEND(void);

_Bool debugSwitch = FALSE;
int main(void) {
	struct Contact *contactHead = NULL;
	char userYN;
	while(TRUE){
		printf("Do you want to create a contact (y/n):\n");
		do{
			scanf("%1c", &userYN);
			if(getchar() != 0x000A)while(getchar() != 0x000A);
			if(userYN != 'y' && userYN != 'n')printf("Invalid Input! try again:\n");
		}while(userYN != 'y' && userYN != 'n');
		if(userYN == 'y'){
			createContact(&contactHead);
		}else break;
		if(getchar() != 0x000A)while(getchar() != 0x000A);
	}

	printf("Print contact? (y/n)\n");
	userYN = NULL;
	do{
		scanf("%1c", &userYN);
		if(getchar() != 0x000A)while(getchar() != 0x000A);
		if(userYN != 'y' && userYN != 'n')printf("Invalid Input! try again:\n");
	}while(userYN != 'y' && userYN != 'n');
	if(userYN == 'y'){
		printContact(contactHead);
	}
	cleanContact(contactHead);

	pCOMMAND();
	return 0;
}

void createContact(struct Contact **contactHead){
	struct Contact *contactPointer;
	struct Contact *contactPointer_Temp;
	contactPointer = (struct Contact*)calloc(1, sizeof(struct Contact));
	if(contactPointer == NULL){printf("Memory allocating fail!\n");exit(1);}

	userContactInput(contactPointer);
	if(*contactHead != NULL){
		contactPointer_Temp = *contactHead;
		*contactHead = contactPointer;
		contactPointer->nextContactPtr = contactPointer_Temp;
	}else{
		*contactHead = contactPointer;
		contactPointer->nextContactPtr = NULL;
	}
}

void userContactInput(struct Contact *contactPointer){
	printf("Please enter the email:\n");
	scanf("%s", contactPointer->email); // @suppress("Format String Vulnerability")
	printf("Please enter the phone:\n");
	scanf("%s", contactPointer->phone); // @suppress("Format String Vulnerability")
	return;
}


void printContact(struct Contact *contactHead){
	struct Contact *contactPointer;
	short unsigned int accumla = 1;

	contactPointer = contactHead;
	while (contactPointer != NULL){
		printf("\n= = = = = = = = Print Contact [%hu] = = = = = = = =\n", accumla);
		printf("email:\t\t%s\n", contactPointer->email);
		printf("phone:\t\t%s\n", contactPointer->phone);
		contactPointer = contactPointer->nextContactPtr;
		accumla++;
	}
	printf("\n= = = = = = = = Print Contact END = = = = = = = =\n");
}

void cleanContact(struct Contact *contactHead){
	struct Contact *contactTemp;
	while(contactHead != NULL){
		contactTemp = contactHead;
		free(contactHead);
		contactHead = contactHead->nextContactPtr;
		free(contactTemp);
	}
}

_Bool pCOMMAND(void){
	char userCommand = NULL;
	char Validator = NULL;
	char loopSwitch = TRUE;

	printf("[*SHELL] Please enter a command:\n");
	while(loopSwitch){
		loopSwitch = FALSE;
		userCommand = NULL;
		Validator = NULL;
		scanf("%c%c", &userCommand, &Validator);
		if(Validator == 0x000A){
			switch(userCommand){
			case 'q': pEND(); continue;
			case 'd': pDEBUG(); loopSwitch = TRUE; continue;
			case 'c': CALC(); break;
			case 's': QUICKSHORT(); break;
			default: printf("%c: is not a command!\n", userCommand); loopSwitch = TRUE; continue;
			}
			loopSwitch = TRUE;printf("[*SHELL] Please enter a command:\n"); while(getchar() != 0x000A);continue;
		}else{
			printf("%c%c", userCommand, Validator);
			while(Validator != 0x0020 && Validator != 0x000A){
				scanf("%c", &Validator);
				if(Validator != 0x0020 && Validator != 0x000A) printf("%c", Validator);
			}
			while(getchar() != 0x000A);
			printf(": is not a command!\n");
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
	printf("Program terminated normally!");
	exit(0);
	return 0;
}
