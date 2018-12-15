/*
;|**********************************************************************;
;* Project           : Basic_C
;*
;* Program name      : 061612_programmingPractise.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 16/12/2018
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
;* 15122018    lst97       2      Add more function
;* 16122018    lst97       3      Recode
;*
;* Known Issue       :
;*
;* 1.I discovered I have running into a stupid way for doing this calculator, These kind of code will not be continue and I found a new way to deal with +- after /* problem. 
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
	float userInput_floatLowPio[64];
	unsigned short int userInput_floatLowPio_Counter;
	char userInput_express;

	double answer_tempFront;
	double answer_temp;
	double answer_tempMul;
	_Bool loopSwitch = TRUE;
	_Bool loopSwitch_continMul = TRUE;
	printf("Please enter formula to calculate [%.0f ~ %.0f]\n", (float)(-pow(BINARY, (sizeof numLimited * BIT)))/2, (float)(pow(BINARY, (sizeof numLimited * BIT))/2));



		if(debugSwitch == 1)printf("\t[*]DEBUG\t(userInput_float[0])= '%f' (userInput_express)= '%c')", userInput_float[0], userInput_express);

		char nextExpress;
		float nextFolat;
		//Classification
		scanf("%f%c%f", &userInput_float[0], &userInput_express, &userInput_float[1]);
		nextExpress = getchar();

		if(userInput_express == 0x002A || userInput_express == 0x002F){
			if(userInput_express == 0x002A){
				answer_tempFront = (double)pMul(userInput_float[0], userInput_float[1]);
			}

			if(userInput_express == 0x002F){
				answer_tempFront = (double)pDiv(userInput_float[0], userInput_float[1]);
			}
		}else if(nextExpress == 0x002A || nextExpress == 0x002F){
			userInput_floatLowPio[1] = userInput_float[0];
			userInput_floatLowPio_Counter++;
			answer_tempMul = userInput_float[1];

		}else if(nextExpress == 0x002B || nextExpress == 0x002D){

			if(userInput_express == 0x002B){
				answer_tempFront = (double)pAdd(userInput_float[0], userInput_float[1]);
			}

			if(userInput_express == 0x002D){
				answer_tempFront = (double)pSub(userInput_float[0], userInput_float[1]);
			}
		}

		if(nextExpress == 0x000A){
			if(userInput_express == 0x002B){
				answer_temp = (double)pAdd(userInput_float[0], userInput_float[1]);
				printf("%lf", answer_temp);
				loopSwitch = FALSE;
			}
			if(userInput_express == 0x002D){
				answer_temp = (double)pSub(userInput_float[0], userInput_float[1]);
				printf("%lf", answer_temp);
				loopSwitch = FALSE;
			}
			if(userInput_express == 0x002A){
				answer_temp = (double)pMul(userInput_float[0], userInput_float[1]);
				printf("%lf", answer_temp);
				loopSwitch = FALSE;
			}
			if(userInput_express == 0x002F){
				answer_temp = (double)pDiv(userInput_float[0], userInput_float[1]);
				printf("%lf", answer_temp);
				loopSwitch = FALSE;
			}
		}
		userInput_express = nextExpress;

		while(loopSwitch){
			scanf("%f", &userInput_float[0]);
			nextExpress = getchar();
			if(nextExpress == 0x000A){
				if(userInput_express == 0x002B){
					answer_temp = (double)pAdd(answer_tempFront, userInput_float[0]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
				if(userInput_express == 0x002D){
					answer_temp = (double)pSub(answer_tempFront, userInput_float[0]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
				if(userInput_express == 0x002A){
					answer_temp = userInput_floatLowPio[userInput_floatLowPio_Counter] + (double)pMul(answer_tempMul, userInput_float[0]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
				if(userInput_express == 0x002F){
					answer_temp = userInput_floatLowPio[userInput_floatLowPio_Counter] + (double)pDiv(answer_tempMul, userInput_float[0]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
			}

			if(userInput_express == 0x002A){
				answer_tempFront = (double)pMul(answer_tempMul, userInput_float[0]);
			}
			if(userInput_express == 0x002F){
				answer_tempFront = (double)pDiv(answer_tempMul, userInput_float[0]);
			}
			if(nextExpress == 0x002A){
				while(loopSwitch_continMul){
					scanf("%f", &userInput_float[1]);
					answer_tempFront = (double)pMul(answer_tempFront, userInput_float[1]);
					nextExpress = getchar();
					if(nextExpress == 0x000A){
						loopSwitch_continMul = FALSE;
					}
				}
			}

			userInput_express = nextExpress;

			scanf("%f", &userInput_float[1]);
			nextExpress = getchar();
			if(nextExpress == 0x000A){
				if(userInput_express == 0x0002B){
					answer_temp = answer_tempFront + (double)pAdd(userInput_float[0], userInput_float[1]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
				if(userInput_express == 0x0002D){
					answer_temp = answer_tempFront + (double)pSub(userInput_float[0], userInput_float[1]);
					printf("%lf", answer_temp);
					loopSwitch = FALSE;
					continue;
				}
			}
			if(nextExpress == 0x002B || nextExpress == 0x002D){
				if(nextExpress == 0x002B){
					answer_tempFront = answer_tempFront + (double)pAdd(userInput_float[0], userInput_float[1]);

				}
				if(nextExpress == 0x002D){
					answer_tempFront = answer_tempFront + (double)pSub(userInput_float[0], userInput_float[1]);

				}
			}
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
