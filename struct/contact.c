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
;* 04012019    lst97       3      Program rewrote using linked list instead, add basic validator, fix search bug
;*
;* Known Issue       :
;*
;* 1. Email validate not fully functional.
;* 2. Not clean char buffer correctly, sometime invalid message pop up.
;* 3. Havn't check memory leak.
;* 4. Havn't check if calloc() fail.
;* 5. Didn't limited data base size.
;* 6. Didn't check free() is function correctly or not.
;*
;* Note:
;* I will add more function in this program: short, search improvement, memory pool, file in/out.
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //exit()

#define SPACEBAR 0x0020
#define cleanChar() while(getchar() != 0x000A)

enum Bool {null, FALSE = 0, TRUE};

struct Contact{
	char fName[16];
	char lName[16];
	char email[32];
	char phone[16];
	struct Contact *nextContactPtr;
};

void showMenu(void);
void addContact(struct Contact **, _Bool *, short unsigned int *);
void userContactInput(struct Contact *, _Bool *, short unsigned int *, _Bool);
void editContact(struct Contact **, _Bool *, short unsigned int *);
void deleteContact(struct Contact **, _Bool *, short unsigned int *);
void searchContact(struct Contact **, _Bool *, short unsigned int *);
void printContact(struct Contact *, _Bool);
int exitContact(struct Contact *, short unsigned int contactSize);

_Bool pCOMMAND(void);
_Bool pDEBUG(void);
_Bool pEND(void);

_Bool debugSwitch = FALSE;
int main(void) {
	struct Contact *contactHead = null;
	short unsigned int contactSize = null;
	short unsigned int userCommand = null;
	_Bool freshFlag = TRUE;

	showMenu();
	while(TRUE){
		userCommand = null;
		scanf("%hu", &userCommand);
		switch (userCommand){
		case 1:
			if(freshFlag == TRUE){
				cleanChar();
				addContact(&contactHead, &freshFlag, &contactSize);
			}
			_Bool invalidFlag = FALSE;
			while(TRUE){
				if(invalidFlag == FALSE)printf("[Contact] Data/s found, do you wish to add another contact? (y/n)\n");
				invalidFlag = FALSE;
				char userYN = null;
				scanf("%1c", &userYN);
				if(userYN == 'y'){
					cleanChar();
					addContact(&contactHead, &freshFlag, &contactSize);
				}else if(userYN == 'n'){break;
				}else{
					printf("[Contact] Invalid Input! try again\n");
					invalidFlag = TRUE;
				}
			}
			printf("\n= = = = = = = = Add Contact END = = = = = = = =\n");
			showMenu();
			continue;
		case 2: editContact(&contactHead, &freshFlag, &contactSize); showMenu(); continue;
		case 3: deleteContact(&contactHead, &freshFlag, &contactSize); showMenu(); continue;
		case 4: searchContact(&contactHead, &freshFlag, &contactSize); showMenu(); continue;
		case 5: printContact(contactHead, freshFlag); showMenu(); continue;
		case 6: if(exitContact(contactHead, contactSize) == 1) goto END; continue;
		}
	}

END:
	printf("EXIT CONTACT()...\nSUCCESS!\n");
	cleanChar();
	pCOMMAND();
	return 0;
}

void showMenu(void){
	printf(";|**********************************************************************;\n;*\n"
			";*\t1.Add contact\t\t\t2.Edit a contact\n;*\n"
			";*\t3.Delete a contact\t\t4.Search a contact\n;*\n"
			";*\t5.Print contact\t\t\t6.EXIT!\n;*\n"
			";|**********************************************************************;\n\n");
	printf("[Contact] Please perform an action:\n");
	return;
}

void addContact(struct Contact **contactHead, _Bool *freshFlag, short unsigned int *contactSize){
	struct Contact *newContact;
	static struct Contact *newContact_Tail;

	newContact = (struct Contact *)calloc(1, sizeof(struct Contact));
	if(newContact == null){printf("[Contact] ERROR Fail when allocation memory!\n"); exit(1);}

	userContactInput(newContact, freshFlag, contactSize, FALSE);
	if(*contactHead != null){newContact_Tail->nextContactPtr = newContact;
	}else *contactHead = newContact;

	newContact_Tail = newContact;
}

