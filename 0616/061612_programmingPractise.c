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
;* 19122018    lst97       5      Totally rewrote, program now working properly in add, sub, mul, div. I will add more function later soon, see Known Issue
;* 20122018    lst97       6      pCOMMAND() rewrote, delete string.h for performance improve, compact code
;* 21122018    lst97       7      inputValidator add
;* 21122018    lst97       8      Integrated with command line
;*
;* Known Issue       :
;*
;* 1. User input '1+1a, 1+1+1a' e.t.c, ERROR message will not show up immediately, but function still works.
;|**********************************************************************;
*/
#include <stdio.h>

#define TRUE 0x01
#define FALSE 0x00
#define NULL 0
#define NEGATIVE -1
#define SPACEBAR 0x0020

extern int CALC(void);

int inputValida(unsigned short int, char, _Bool*);
void resetDecla(double*, unsigned short int*, _Bool*, _Bool*, _Bool*, char*, float*, char*, double*);

double keepAddSub(float*, char*, float, char, _Bool*, char*, _Bool*);
double keepMulDiv(float*, char*, float, char, _Bool*, char*, _Bool*);
double inputFinish(double*, unsigned short int);

extern _Bool debugSwitch;
//ASCII: +2Bh -2Dh *2Ah /2Fh
int CALC(void) {
	printf("Loading CALC()...");
	double answerTemp = NULL;
	unsigned short int answerTempArray_Counter = NULL;
	_Bool loopSwitch = TRUE;

	_Bool runFlag = TRUE;
	_Bool inputFinishFlag = FALSE;
	_Bool inputValidaFlag = TRUE;
	char diffSymbol = NULL;

	float userInputNum[2] = {NULL, NULL};
	char userInputSymbol[2]= {NULL, NULL};
	double answerTempArray[64];

	printf("\nSUCCESS!\n");
	while(runFlag){
		char userCommand;
		printf("[CALC] Press q to exit CALC(), ENTER to activate calculator.\n");
		scanf("%c", &userCommand);
		if(userCommand == 0x000A){
			if(userCommand != 0x000A)while(getchar() != 0x000A);
			loopSwitch = TRUE;
			resetDecla(&answerTemp, &answerTempArray_Counter, &loopSwitch, &inputFinishFlag, &inputValidaFlag, &diffSymbol, userInputNum, userInputSymbol, answerTempArray);
			printf("[CALC] Please enter number for calculate:\n");
			while(loopSwitch){
				//scanf("%1f", &userInputNum[1]);
				inputValida(scanf("%f", &userInputNum[1]), userInputSymbol[1], &inputValidaFlag);
				if(inputValidaFlag == FALSE){resetDecla(&answerTemp, &answerTempArray_Counter, &loopSwitch, &inputFinishFlag, &inputValidaFlag, &diffSymbol, userInputNum, userInputSymbol, answerTempArray);continue;}
				if(diffSymbol == 0x002D) userInputNum[1] *= NEGATIVE;		//diffSymbol used for set negative integer
				userInputSymbol[1] = getchar();

				if(userInputSymbol[1] == 0x000A){
					//Only have one integer after symbol change
					if(userInputNum[0] == NULL && userInputSymbol[0] == NULL)answerTempArray[answerTempArray_Counter] = userInputNum[1];

					//Only have two integer after symbol change
					switch(userInputSymbol[0]){
						case 0x002B: answerTempArray[answerTempArray_Counter] = userInputNum[0] + userInputNum[1]; break;
						case 0x002D: answerTempArray[answerTempArray_Counter] = userInputNum[0] - userInputNum[1]; break;
						case 0x002A: answerTempArray[answerTempArray_Counter] = userInputNum[0] * userInputNum[1]; break;
						case 0x002F: answerTempArray[answerTempArray_Counter] = userInputNum[0] / userInputNum[1];
					}
				answerTempArray_Counter++;
				answerTemp = inputFinish(answerTempArray, answerTempArray_Counter);
				loopSwitch = FALSE;
				continue;
				}

				diffSymbol = NULL;
				//Function Call keepAddSub(): Check if it is more than 3 integer ADD or SUB together.
				if((userInputSymbol[0] == 0x002B || userInputSymbol[0] == 0x002D) && (userInputSymbol[1] == 0x002B || userInputSymbol[1] == 0x002D)){
					answerTempArray[answerTempArray_Counter] = keepAddSub(&userInputNum[0], &userInputSymbol[0], userInputNum[1], userInputSymbol[1], &inputFinishFlag, &diffSymbol, &inputValidaFlag);
					answerTempArray_Counter++;
					if(inputFinishFlag == TRUE){
						answerTemp = inputFinish(answerTempArray, answerTempArray_Counter);
						loopSwitch = FALSE;
						continue;
					}
					if(inputValidaFlag == FALSE){
						resetDecla(&answerTemp, &answerTempArray_Counter, &loopSwitch, &inputFinishFlag, &inputValidaFlag, &diffSymbol, userInputNum, userInputSymbol, answerTempArray);
						continue;
					}
					continue;
				}

				//Function Call keepMulDiv(): Check if it is more than 3 integer MUL or DIV together.
				if((userInputSymbol[0] == 0x002A || userInputSymbol[0] == 0x002F) && (userInputSymbol[1] == 0x002A || userInputSymbol[1] == 0x002F)){
					answerTempArray[answerTempArray_Counter] = keepMulDiv(&userInputNum[0], &userInputSymbol[0], userInputNum[1], userInputSymbol[1], &inputFinishFlag, &diffSymbol, &inputValidaFlag);
					answerTempArray_Counter++;
					if(inputFinishFlag == TRUE){
						answerTemp = inputFinish(answerTempArray, answerTempArray_Counter);
						loopSwitch = FALSE;
						continue;
					}
					if(inputValidaFlag == FALSE){
						resetDecla(&answerTemp, &answerTempArray_Counter, &loopSwitch, &inputFinishFlag, &inputValidaFlag, &diffSymbol, userInputNum, userInputSymbol, answerTempArray);
						continue;
					}
					continue;
				}

				//Check if it is more than last symbol different with the next symbol. (E.G 1*2+1 or 1+2*1)
				if((userInputSymbol[0] == 0x002A || userInputSymbol[0] == 0x002F) && (userInputSymbol[1] == 0x002B || userInputSymbol[1] == 0x002D)){
					switch(userInputSymbol[0]){
					case 0x002A: answerTempArray[answerTempArray_Counter] = userInputNum[0] * userInputNum[1]; break;
					case 0x002F: answerTempArray[answerTempArray_Counter] = userInputNum[0] / userInputNum[1];
					}
					answerTempArray_Counter++;
					diffSymbol = userInputSymbol[1];
					userInputSymbol[0] = NULL; userInputNum[0] = NULL;
					continue;
				}
				if((userInputSymbol[0] == 0x002B || userInputSymbol[0] == 0x002D) && (userInputSymbol[1] == 0x002A || userInputSymbol[1] == 0x002F)){
					answerTempArray[answerTempArray_Counter] = userInputNum[0];
					answerTempArray_Counter++;
					diffSymbol = userInputSymbol[0];
					userInputSymbol[0] = userInputSymbol[1]; userInputNum[0] = userInputNum[1];
					continue;
				}

				//Will run to here if only got 1 integer and symbol.
				userInputSymbol[0] = userInputSymbol[1]; userInputNum[0] = userInputNum[1];
			}

			printf("= %lf\n", answerTemp);
		}else if(userCommand == 0x0071){
			runFlag = FALSE;
		}else{
			printf("[CALC] COMMAND NOT FOUND!\n");
			while(getchar() != 0x000A);
		}
	}
	printf("EXIT CALC()...\nSUCCESS!\n");
	return 0;
}

