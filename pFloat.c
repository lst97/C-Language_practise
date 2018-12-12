/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : pFloat.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Print floating point number. Ansi-style
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
#include <stdlib.h>

int main(void) {
	float userInput;

	printf("Please enter a floating-point number in Dex\n");
	scanf("%f", &userInput);
	printf("*DEBUG[sizeof(userInput)]\t= '%ld'\n",sizeof(userInput));
	printf("\nFixed-point notation\t= '%f'", userInput);
	printf("\nExponential notation\t= '%e'", userInput);

	getchar();
	return 0;
}

/*RESULT
Please enter a floating-point number in Dex
0.33333333333
*DEBUG[sizeof(userInput)]	= '4'

Fixed-point notation	= '0.333333'
Exponential notation	= '3.333333e-01'

;END RESULT*/



