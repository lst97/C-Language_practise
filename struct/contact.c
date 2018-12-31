/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : contact.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 30/12/2018
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Work as a normal contact list. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 30122018    lst97       1      First release
;* 31122018    lst97       2      Search function add
;*
;* Known Issue       :
;*
;* 1. All function working properly except Validation.
;* 2. Search function bug if input contain 0x0020.
;*
;* Note:
;* I am working on this program using 'Link List' for performance improve. Planed to rewrite.
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //exit()

#define TRUE 0x01
#define FALSE 0x00
#define NULL 0
#define SPACEBAR 0x0020

struct Contact *creatContact(unsigned short int *, _Bool *, struct Contact *);
void editContact(unsigned short int *, _Bool *, struct Contact *);
void deleteContact(unsigned short int *, _Bool *, struct Contact *);
void searchContact(unsigned short int *, _Bool *, struct Contact *);
void printContact(unsigned short int *, _Bool *, struct Contact *);
void printContact(unsigned short int *, _Bool *, struct Contact *);
void exitContact(struct Contact *);
void showMenu(void);

_Bool pCOMMAND(void);
_Bool pDEBUG(void);
_Bool pEND(void);

struct ContactName{
	char fName[16];
	char lName[16];
};
struct Contact{
	struct ContactName StructName;
	char company[32];
	char email[32];
	char phone[16];
};

_Bool debugSwitch = FALSE;
int main(void) {
	unsigned short int userCommand = NULL;
	_Bool loopSwitch = TRUE;
	struct Contact *contactPointer = NULL;
	unsigned short int structSize;
	_Bool freshFlag = TRUE;

	showMenu();
	printf("[CONTACT] Please perform an action:\n");
	while(loopSwitch){
		loopSwitch = FALSE;
		userCommand = NULL;
		if(scanf("%hu", &userCommand) == 1){
			switch(userCommand){
			case 1: contactPointer = creatContact(&structSize, &freshFlag, contactPointer); break;
			case 2: editContact(&structSize, &freshFlag, contactPointer); printContact(&structSize, &freshFlag, contactPointer);; break;
			case 3: deleteContact(&structSize, &freshFlag, contactPointer); break;
			case 4: searchContact(&structSize, &freshFlag, contactPointer); break;
			case 5: printContact(&structSize, &freshFlag, contactPointer); break;
			case 6: exitContact(contactPointer); goto END; break;
			default: printf("[CONTACT] Invalid Input!\n[CONTACT] Please perform an action:\n"); loopSwitch = TRUE; continue;
			}
			loopSwitch = TRUE; showMenu(); printf("[CONTACT] Please perform an action:\n"); while(getchar() != 0x000A);continue;
		}else {printf("[CONTACT] Invalid Input!\n[CONTACT] Please perform an action:\n"); loopSwitch = TRUE; while(getchar() != 0x000A);}
	}

END:
	while(getchar() != 0x000A);
	pCOMMAND();
	return 0;
}

struct Contact *creatContact(unsigned short int *structSize, _Bool *freshFlag, struct Contact *structPointer){
	char chooseYN = 'y';
	while (chooseYN == 'y' && *freshFlag == FALSE){
		printf("[CONTACT] Contact contain data, new data will replace it, continue? (y/n)\n");
		while(getchar() != 0x000A);
		scanf("%1c", &chooseYN);
		if(chooseYN == 'n') return 0;

		if(chooseYN == 'y'){
			for(unsigned short int accmula; accmula <= *structSize -1; accmula++){
				free(structPointer + accmula);
			}
			chooseYN = 0;
			continue;
		}else{
			printf("[CONTACT] Invalid Input! ");
			chooseYN = 'y';
		}
	}
	printf("[CONTACT] How many contact you want to create (1 ~ 16):\n");
	while(scanf("%hu", structSize) != 1 || *structSize == 0 || *structSize > 16 || getchar() != 0x000A){
		printf("[CONTACT] Invalid Input! try again\n");
		while(getchar() != 0x000A);
	}

