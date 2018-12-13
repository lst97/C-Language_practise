/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : 051103_programmingPractise.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 13/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Calculate Day converted to Year/Weeks + day. Ansi-style
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
#define DAY 7
#define WEEK 4
#define MOUNTH 12

int pEND(void);

int main(void) {
	const unsigned short int numLimited;
	unsigned int userInput;
	unsigned int userInput_temp;

	printf("Please enter a day [0 ~ %u]\n", (unsigned short int)pow(BINARY, (sizeof numLimited) * BIT));
	scanf("%u", &userInput);
	userInput_temp = userInput;

	printf("%u Day = %u Year / %u Weeks + %u Days\t", userInput, userInput_temp / (DAY * WEEK * MOUNTH), userInput_temp / DAY, userInput_temp % DAY);

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
Please enter a day [0 ~ 65535]
1000
1000 Day = 2 Year / 142 Weeks + 6 Days

Enter q to exit
q

Program terminated normally

;END RESULT*/
