/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : 051107_programmingPractise.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 13/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : DEBUG added. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 13122018    lst97       1      First release
;* 13122018    lst97       2      Fix pCOMMAND() function bug
;* Known Issue       :
;*
;* 1.COMMAND mode problem scanf() 2.No error handling when user input non-integer type.
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SPACEBAR 0x20
#define BINARY 2
#define BIT 8
#define DAY 7
#define WEEK 4
#define MOUNTH 12
#define TRUE 1
#define FALSE 0

int pCOMMAND(void);
int pDEBUG(void);
int pEND(void);

_Bool debugSwitch = 1;
int main(void) {
	const unsigned short int numLimited;
	unsigned int userInput;
	unsigned int userInput_temp;

	printf("Please enter a day [0 ~ %u]\n", (unsigned short int)pow(BINARY, (sizeof numLimited) * BIT));
	scanf("%u", &userInput);
	userInput_temp = userInput;

	printf("%u Day = %u Year / %u Weeks + %u Days", userInput, userInput_temp / (DAY * WEEK * MOUNTH), userInput_temp / DAY, userInput_temp % DAY);
	if(debugSwitch == 1){printf("\t*DEBUG[userInput]= '%X'\t[userInput_temp]= '%X'\t", userInput, userInput_temp);}

	printf("\n\nEnter q to exit\n");
	pCOMMAND();
	return 0;
}

int pCOMMAND(void){
	const unsigned short int arrayNum = 2;
		char userCommand[arrayNum];

		while(TRUE){
			scanf("%s", userCommand);
			if(strlen(userCommand) == 1){
				if(userCommand[0] == 'd'){
					pDEBUG();
					userCommand[0] = SPACEBAR;
					pCOMMAND();
				}else if (userCommand[0] == 'q'){
					pEND();
				}
			}
			printf("%s: Command not found!\n", userCommand);
			userCommand[0] = SPACEBAR;
		}
		return 0;
}

int pDEBUG(void){
	if(debugSwitch == TRUE){
		debugSwitch = FALSE;
	}else{
		debugSwitch = TRUE;
	}
	printf("Debug Mode = %u\n", debugSwitch);
	return 0;
}

int pEND(void){
	printf("Program terminated normally\n");
	exit(0);
}
/*RESULT
Please enter a day [0 ~ 65535]
8192
8192 Day = 24 Year / 1170 Weeks + 2 Days	*DEBUG[userInput]= '2000'	[userInput_temp]= '2000'

Enter q to exit
d
Debug Mode = 0
q
Program terminated normally

;END RESULT*/