	structPointer = (struct Contact*)calloc(*structSize, sizeof(struct Contact));
	if(structPointer == NULL){printf("[CONTACT] Memory allocation ERROR!\n"); exit(1);}

	for(unsigned short int accumla = 0; accumla <= *structSize-1; accumla++){
		char checkSpace;
		char additionalString;

		printf("\n= = = = = = = = Create Contact [%hu] = = = = = = = =\n", accumla +1);
		printf("[CONTACT] Please enter the first name:\n");
		scanf("%s", (structPointer + accumla)->StructName.fName); // @suppress("Format String Vulnerability")
		scanf("%c", &checkSpace);
		while(checkSpace == 0x0020){
			scanf("%s", &additionalString); // @suppress("Format String Vulnerability")
			strcat((structPointer + accumla)->StructName.fName, &checkSpace);
			scanf("%c", &checkSpace);
		}
		printf("[CONTACT] Please enter the last name:\n");
		scanf("%s", (structPointer + accumla)->StructName.lName); // @suppress("Format String Vulnerability")
		scanf("%c", &checkSpace);
		while(checkSpace == 0x0020){
			scanf("%s", &additionalString); // @suppress("Format String Vulnerability")
			strcat((structPointer + accumla)->StructName.lName, &checkSpace);
			scanf("%c", &checkSpace);
		}
		printf("[CONTACT] Please enter the company:\n");
		scanf("%s", (structPointer + accumla)->company); // @suppress("Format String Vulnerability")
		printf("[CONTACT] Please enter the phone:\n");
		scanf("%s", (structPointer + accumla)->phone); // @suppress("Format String Vulnerability")
		printf("[CONTACT] Please enter the email:\n");
		scanf("%s", (structPointer + accumla)->email); // @suppress("Format String Vulnerability")
	}
	*freshFlag = FALSE;
	printf("\n= = = = = = = = Create Contact END = = = = = = = =\n");
	return structPointer;
}

void editContact(unsigned short int *structSize, _Bool *freshFlag, struct Contact *structPointer){
	if(*freshFlag == TRUE){printf("[CONTACT] Database Empty! Return...\n");return;}

	unsigned short int userInputNum;
	printf("[CONTACT] Which contact you want to edit:\n");
	while(getchar() != 0x000A);
	scanf("%hu", &userInputNum);
	while(userInputNum > *structSize || userInputNum == 0){
		printf("[CONTACT] Contact not found!\nWhich contact you want to edit:\n");
		while(getchar() != 0x000A);
		scanf("%hu", &userInputNum);
	}

	printf("\n= = = = = = = = Edit Contact [%hu] = = = = = = = =\n", userInputNum);
	printf("[CONTACT] Please enter the first name:\n");
	scanf("%s", (structPointer + (userInputNum -1))->StructName.fName); // @suppress("Format String Vulnerability")
	printf("[CONTACT] Please enter the last name:\n");
	scanf("%s", (structPointer + (userInputNum -1))->StructName.lName); // @suppress("Format String Vulnerability")
	printf("[CONTACT] Please enter the company:\n");
	scanf("%s", (structPointer + (userInputNum -1))->company); // @suppress("Format String Vulnerability")
	printf("[CONTACT] Please enter the phone:\n");
	scanf("%s", (structPointer + (userInputNum -1))->phone); // @suppress("Format String Vulnerability")
	printf("[CONTACT] Please enter the email:\n");
	scanf("%s", (structPointer + (userInputNum -1))->email); // @suppress("Format String Vulnerability")
	return;
}

