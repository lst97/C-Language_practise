/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : printf.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Introduce 'printf()' format argument. Ansi-style
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
	short int numA = 0xFFFF;
	short int numB = 65536;			//overflow = 1 00000000 00000000b
	printf("'numA'\t= %d\n", numA);
	printf("'numB'\t= %d %o %x %d %#o %#x %ld %lo %lx %hd %#ho %#hx %d %o %x %d %#o %#x \n", numB);		//Missing argument, it will read stuffs from memory.

	printf("\n* * * printf() Format * * *\n");
	printf("\n%%d\t\t= Decimo\n%%o\t\t= Octal\n%%x\t\t= Hex\n");
	printf("\n%%#o\t\t= Format display Octal\t%#o\n%%#h\t\t= Format display Hex\t%#x",numA, numA);

	printf("\n\n%%ld %%lo %%lx\t= Display long\n");
	printf("%%hd %%ho %%hx\t= Display short\n");

	printf("\n%%u\t\t= Display unsigned\n");

	getchar();
	return EXIT_SUCCESS;
}

/*RESULT
'numA'	= -1
'numB'	= 0 0 0 0 01 0x4bdf2840 281470681743360 2530721332313640 7fa5ad8ceb97 1 024110 0x8000 -1956014374 0 5dfc1406 -1956014640 011367624100 0

* * * printf() Format * * *

%d		= Decimo
%o		= Octal
%x		= Hex

%#o		= Format display Octal	037777777777
%#h		= Format display Hex	0xffffffff

%ld %lo %lx	= Display long
%hd %ho %hx	= Display short

%u		= Display unsigned

;END RESULT*/