double keepAddSub(float *userInputNumA, char *userInputSymbolA, float userInputNumB, char userInputSymbolB, _Bool *inputFinishFlag, char *diffSymbol, _Bool *inputValidaFlag){
	double answerTemp = NULL;
	_Bool loopSwitch = TRUE;
	*inputFinishFlag = FALSE;

	switch(*userInputSymbolA){
	case 0x002B: answerTemp = *userInputNumA + userInputNumB; break;
	case 0x002D: answerTemp = *userInputNumA - userInputNumB;
	}

	while(loopSwitch){
		*userInputNumA = userInputNumB;
		*userInputSymbolA = userInputSymbolB;
		//Call Function inputValida();
		inputValida(scanf("%f", &userInputNumB), userInputSymbolB, &inputValidaFlag);
		userInputSymbolB = getchar();
		if(*inputValidaFlag == FALSE){loopSwitch = 0;continue;}

		//If enter detected during add, it will add the final 'userInputNumB' to answerTemp than storage it to array. Note that it a bit different to continMulDiv()
		if(userInputSymbolB == 0x000A){
			switch(*userInputSymbolA){
				case 0x002B: answerTemp = answerTemp + userInputNumB; break;
				case 0x002D: answerTemp = answerTemp - userInputNumB;
			}
			*inputFinishFlag = TRUE;
			return answerTemp;
		}

		//Check if next symbol not + or -, it will add answerTemp to array. diffSymbol show what is the different symbol. (E.G 5*5-1 diffSymbol = '-')
		if(userInputSymbolB == 0x002A || userInputSymbolB == 0x002F){
			*diffSymbol = *userInputSymbolA;
			*userInputSymbolA = userInputSymbolB;
			*userInputNumA = userInputNumB;
			loopSwitch = FALSE;
			return answerTemp;
		}else{
			//Continue Add or Sub
			switch(*userInputSymbolA){
			case 0x002B: answerTemp = answerTemp + userInputNumB;break;
			case 0x002D: answerTemp = answerTemp - userInputNumB;
			}
		}
	}
	return 0;
}

