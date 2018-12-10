/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : temp.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 11/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Hello World in C, Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 11122018    lst97       1      First release
;*
;* Known Issue       :
;*
;* N/A
;|**********************************************************************;
*/

//'stdio.h' for keyboard input and graphic output.
#include <stdio.h>
#include <stdlib.h>

//'int main(void)' function name. 'int main()' will return a integer. 'void' do not need parameter.
int main(void) {
	int num;	//declaration
	num = 1;

	printf("I am a simple ");	//call a 'printf()' function.
	printf("computer.\n");
	printf("My favorite number is %d because it is first.\n", num); //'%d' tell CPU in which form to output the value of num.
	return EXIT_SUCCESS;
}
