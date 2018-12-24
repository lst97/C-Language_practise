/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : main.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 24/12/2018
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : A command block to call other functions in same folder. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 24122018    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/
#include <stdio.h>
#include <stdlib.h> //exit()

//There are some different way for doing quickshort.
#define TRUE 0x01
#define FALSE 0x00
#define NULL 0
#define SPACEBAR 0x0020

extern int QUICKSHORT(void);
extern int CALC(void);

_Bool pCOMMAND(void);
_Bool pDEBUG(void);
_Bool pEND(void);

_Bool debugSwitch = FALSE;
int main(void) {
	pCOMMAND();
	return 0;
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