double keepMulDiv(float *userInputNumA, char *userInputSymbolA, float userInputNumB, char userInputSymbolB, _Bool *inputFinishFlag, char *diffSymbol, _Bool *inputValidaFlag){
	double answerTemp = NULL;
	_Bool loopSwitch = TRUE;
	*inputFinishFlag = FALSE;

	switch(*userInputSymbolA){
	case 0x002A: answerTemp = *userInputNumA * userInputNumB; break;
	case 0x002F: answerTemp = *userInputNumA / userInputNumB;
	}
	while(loopSwitch){
		*userInputNumA = userInputNumB;
		*userInputSymbolA = userInputSymbolB;
		inputValida(scanf("%f", &userInputNumB), userInputSymbolB, &inputValidaFlag);
		userInputSymbolB = getchar();
		if(*inputValidaFlag == FALSE){loopSwitch = 0;continue;}

		if(userInputSymbolB == 0x000A){
			switch(*userInputSymbolA){
			case 0x002A:answerTemp = answerTemp * userInputNumB; break;
			case 0x002F:answerTemp = answerTemp / userInputNumB;
			}
			*inputFinishFlag = TRUE;
			return answerTemp;
		}
		if(userInputSymbolB == 0x002B || userInputSymbolB == 0x002D){
			switch(*userInputSymbolA){
				case 0x002A: answerTemp = answerTemp * userInputNumB; break;
				case 0x002F: answerTemp = answerTemp / userInputNumB;
			}
			*diffSymbol = userInputSymbolB;
			*userInputSymbolA = NULL;
			*userInputNumA = NULL;
			loopSwitch = FALSE;
			return answerTemp;
		}else{
			switch(*userInputSymbolA){
			case 0x002A: answerTemp = answerTemp * userInputNumB; break;
			case 0x002F: answerTemp = answerTemp / userInputNumB;
			}
		}
	}
	return 0;
}

double inputFinish(double *answerTempArray, unsigned short int arrayCounter){
	double finalAnswer = NULL;
	do finalAnswer = finalAnswer + *(answerTempArray + arrayCounter); while(arrayCounter--);
	return finalAnswer;
}

int inputValida(unsigned short int userInputNum, char userInputSymbol, _Bool *inputValidaFlag){
	if(userInputNum == NULL){
		printf("[CALC] Syntax ERROR (0x0001)\n");
		if(debugSwitch == TRUE)printf("0x0001: Input should be an integer\n");
		*inputValidaFlag = FALSE;
		while(getchar() != 0x000A);
		return 0;
	}else{
		switch(userInputSymbol){
			case NULL: break; case 0x000A: break; case 0x002B: break; case 0x002D: break; case 0x002A: break; case 0x002F: break;
			default:
				printf("[CALC] Syntax ERROR (0x0002)\n");
				if(debugSwitch == TRUE)printf("0x0002: Symbol should be '+', '-', '*', '/'\n");
				*inputValidaFlag = FALSE;
				while(getchar() != 0x000A);
				return 0;
		}
	}
	*inputValidaFlag = TRUE;
	return 0;
}

void resetDecla(double *A, unsigned short int *B, _Bool *C, _Bool *D, _Bool *E, char *F, float *G, char *H, double *I){
	*A = NULL;*B = NULL;*C = TRUE;*D = FALSE;*E = TRUE;*F = NULL;
	for(unsigned short int counter = 0; counter <=1; counter ++){*(G + counter) = NULL;*(H + counter) = NULL;}
	for(unsigned short int counter = 0; counter <=63; counter ++){*(I + counter) = NULL;}
}