void userContactInput(struct Contact *contact, _Bool *freshFlag, short unsigned int *contactSize, _Bool fromEdit){
	if(fromEdit == TRUE){printf("\n= = = = = = = = Edit Contact [%hu] = = = = = = = =\n", *contactSize +1);
	}else {printf("\n= = = = = = = = Add Contact [%hu] = = = = = = = =\n", *contactSize +1);}

	char strValida = null;
	char sizeCheck = null;
	char *charPosistionPtr = null;

fname:
	printf("[Contact] Please enter First Name: (Less than 16 char and only capital allowed)\n");
	charPosistionPtr = (char *)contact;
	while(strValida != 0x000A){
		scanf("%1c", &strValida);
		if(((strValida < 0x0041 && strValida != 0x000A) && strValida != 0x0020) || strValida > 0x005A){			// Only allow capital letter, ' ', '\n' see ASCII table.
			charPosistionPtr = (char *)contact;				// Have to relocate pointer if Invalid as I am adding pointer itself.
			printf("[Contact] Invalid Input! try again\n");
			memset (contact->fName, null, 16);
			cleanChar();
		}else{
			if(strValida != 0x000A){						// Save letter one by one if valid.
				memset(charPosistionPtr, strValida, 1);
				charPosistionPtr++;
				sizeCheck++;
			}
		}
	}
	if(sizeCheck > 15){
		printf("[Contact] Over size! try again\n");
		memset (contact->fName, null, 16);
		goto fname;
	}

	printf("[Contact] Please enter Last Name:\n");
lname:
	strValida = null; sizeCheck = null;
	charPosistionPtr = (char *)contact;
	while(strValida != 0x000A){
		scanf("%1c", &strValida);
		if(((strValida < 0x0041 && strValida != 0x000A) && strValida != 0x0020) || strValida > 0x005A){
			charPosistionPtr = (char *)contact;
			printf("[Contact] Invalid Input! try again\n");
			memset (contact->lName, null, 16);
			cleanChar();
		}else{
			if(strValida != 0x000A){
				memset(charPosistionPtr +16, strValida, 1);
				charPosistionPtr++;
				sizeCheck++;
			}
		}
	}
	if(sizeCheck > 15){
			printf("[Contact] Over size! try again\n");
			memset (contact->lName, null, 16);
			goto lname;
		}

	printf("[Contact] Please enter E-Mail:\n");
email:													// I am looking for an email validator, I will try to do it myself first. It contain problem for now.
	strValida = null; sizeCheck = null;
	_Bool atFlag = FALSE;
	charPosistionPtr = (char *)contact;
	while(strValida != 0x000A){
		scanf("%1c", &strValida);
		//Basic validate allow {'a-z', 'A-Z', '0-9', '@', ' ', '\n', '.'}
		if((strValida == 0x000A) || (strValida == 0x002E) || (strValida > 0x002F && strValida < 0x003A) ||
				(strValida > 0x003F && strValida < 0x005B) || (strValida > 0x0060 && strValida < 0x007B)){
			if(strValida == 0x0040) atFlag = TRUE;
			if(strValida != 0x000A){
				memset(charPosistionPtr +32, strValida, 1);
				charPosistionPtr++;
				sizeCheck++;
			}
		}else{
			charPosistionPtr = (char *)contact;
			printf("[Contact] Invalid Input! try again\n");
			memset (contact->email, null, 32);
		}
	}
	if(atFlag != TRUE){							// E-Mail must have '@' sign.
		printf("[Contact] Invalid Input! try again\n");
		memset (contact->email, null, 32);
		goto email;
	}
	if(sizeCheck > 31){
		printf("[Contact] Over size! try again\n");
		memset (contact->email, null, 32);
		goto email;
	}


	printf("[Contact] Please enter Phone:\n");
phone:
	strValida = null;
	sizeCheck = null;
	charPosistionPtr = (char *)contact;
	while(strValida != 0x000A){
		scanf("%1c", &strValida);
		if((strValida < 0x003A && strValida > 0x002F) || strValida == 0x000A){			// Allow only number and '\n'
			if(strValida != 0x000A){
				memset(charPosistionPtr +64, strValida, 1);
				charPosistionPtr++;
				sizeCheck++;
			}
		}else{
			charPosistionPtr = (char *)contact;
			printf("[Contact] Invalid Input! try again\n");
			memset (contact->phone, null, 16);
		}
	}
	if(sizeCheck > 16){
			printf("[Contact] Over size! try again\n");
			memset (contact->phone, null, 16);
			goto phone;
		}

	if(fromEdit == TRUE){printf("\n= = = = = = = = Edit Contact END = = = = = = = =\n");
	}else {printf("\n= = = = = = = = Add Contact END = = = = = = = =\n");}

	*freshFlag = FALSE;					// Flag will FALSE if contain data or *contactSize > 0.
	*contactSize = *contactSize +1;
	cleanChar();
	return;
}
void editContact(struct Contact **contact, _Bool *freshFlag, short unsigned int *contactSize){
	if(*freshFlag == TRUE){printf("[Contact] Data base empty! please add contact first\n");return;}

	struct Contact *contactTemp = null;
	short unsigned int userInput = null;

	contactTemp = *contact;
	printContact(*contact, *freshFlag);
	printf("[CONTACT] Which contact you want to edit?\n" );
	while(TRUE){
		if(scanf("%1hu", &userInput) != 1 || userInput == 0 || userInput > *contactSize){
			printf("[CONTACT] Over size! try again\n");
			cleanChar();
			continue;
		}
			short unsigned int accumla = 1;
			while(accumla == userInput -1){					// Tail insertion, learn more on YouTube: https://www.youtube.com/watch?v=NobHlGUjV3g&index=3&list=PL2_aWCzGMAwI3W_JlcBbtYTwiQSsOTa6P
				contactTemp = contactTemp->nextContactPtr;
				accumla++;
			}
			userInput--;
			cleanChar();
			userContactInput(contactTemp, freshFlag, &userInput, TRUE);
			break;
	}
}

