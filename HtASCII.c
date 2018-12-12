/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : HtASCII.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Hex number to ASCII. Ansi-style
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
	short int userInput;

	printf("Please enter a ASCII code in Hex\n");
	scanf("%hx", &userInput);
	printf("*DEBUG[sizeof(userInput)]\t= '%ld'\n",sizeof(userInput));
	printf("\nThe code represent '%c'", userInput);

	getchar();
	return 0;
}

/*RESULT
Please enter a ASCII code in Hex
61
*DEBUG[sizeof(userInput)]	= '2'

The code represent 'a'

;END RESULT*/



