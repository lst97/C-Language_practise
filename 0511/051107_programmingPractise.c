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
;*
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

int pCOMMAND(void);
void pDEBUG(_Bool torf);
int pEND(_Bool torf);

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
	pCOMMAND();
	return 0;
}

int pCOMMAND(void){
	const unsigned short int arrayNum = 2;
		char userCommand[arrayNum];
		printf("\n\nEnter q to exit\n");
		while(userCommand[0] != 'q' || userCommand[0] != 'd'){
			scanf("%s", userCommand);
			if(strlen(userCommand) != 1){
				printf("%s: Command not found!\n", userCommand);
				userCommand[0] = SPACEBAR;
			}else if(userCommand[0] == 'd'){
				if(debugSwitch == 1){
					pDEBUG(0);
				}else{
					pDEBUG(1);
				}
			}else{
				pEND(1);
			}
		}
		return 0;
}

void pDEBUG(_Bool torf){
	debugSwitch = torf;
	printf("Debug Mode = %u\n", debugSwitch);
}

int pEND(_Bool torf){
	printf("Program terminated normally\n");
	return 0;
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
