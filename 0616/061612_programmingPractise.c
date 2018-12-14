/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : 061612_programmingPractise.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 14/12/2018
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Basic calculator. Ansi-style
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 14122018    lst97       1      First release
;* 15122018    lst97       2      Add more function add, sub, mul, div
;* Known Issue       :
;*
;* 1.Only calculate one type each time for now.
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define SPACEBAR 0x20
#define BINARY 2

#define BIT 8


int pCOMMAND(void);
int pDEBUG(void);
_Bool pEND(void);

double pAdd(double numA, double numB);

double pSub(double numA, double numB);
double pMul(double numA, double numB);
double pDiv(double numA, double numB);

_Bool debugSwitch = FALSE;
int main(void) {
	float numLimited;
	float userInput_float[2];
	char userInput_express;

	double answer_tempMul;
	double answer_temp;
	_Bool loopSwitch = TRUE;

	printf("Please enter formula to calculate [%.0f ~ %.0f]\n", (float)(-pow(BINARY, (sizeof numLimited * BIT)))/2, (float)(pow(BINARY, (sizeof numLimited * BIT))/2));

	scanf("%f%c", &userInput_float[0], &userInput_express);


		if(debugSwitch == 1)printf("\t[*]DEBUG\t(userInput_float[0])= '%f' (userInput_express)= '%c')", userInput_float[0], userInput_express);

		char nextExpress;
		//Classification
		do{
			if (userInput_express == 0x002B || userInput_express == 0x002D){
				scanf("%f", &userInput_float[1]);
				nextExpress = getchar();
				if (nextExpress == 0x000A && userInput_express == 0x002B){
					userInput_float[0] = (double)pAdd(userInput_float[0], userInput_float[1]);
					printf("%lf", userInput_float[0]);
					goto lpEND;
				}else if(nextExpress == 0x000A && userInput_express == 0x002D){
					userInput_float[0] = (double)pSub(userInput_float[0], userInput_float[1]);
					printf("%lf", userInput_float[0]);
					goto lpEND;
				}
				if(nextExpress == 0x002B || nextExpress == 0x002D){
					switch (userInput_express){
						case 0x002B:
							userInput_float[0] = (double)pAdd(userInput_float[0], userInput_float[1]);
							userInput_express = nextExpress;
						break;
						case 0x002D:
							userInput_float[0] = (double)pSub(userInput_float[0], userInput_float[1]);
							break;
					}
				}else{
					answer_tempMul = userInput_float[1];
					userInput_express = nextExpress;
				}
			}

			if (userInput_express == 0x002A || userInput_express == 0x002F){
				scanf("%f", &userInput_float[1]);
				nextExpress = getchar();

				if (nextExpress == 0x000A && userInput_express == 0x002A){
					userInput_float[0] = (double)pMul(userInput_float[0], userInput_float[1]);
					printf("%lf", userInput_float[0]);
					goto lpEND;
				}else if(nextExpress == 0x000A && userInput_express == 0x002F){
					userInput_float[0] = (double)pDiv(userInput_float[0], userInput_float[1]);
					printf("%lf", userInput_float[0]);
					goto lpEND;
				}
				switch (userInput_express){
					case 0x002A:
						userInput_float[0] = (double)pMul(userInput_float[0], userInput_float[1]);
						userInput_express = nextExpress;
						break;
					case 0x002F:
						userInput_float[0] = (double)pDiv(userInput_float[0], userInput_float[1]);
						userInput_express = nextExpress;
						break;
				}
			}

		}while(loopSwitch);
lpEND:
	printf("\n\nEnter q to exit\n");
	pCOMMAND();
	printf("Program terminated normally\n");
	return 0;
}

double pAdd(double numA, double numB){
	double answer;
	answer = numA + numB;

	return answer;
}

double pSub(double numA, double numB){
	double answer;
	answer = numA - numB;
	return answer;
}

double pMul(double numA, double numB){
	double answer;
	answer = numA * numB;
	return answer;
}

double pDiv(double numA, double numB){
	double answer;
	answer = numA / numB;
	return answer;
}


int pCOMMAND(void){
	const unsigned short int arrayNum = 1;
	char userCommand[arrayNum];
	_Bool loopSwitch = TRUE;

	do{
		scanf("%s", userCommand);
		if(strlen(userCommand) == 1){
			switch (userCommand[0]){
			case 'd':
				pDEBUG();
				userCommand[0] = SPACEBAR;
				continue;
			case 'q':
				pEND();
				loopSwitch = (int)pEND();
				break;
			default:
			printf("%s: Command not found!\n", userCommand);
			}
		}else {
			printf("%s: Command not found!\n", userCommand);
			while (getchar() != 0x000A);
		}
	}while(loopSwitch);
	return 0;
}

int pDEBUG(void){
	if(debugSwitch == TRUE)debugSwitch = FALSE;
	else debugSwitch = TRUE;
	printf("Debug Mode = %u\n", debugSwitch);
	return 0;
}

_Bool pEND(void){
	return 0;
}
/*RESULT
Please enter formula to calculate [-2147483648 ~ 2147483648]
1+23+3
27.000000

Enter q to exit
q
Program terminated normally

;END RESULT*/