void deleteContact(struct Contact **contactHead, _Bool *freshFlag, short unsigned int *contactSize){
	if(*freshFlag == TRUE){printf("[Contact] Data base empty! please add contact first\n");return;}

	struct Contact *contactTemp = null;
	struct Contact *contactDelete_Temp = null;
	struct Contact *contactBefore = null;
	short unsigned int userInput = null;

	contactTemp = *contactHead;
	printContact(*contactHead, *freshFlag);
	printf("[CONTACT] Which contact you want to delete?\n" );
	while(TRUE){
		if(scanf("%1hu", &userInput) != 1 || userInput == 0 || userInput > *contactSize){
			printf("[CONTACT] Invalid Input! try again\n");
			cleanChar();
			continue;
		}
		// Have not check if free() function correctly or not.
		if(*contactSize == 1){
			*contactHead = contactTemp->nextContactPtr;
			free(*contactHead);
		}else{
			short unsigned int accumla = 1;
			while(accumla < userInput){
				contactBefore = contactTemp;				// Have to remember the address of the before contact.
				contactDelete_Temp = contactTemp->nextContactPtr;
				contactTemp = contactTemp->nextContactPtr;
				accumla++;
			}
			contactBefore->nextContactPtr = contactTemp->nextContactPtr;
			free(contactDelete_Temp);
		}
		cleanChar();
		*contactSize = *contactSize -1;
		if(*contactSize == 0)*freshFlag = TRUE;
		break;
	}
}

