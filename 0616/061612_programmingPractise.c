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
;*
;* Known Issue       :
;*
;* 1. No function for validating users' input. Will add soon (planed: 21/12/2018)
;* 2. Integrated with command line, so it will not calculate one time only. Will add soon. (planed: 21/12/2018)
;|**********************************************************************;
*/
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define NULL 0
#define NEGATIVE -1
#define SPACEBAR 0x20

int pCOMMAND(void);
int pDEBUG(void);
_Bool pEND(void);
//ASCII: +2Bh -2Dh *2Ah /2Fh

double keepAddSub(float, char, float, char);
double keepMulDiv(float, char, float, char);
double inputFinish(unsigned short int);

_Bool debugSwitch = FALSE;

_Bool inputFinishFlag = FALSE;
char diffSymbol = NULL;

double answerTempArray[64];
float userInputNum[2] = {NULL, NULL};
char userInputSymbol[2]= {NULL, NULL};

int main(void) {
	double answerTemp = NULL;
	unsigned short int answerTempArray_Counter = NULL;
	_Bool loopSwitch = TRUE;

	while(loopSwitch){
		scanf("%f", &userInputNum[1]);
		if(diffSymbol == 0x002D){				//diffSymbol used for set negative integer
			userInputNum[1] *= -1;
		}
		userInputSymbol[1] = getchar();


		if(userInputSymbol[1] == 0x000A){
			//Only have one integer after symbol change
			if(userInputNum[0] == NULL && userInputSymbol[0] == NULL){
				answerTempArray[answerTempArray_Counter] = userInputNum[1];
			}
			//Only have two integer after symbol change
			switch(userInputSymbol[0]){
				case 0x002B: answerTempArray[answerTempArray_Counter] = userInputNum[0] + userInputNum[1]; break;
				case 0x002D: answerTempArray[answerTempArray_Counter] = userInputNum[0] - userInputNum[1]; break;
				case 0x002A: answerTempArray[answerTempArray_Counter] = userInputNum[0] * userInputNum[1]; break;
				case 0x002F: answerTempArray[answerTempArray_Counter] = userInputNum[0] / userInputNum[1];
			}
		answerTempArray_Counter++;
		answerTemp = inputFinish(answerTempArray_Counter);
		loopSwitch = FALSE;
		continue;
		}

		diffSymbol = NULL;
		//Function Call keepAddSub(): Check if it is more than 3 integer ADD or SUB together.
		if((userInputSymbol[0] == 0x002B || userInputSymbol[0] == 0x002D) && (userInputSymbol[1] == 0x002B || userInputSymbol[1] == 0x002D)){
			answerTempArray[answerTempArray_Counter] = keepAddSub(userInputNum[0], userInputSymbol[0], userInputNum[1], userInputSymbol[1]);
			answerTempArray_Counter++;
			if(inputFinishFlag == TRUE){
				answerTemp = inputFinish(answerTempArray_Counter);
				loopSwitch = FALSE;
				continue;
			}
			continue;
		}

		//Function Call keepMulDiv(): Check if it is more than 3 integer MUL or DIV together.
		if((userInputSymbol[0] == 0x002A || userInputSymbol[0] == 0x002F) && (userInputSymbol[1] == 0x002A || userInputSymbol[1] == 0x002F)){
			answerTempArray[answerTempArray_Counter] = keepMulDiv(userInputNum[0], userInputSymbol[0], userInputNum[1], userInputSymbol[1]);
			answerTempArray_Counter++;
			if(inputFinishFlag == TRUE){
				answerTemp = inputFinish(answerTempArray_Counter);
				loopSwitch = FALSE;
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

	printf("%lf", answerTemp);
	pCOMMAND();
	printf("Program terminated normally!");
	return 0;
}

double keepAddSub(float userInputNumA, char userInputSymbolA, float userInputNumB, char userInputSymbolB){
	double answerTemp = NULL;
	_Bool loopSwitch = TRUE;
	inputFinishFlag = FALSE;

	switch(userInputSymbolA){
	case 0x002B: answerTemp = userInputNumA + userInputNumB; break;
	case 0x002D: answerTemp = userInputNumA - userInputNumB;
	}

	while(loopSwitch){
		userInputNumA = userInputNumB;
		userInputSymbolA = userInputSymbolB;
		scanf("%f", &userInputNumB);
		userInputSymbolB = getchar();

		//If enter detected during add, it will add the final 'userInputNumB' to answerTemp than storage it to array. Note that it a bit different to continMulDiv()
		if(userInputSymbolB == 0x000A){
			switch(userInputSymbolA){
				case 0x002B:
					answerTemp = answerTemp + userInputNumB;
					inputFinishFlag = TRUE;
					return answerTemp;
				case 0x002D:
					answerTemp = answerTemp - userInputNumB;
					inputFinishFlag = TRUE;
					return answerTemp;
			}
		}

		//Check if next symbol not + or -, it will add answerTemp to array. diffSymbol show what is the different symbol. (E.G 5*5-1 diffSymbol = '-')
		if(userInputSymbolB == 0x002A || userInputSymbolB == 0x002F){
			switch(userInputSymbolA){
				case 0x002B:
					diffSymbol = userInputSymbolA;
					userInputSymbol[0] = userInputSymbolB;
					userInputNum[0] = userInputNumB;
					break;
				case 0x002D:
					diffSymbol = userInputSymbolA;
					userInputSymbol[0] = userInputSymbolB;
					userInputNum[0] = userInputNumB;
			}

			loopSwitch = FALSE;
			return answerTemp;
		}else{
			//Continue Add or Sub
			switch(userInputSymbolA){
			case 0x002B: answerTemp = answerTemp + userInputNumB;break;
			case 0x002D: answerTemp = answerTemp - userInputNumB;
			}
		}
	}
	return 0;
}

double keepMulDiv(float userInputNumA, char userInputSymbolA, float userInputNumB, char userInputSymbolB){
	double answerTemp = NULL;
	_Bool loopSwitch = TRUE;
	inputFinishFlag = FALSE;

	switch(userInputSymbolA){
	case 0x002A: answerTemp = userInputNumA * userInputNumB; break;
	case 0x002F: answerTemp = userInputNumA / userInputNumB;
	}

	while(loopSwitch){
		userInputNumA = userInputNumB;
		userInputSymbolA = userInputSymbolB;

		scanf("%f", &userInputNumB);
		userInputSymbolB = getchar();

		if(userInputSymbolB == 0x000A){
			switch(userInputSymbolA){
			case 0x002A:
				answerTemp = answerTemp * userInputNumB;
				inputFinishFlag = TRUE;
				return answerTemp;
			case 0x002F:
				answerTemp = answerTemp / userInputNumB;
				inputFinishFlag = TRUE;
				return answerTemp;
			}
		}

		if(userInputSymbolB == 0x002B || userInputSymbolB == 0x002D){
			switch(userInputSymbolA){
				case 0x002A:
					answerTemp = answerTemp * userInputNumB;
					diffSymbol = userInputSymbolB;
					userInputSymbol[0] = NULL;
					userInputNum[0] = NULL;
					break;
				case 0x002F:
					answerTemp = answerTemp / userInputNumB;
					diffSymbol = userInputSymbolB;
					userInputSymbol[0] = NULL;
					userInputNum[0] = NULL;
			}
			loopSwitch = FALSE;
			return answerTemp;
		}else{
			switch(userInputSymbolA){
			case 0x002A: answerTemp = answerTemp * userInputNumB; break;
			case 0x002F: answerTemp = answerTemp / userInputNumB;
			}
		}
	}
	return 0;
}

double inputFinish(unsigned short int arrayCounter){
	double finalAnswer = NULL;
	do{
		finalAnswer = finalAnswer + answerTempArray[arrayCounter];
	}while(arrayCounter--);
	return finalAnswer;
}

int pCOMMAND(void){
	const unsigned short int arrayNum = 1;
	char userCommand[arrayNum];
	_Bool loopSwitch = TRUE;

	printf("\nEnter q to exit\n");
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
