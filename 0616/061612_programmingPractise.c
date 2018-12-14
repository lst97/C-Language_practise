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
;*
;* Known Issue       :
;*
;* 1.Only calculate add for now.
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
/*
double pSub(double numA, double numB);
double pMul(double numA, double numB);
double pDiv(double numA, double numB);
*/
_Bool debugSwitch = FALSE;
int main(void) {
	float numLimited;
	float userInput_int[2];
	char userInput_express;
	double answer_temp;

	_Bool loopSwitch = TRUE;

	printf("Please enter formula to calculate [%.0f ~ %.0f]\n", (float)(-pow(BINARY, (sizeof numLimited * BIT)))/2, (float)(pow(BINARY, (sizeof numLimited * BIT))/2));

	scanf("%f%c%f", &userInput_int[0], &userInput_express, &userInput_int[1]);


	if(debugSwitch == 1)printf("\t[*]DEBUG\t(userInput_int[0])= '%f' (userInput_express)= '%c' (userInput_int[1]= '%f')",
	userInput_int[0], userInput_express, userInput_int[1]);


	//Classification
	if (userInput_express == 0x2B){
	answer_temp = (double)pAdd(userInput_int[0],userInput_int[1]);
		do{
			//Follow-Up
			char nextExpress;
			nextExpress = getchar();
			if (nextExpress == 0x000A){
				printf("%lf", answer_temp);
				loopSwitch = FALSE;
			}else{
				userInput_express = nextExpress;
				scanf("%f", &userInput_int[0]);
				answer_temp = (unsigned int)pAdd(answer_temp, userInput_int[0]);
			}
		}while(loopSwitch);
	}

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
/*
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
*/

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
Please enter a day [0 ~ 65535]
8192
8192 Day = 24 Year / 1170 Weeks + 2 Days	*DEBUG[userInput]= '2000'	[userInput_temp]= '2000'

Enter q to exit
d
Debug Mode = 0
q
Program terminated normally

;END RESULT*/
