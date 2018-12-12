/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : define__rounding__sizeLimitation__string__endProgramFunction.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Introduce #define, Rounding, Size Limitation, String & add 'END program' function instead of getchar(). Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 12122018    lst97       1      First release
;*
;* Known Issue       :
;*
;* N/A
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define SPACEBAR 0x20
#define PI 3.14159625

int main(void) {
	//Rounding
	printf("Pi is = %1.2f", PI);

	//Show Limitation
	printf("\n\n* * * The number limits for this system * * *\n");
	printf("Biggest int = %41d\n", INT_MAX);
	printf("Smallest long long = %44lld\n", LLONG_MIN);
	printf("One byte = %d bits on this system.", CHAR_BIT);

	//END program
	const unsigned short int arrayNum = 2;
	char userCommand[arrayNum];
	printf("\n\nEnter q to exit\n");
	while(userCommand[0] != 'q'){
		scanf("%s", userCommand);				//Do not need & if it's a string
		if(strlen(userCommand) != 1){
			userCommand[0] = SPACEBAR;
		}
	}
	printf("\nProgram terminated normally");
	return 0;
}
/*RESULT
Pi is = 3.14

* * * The number limits for this system * * *
Biggest int =                                2147483647
Smallest long long =                         0
One byte = 8 bits on this system.

Enter q to exit
q

Program terminated normally

;END RESULT*/



