// This code compiled by gcc (Debian 6.3.0-18+deb9u1) 6.3.0 20170516
/*
;|**********************************************************************;
;* Project           : Misc
;*
;* Program name      : HowFloatWorks.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 17/06/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Dynamic Linked List and 2 Ways of middle search
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 17062019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <stdio.h>

int main(void){
	union{
		float fNum;
		unsigned int uiNum;
	}p;

	p.fNum = 263.3;
	unsigned int uiSign = (p.uiNum >> 31) & 1;
	unsigned int uiExp = (p.uiNum >> 23) & 0xFF;

	unsigned int uiCoef_Mask = (1 << 23) -1;
	unsigned int uiCoef = p.uiNum & uiCoef_Mask;

	printf("Sign\t: %d\n", uiSign);
	printf("Index\t: %d\n", uiExp);
	printf("Coef\t: 0x%X\n\n", uiCoef);

	printf("Press return to continue...\n");
	getchar();
	return 0;
}
