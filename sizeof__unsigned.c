/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : sizeof__unsigned.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/12/2018
;*
;* Version			 : Internal
;*
;* Copyright   		 : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Introduce 'sizeof()', signed, unsigned. Ansi-style
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

//ERROR (format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long unsigned int’ [-Wformat=])
int main(void) {
	printf("* * * [1 byte = 8 bit] = [0000 0000b ~ 1111 1111b] = [00h ~ FFh] = 2^8 = 256 * * *");
	printf("\n\nSize of 'short int'\t\t= %d byte", sizeof(short int));
	printf("\nSize of 'int'\t\t\t= %d byte", sizeof(int));
	printf("\nSize of 'long int'\t\t= %d byte", sizeof(long int));
	printf("\nSize of 'long long int'\t\t= %d byte\n", sizeof(long long int));

	printf("\nSize of 'float'\t\t\t= %d byte", sizeof(float));
	printf("\nSize of 'double'\t\t= %d byte", sizeof(double));
	printf("\nSize of 'long double'\t\t= %d byte\n", sizeof(long double));

	printf("\nSize of '_Bool'\t\t\t= %d byte\n", sizeof(_Bool));

	printf("\nSize of 'char'\t\t\t= %d byte\n\n", sizeof(char));

	printf("* * * signed v.s unsigned * * *");

	short int numA = -1;
	unsigned short int numB = -1;

	printf("\n\n'numA'\t= %d\n'numB'\t= %u",numA ,numB );

	getchar();
	return EXIT_SUCCESS;
}

/*RESULT
* * * [1 byte = 8 bit] = [0000 0000b ~ 1111 1111b] = [00h ~ FFh] = 2^8 = 256 * * *

Size of 'short int'		= 2 byte
Size of 'int'			= 4 byte
Size of 'long int'		= 8 byte
Size of 'long long int'		= 8 byte

Size of 'float'			= 4 byte
Size of 'double'		= 8 byte
Size of 'long double'		= 16 byte

Size of '_Bool'			= 1 byte

Size of 'char'			= 1 byte

* * * signed v.s unsigned * * *

'numA'	= -1
'numB'	= 65535
;END RESULT*/
