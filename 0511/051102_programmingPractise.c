/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : 051102_programmingPractise.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 13/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Add 1 to user input by 10 times. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 13122018    lst97       1      First release
;*
;* Known Issue       :
;*
;* scanf() is not the best way to check user input as it will check the whole input include space.
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SPACEBAR 0x20
#define BINARY 2
#define BIT 8

int pEND(void);

int main(void) {
	const unsigned short int numLimited;
	unsigned int userInput;
	unsigned int userInput_temp;

	printf("Please enter a number [0 ~ %u]\n", (unsigned short int)pow(BINARY, (sizeof numLimited) * BIT));
	scanf("%u", &userInput);
	userInput_temp = userInput;

	while(userInput <= userInput_temp + 10){
		printf("%u\t", userInput++);
	}

	pEND();
	printf("\nProgram terminated normally");
	return 0;
}

int pEND(void){
	const unsigned short int arrayNum = 2;
		char userCommand[arrayNum];
		printf("\n\nEnter q to exit\n");
		while(userCommand[0] != 'q'){
			scanf("%s", userCommand);				//Do not need & if it's a string
			if(strlen(userCommand) != 1){
				printf("%s: Command not found!\n", userCommand);
				userCommand[0] = SPACEBAR;
			}
		}
		return 0;
}
/*RESULT
Please enter a number [0 ~ 65535]
65535
65535	65536	65537	65538	65539	65540	65541	65542	65543	65544	65545

Enter q to exit
q

Program terminated normally

;END RESULT*/