void deleteContact(unsigned short int *structSize, _Bool *freshFlag, struct Contact *structPointer){
	if(*freshFlag == TRUE){
		printf("[CONTACT] Database Empty! Return...\n");
		return;
	}

	_Bool loopSwitch;
	unsigned short int deleteNum;
	while(loopSwitch){
		printf("[CONTACT] Which contact you want to delete:\n");
		while(getchar() != 0x000A);
		scanf("%hu", &deleteNum);
		if(deleteNum > *structSize || deleteNum == 0){
			printf("[CONTACT] Invalid input! array over size\n");
			continue;
		}
		loopSwitch = FALSE;
	}

	for(unsigned short int accumla = 0; accumla < deleteNum -1; accumla++, structPointer++);
	for(; deleteNum < *structSize; structPointer++, deleteNum++){
		memcpy(structPointer, structPointer + 1, sizeof(struct Contact));
	}
	memset(structPointer, NULL, sizeof(struct Contact));
	structPointer = structPointer - (*structSize -1);
	*structSize = *structSize -1;
	if(*structSize == 0){
		*freshFlag = TRUE;
	}
	printf("= = = = = = = = Delete Contact [%hu] = = = = = = = =\nSUCCESS!\n", deleteNum);
	printf("= = = = = = = = = = = = = = = = = = = = = = = = = = \n");
	return;
}

void searchContact(unsigned short int *structSize, _Bool *freshFlag, struct Contact *structPointer){
	if(*freshFlag == TRUE){
		printf("[CONTACT] Database Empty! Return...\n");
		return;
	}
	char userSearch = NULL;
	_Bool matchFlag = FALSE;
	printf("[CONTACT] Please enter First Name for search\n");
	while(getchar() != 0x000A);
	scanf("%c", &userSearch); // @suppress("Format String Vulnerability")
	for(unsigned short int accumla = 0; accumla <= *structSize -1; accumla++){
		if(!strcmp((structPointer + accumla)->StructName.fName, &userSearch)){
			printf("= = = = = = = = Contact Found [%hu] = = = = = = = =\n", accumla + 1);
			printf("First Name:\t%s\n", (structPointer + accumla)->StructName.fName);
			printf("Last Name:\t%s\n", (structPointer + accumla)->StructName.lName);
			printf("Company:\t%s\n", (structPointer + accumla)->company);
			printf("Phone:\t\t%s\n", (structPointer + accumla)->phone);
			printf("E-mail:\t\t%s\n", (structPointer + accumla)->email);
			matchFlag = TRUE;
		}
	}
	if(matchFlag == FALSE)printf("[CONTACT] Nothing Found!\n");
	printf("\n= = = = = = = = Search Contact END = = = = = = = =\n");
	return;
}

void printContact(unsigned short int *structSize, _Bool *freshFlag, struct Contact *structPointer){
	if(*freshFlag == TRUE){
		printf("[CONTACT] Database Empty! Return...\n");
		return;
	}
	for(unsigned short int accumla = 0; accumla <= *structSize -1; accumla++){
		printf("\n= = = = = = = = Print Contact [%d] = = = = = = = =\n", accumla +1);
		printf("First Name:\t%s\n", (structPointer + accumla)->StructName.fName);
		printf("Last Name:\t%s\n", (structPointer + accumla)->StructName.lName);
		printf("Company:\t%s\n", (structPointer + accumla)->company);
		printf("Phone:\t\t%s\n", (structPointer + accumla)->phone);
		printf("E-mail:\t\t%s\n", (structPointer + accumla)->email);
	}
	printf("\n= = = = = = = = Print Contact END = = = = = = = =\n\n");
	return;
}

void exitContact(struct Contact *StructContact){
	free(StructContact);
	printf("Exit CONTACT()\nSUCCESS!\n");
	return;
}

void showMenu(void){
	printf(";|**********************************************************************;\n;*\n"
			";*\t1.Create contact\t\t2.Edit a contact\n;*\n"
			";*\t3.Delete a contact\t\t4.Search a contact\n;*\n"
			";*\t5.Print contact\t\t\t6.EXIT!\n;*\n"
			";|**********************************************************************;\n\n");
	return;
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
