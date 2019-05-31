#include <stdio.h>
#include <stdlib.h>

// There are some bugs in this program, fix when having free time. (lst97 31052019)

struct StaticLinkedList{
	unsigned int uiSuperScript;
	char cData;
	unsigned int uiSubScript;
}SLL[10];

unsigned int fnMalloc_SLL(struct StaticLinkedList SLL[]);
static fnSLL_Insert(struct StaticLinkedList SLL[], unsigned int, char);
static fnSLL_Create(struct StaticLinkedList SLL[], unsigned int);
static fnSLL_Print(struct StaticLinkedList SLL[], unsigned int);

int main(void) {
	unsigned int uiPosistion = 0;
	char cData;
	fnSLL_Create(SLL, 10);

	printf("Insert index position:\n");
	scanf_s("%d", &uiPosistion);
	printf("Character data:\n");
	while (getchar() != '\n');
	scanf_s("%c", &cData, 1);

	fnSLL_Insert(SLL, uiPosistion, cData);
	fnSLL_Print(SLL, 10);

	system("pause");
	return 0;
}

static fnSLL_Create(struct StaticLinkedList SLL[], unsigned int iLength) {
	if (iLength < 1 || iLength > 10) {
		return -1;
	}
	for (unsigned int iLoop = 0; iLoop < 10; iLoop++) {
		SLL[iLoop].uiSuperScript = iLoop +1;
		SLL[iLoop].uiSubScript = iLoop;
	}
	SLL[9].uiSuperScript = 1;
	return 0;
}

static fnSLL_Print(struct StaticLinkedList SLL[], unsigned int iLength) {
	for (unsigned int iLoop = 0; iLoop < iLength; iLoop++) {
		printf("%d ", SLL[iLoop].uiSuperScript);
	}
	printf("\n");
	for (unsigned int iLoop = 0; iLoop < iLength; iLoop++) {
		printf("%c ", SLL[iLoop].cData);
	}
	printf("\n");
	for (unsigned int iLoop = 0; iLoop < iLength; iLoop++) {
		printf("%d ", SLL[iLoop].uiSubScript);
	}
	printf("\n");
}

static fnSLL_Insert(struct StaticLinkedList SLL[], unsigned int uiPosistion, char cData) {
	unsigned int uiEmptyNode, uiHeadSubScript = 10 -1;

	if (uiPosistion < 1 || uiPosistion > 10) {
		return -1;
	}
	uiEmptyNode = fnMalloc_SLL(SLL);

	if (uiEmptyNode) {
		SLL[uiEmptyNode].cData = cData;
		for (unsigned int iLoop = 1; iLoop <= uiPosistion -1; iLoop++) {		// Go to the uiPosistion pointing.
			uiHeadSubScript = SLL[uiHeadSubScript].uiSuperScript;
		}
		SLL[uiEmptyNode].uiSuperScript = SLL[uiHeadSubScript].uiSuperScript;
		SLL[uiHeadSubScript].uiSuperScript = uiEmptyNode;

		return 0;
	}
	return -1;
}

unsigned int fnMalloc_SLL(struct StaticLinkedList SLL[]) {
	unsigned int uiEmptyNode = SLL[0].uiSuperScript;
	if (SLL[0].uiSuperScript) {
		SLL[0].uiSuperScript = SLL[uiEmptyNode].uiSuperScript;
	}
	return uiEmptyNode;
}