void searchContact(struct Contact **contactHead, _Bool *freshFlag, short unsigned int *contactSize){
	if(*freshFlag == TRUE){printf("[Contact] Data base empty! please add contact first\n");return;}
	if(*contactSize < 2){printf("[Contact] Too few data for search! use Print contact instead\n");return;}

	struct Contact *contactTemp = null;
	char accumla = null;
	char userInput = null;
	char charTempArray[16];
	contactTemp = *contactHead;

	printf("= = = = = = = = Search Contact = = = = = = = =\n");
	printf("[CONTACT] Please enter First Name for search:\n" );
	memset(charTempArray, null, 16);		// Array have to be clean, search trash if did not
	cleanChar();
	while(TRUE){
		while(TRUE){
			scanf("%1c", &userInput);
			if(((userInput < 0x0041 && userInput != 0x000A) && userInput != 0x0020) || userInput > 0x005A){
				printf("[CONTACT] Invalid Input! try again\n");
				cleanChar();
				memset(charTempArray, null, 16);
				accumla = null;
				continue;
			}else{
				if(userInput != 0x000A){						// Save to array and ready for compare if Input valid.
					charTempArray[accumla] = userInput;			// Unknown error, program works OK. (array subscript has type ‘char’ [-Wchar-subscripts]) eclipse 4.8.0
					accumla++;
				}else break;
			}
		}

		char accumla = null;
		_Bool foundFlag = FALSE;
		while(accumla < *contactSize -1){
			if(!strcmp(contactTemp->fName, charTempArray)){
				printf("\n= = = = = = = = Print Contact [%d] = = = = = = = =\n", accumla +1);
				printf("First Name:\t\t%s\n", contactTemp->fName);
				printf("Last Name:\t\t%s\n", contactTemp->lName);
				printf("E-Mail:\t\t\t%s\n", contactTemp->email);
				printf("Phone:\t\t\t%s\n", contactTemp->phone);
				accumla++;
				foundFlag = TRUE;
			}
			contactTemp = contactTemp->nextContactPtr;
		}
		if(foundFlag == FALSE)printf("[CONTACT] Nothing Found!\n");
		printf("\n= = = = = = = = [%hu] Contact FOUND = = = = = = = =\n", accumla);
		printf("= = = = = = = = Search Contact END = = = = = = = =\n");
		break;
	}

}

void printContact(struct Contact *contactHead, _Bool freshFlag){
	struct Contact *contactTemp;
	char accumla = 1;

	// Linked list better explained on YouTube.
	contactTemp = contactHead;
	while(contactTemp != null){
		printf("\n= = = = = = = = Print Contact [%d] = = = = = = = =\n", accumla);
		printf("First Name:\t\t%s\n", contactTemp->fName);
		printf("Last Name:\t\t%s\n", contactTemp->lName);
		printf("E-Mail:\t\t\t%s\n", contactTemp->email);
		printf("Phone:\t\t\t%s\n", contactTemp->phone);
		contactTemp = contactTemp->nextContactPtr;
		accumla++;
	}
	if(freshFlag == TRUE){
		printf("Data base EMPTY!\n");
	}
	printf("\n= = = = = = = = Print Contact END = = = = = = = =\n");
	return;
}

int exitContact(struct Contact *contactHead, short unsigned int contactSize){
	struct Contact *contactTemp = null;
	struct Contact *contactTemp_Temp = null;

	printf("[CONTACT] Exit will wipe all the data, you sure? (y/n)\n");
	contactTemp = contactHead;

	// Have not check if free() function correctly or not.
	while(TRUE){
		char userYN = null;
		cleanChar();
		scanf("%1c", &userYN);
		if(userYN == 'y'){
			if(contactSize == 0){return 1;}
			else if(contactSize == 1){
				contactHead = contactTemp->nextContactPtr;
				free(contactHead);
				return 1;
			}else{
				contactTemp = contactHead;
				while(contactTemp->nextContactPtr != null){
					contactTemp = contactTemp->nextContactPtr;
					contactTemp_Temp = contactTemp;
					free(contactTemp_Temp);
					return 1;
				}
			}
		}else if(userYN == 'n'){return 0;}
		else printf("[Contact] Invalid Input! try again\n");

	}
}

_Bool pCOMMAND(void){
	char userCommand = null;
	char Validator = null;
	char loopSwitch = TRUE;

	printf("[*SHELL] Please enter a command:\n");
	while(loopSwitch){
		loopSwitch = FALSE;
		userCommand = null;
		Validator = null;
		scanf("%c%c", &userCommand, &Validator);
		if(Validator == 0x000A){
			switch(userCommand){
			case 'q': pEND(); continue;
			case 'd': pDEBUG(); loopSwitch = TRUE; continue;
			case 'c': CALC(); break;
			case 's': QUICKSHORT(); break;
			default: printf("%c: is not a command!\n", userCommand); loopSwitch = TRUE; continue;
			}
			loopSwitch = TRUE;printf("[*SHELL] Please enter a command:\n");if(getchar() != 0x000A) cleanChar();continue;
		}else{
			printf("%c%c", userCommand, Validator);
			while(Validator != 0x0020 && Validator != 0x000A){
				scanf("%c", &Validator);
				if(Validator != 0x0020 && Validator != 0x000A) printf("%c", Validator);
			}
			if(getchar() != 0x000A) cleanChar();
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
