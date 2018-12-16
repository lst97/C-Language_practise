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
;* 15122018    lst97       2      Add more function
;* 16122018    lst97       3      Recode
;* 17122018    lst97       4      Recoding, can solve basic math problem between two number
;*
;* Known Issue       :
;*
;* 1. As my skill not good at the moment, I don't want to spend too much time on this program, I will finish it after my skill grows up.
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
	double userInput_floatList[64];
	unsigned short int userInput_floatList_Counter = 0;
	char userInput_express;

	double answer_temp;
	_Bool loopSwitch = TRUE;
	_Bool loopSwitch_continMul = TRUE;
	_Bool loopSwitch_continAdd = TRUE;
	_Bool differentFound = FALSE;
	printf("Please enter formula to calculate [%.0f ~ %.0f]\n", (float)(-pow(BINARY, (sizeof numLimited * BIT)))/2, (float)(pow(BINARY, (sizeof numLimited * BIT))/2));



	if(debugSwitch == 1)printf("\t[*]DEBUG\t(userInput_float[0])= '%f' (userInput_express)= '%c')", userInput_float[0], userInput_express);
	char nextExpress;


	//Classification
	while(loopSwitch){
		userInput_express = nextExpress;

		scanf("%f", &userInput_float[1]);
		nextExpress = getchar();

		if(nextExpress == 0x000A && answer_temp == 0){
			if(userInput_express == 0x002B){
				answer_temp = (double)pAdd(userInput_float[0], userInput_float[1]);
			}
			if(userInput_express == 0x002D){
				answer_temp = (double)pSub(userInput_float[0], userInput_float[1]);
			}
			if(userInput_express == 0x002A){
				answer_temp = (double)pMul(userInput_float[0], userInput_float[1]);
			}
			if(userInput_express == 0x002F){
				answer_temp = (double)pDiv(userInput_float[0], userInput_float[1]);
			}
			userInput_floatList[userInput_floatList_Counter] = answer_temp;
			userInput_floatList_Counter++;
			goto result;
		}

		if(nextExpress == 0x000A){
			if(userInput_express == 0x002D){
				answer_temp = -userInput_float[1];
				userInput_floatList[userInput_floatList_Counter] = answer_temp;
				userInput_floatList_Counter++;
				goto result;
			}
			answer_temp = userInput_float[1];
			userInput_floatList[userInput_floatList_Counter] = answer_temp;
			userInput_floatList_Counter++;
			goto result;
		}

		//loopSwitch_continAdd?
		if((userInput_express == 0x002B || userInput_express == 0x002D) && (nextExpress == 0x002B || nextExpress == 0x002D)){
			loopSwitch_continAdd = TRUE;
			if(differentFound == FALSE){
				answer_temp = userInput_float[0];
				if(userInput_express == 0x002B){
					answer_temp = (double)pAdd(answer_temp, userInput_float[1]);
				}
				if(userInput_express == 0x002D){
					answer_temp = (double)pSub(answer_temp, userInput_float[1]);
				}
			}else{
				answer_temp = 0;
				if(userInput_express == 0x002B){
					answer_temp = (double)pAdd(answer_temp, userInput_float[1]);
				}
				if(userInput_express == 0x002D){
					answer_temp = (double)pSub(answer_temp, userInput_float[1]);
				}
			}


			while(loopSwitch_continAdd){
				userInput_express = nextExpress;
				scanf("%f", &userInput_float[1]);
				nextExpress = getchar();
				if(nextExpress == 0x002B || nextExpress == 0x002D){
					if(userInput_express == 0x002B){
						answer_temp = (double)pAdd(answer_temp, userInput_float[1]);
					}
					if(userInput_express == 0x002D){
						answer_temp = (double)pSub(answer_temp, userInput_float[1]);
					}
				}

				if(nextExpress == 0x002A || nextExpress == 0x002F){
					userInput_floatList[userInput_floatList_Counter] = answer_temp;
					userInput_floatList_Counter++;
					loopSwitch_continAdd = FALSE;

					userInput_float[0] = userInput_float[1];
				}
				if(nextExpress == 0x000A){
					if(userInput_express == 0x002B){
						answer_temp = (double)pAdd(answer_temp, userInput_float[1]);
					}
					if(userInput_express == 0x002D){
						answer_temp = (double)pSub(answer_temp, userInput_float[1]);
					}
					userInput_floatList[userInput_floatList_Counter] = answer_temp;
					userInput_floatList_Counter++;
					goto result;
				}
			}
		}

		//continueMul?
		if((userInput_express == 0x002A || userInput_express == 0x002F) && (nextExpress == 0x002A || nextExpress == 0x002F)){
			loopSwitch_continMul = TRUE;
			if(differentFound == FALSE){
				answer_temp = userInput_float[0];
				if(userInput_express == 0x002A){
					answer_temp = (double)pMul(answer_temp, userInput_float[1]);
				}
				if(userInput_express == 0x002F){
					answer_temp = (double)pDiv(answer_temp, userInput_float[1]);
				}
			}else{
				answer_temp = 0;
				if(userInput_express == 0x002A){
					answer_temp = (double)pMul(answer_temp, userInput_float[1]);
				}
				if(userInput_express == 0x002F){
					answer_temp = (double)pDiv(answer_temp, userInput_float[1]);
				}
			}

			while(loopSwitch_continMul){
				userInput_express = nextExpress;
				scanf("%f", &userInput_float[1]);
				nextExpress = getchar();
				if(nextExpress == 0x002A || nextExpress == 0x002F){
					if(userInput_express == 0x002A){
						answer_temp = (double)pMul(answer_temp, userInput_float[1]);
					}
					if(userInput_express == 0x002F){
						answer_temp = (double)pDiv(answer_temp, userInput_float[1]);
					}
				}

				if(nextExpress == 0x002B || nextExpress == 0x002D){
					if(userInput_express == 0x002A){
						answer_temp = (double)pMul(answer_temp, userInput_float[1]);
					}
					if(userInput_express == 0x002F){
						answer_temp = (double)pDiv(answer_temp, userInput_float[1]);
					}
					userInput_floatList[userInput_floatList_Counter] = answer_temp;
					userInput_floatList_Counter++;
					loopSwitch_continMul = FALSE;
					userInput_float[0] = userInput_float[1];
				}

				if(nextExpress == 0x000A){
					if(userInput_express == 0x002A){
						answer_temp = (double)pMul(answer_temp, userInput_float[1]);
					}
					if(userInput_express == 0x002F){
						answer_temp = (double)pDiv(answer_temp, userInput_float[1]);
					}
					userInput_floatList[userInput_floatList_Counter] = answer_temp;
					userInput_floatList_Counter++;
					goto result;
				}
			}
		}
		//Different FOUND!!
		if((userInput_express == 0x002A || userInput_express == 0x002F) && (nextExpress == 0x002B || nextExpress == 0x002D)){
			if(userInput_express == 0x002A){
				answer_temp = (double)pMul(userInput_float[0], userInput_float[1]);
				userInput_floatList[userInput_floatList_Counter] = answer_temp;
				userInput_floatList_Counter++;

			}
			if(userInput_express == 0x002F){
				answer_temp = (double)pDiv(userInput_float[0], userInput_float[1]);
				userInput_floatList[userInput_floatList_Counter] = answer_temp;
				userInput_floatList_Counter++;

			}
		}
		if((userInput_express == 0x002B || userInput_express == 0x002D) && (nextExpress == 0x002A || nextExpress == 0x002F)){
			if(userInput_express == 0x002B){
				answer_temp = (double)pAdd(userInput_float[0], userInput_float[1]);
				userInput_floatList[userInput_floatList_Counter] = answer_temp;
				userInput_floatList_Counter++;

			}
			if(userInput_express == 0x002D){
				answer_temp = (double)pSub(userInput_float[0], userInput_float[1]);
				userInput_floatList[userInput_floatList_Counter] = answer_temp;
				userInput_floatList_Counter++;

			}
		}

		userInput_float[0] = userInput_float[1];
	}

result:
	answer_temp = 0;
	do{
		answer_temp = answer_temp + userInput_floatList[userInput_floatList_Counter];
	}while(userInput_floatList_Counter--);
	printf("%lf!!!!", answer_temp);


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
