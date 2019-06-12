#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct BinaryTreeNode {
	char data;
	struct BinaryTreeNode* leftChild, * rightChild;
}BinaryTreeNode;

void fnCreateBinaryTree(BinaryTreeNode**);
void fnTraverse(BinaryTreeNode*, int);
void fnVisit(char, int);

int main(void) {
	int iLevel = 1;
	BinaryTreeNode *pBinaryTreeNode = NULL;

	fnCreateBinaryTree(&pBinaryTreeNode);
	fnTraverse(pBinaryTreeNode, iLevel);

	system("pause");
	return 0;
}

void fnCreateBinaryTree(BinaryTreeNode** pBinaryTree) {
	char cData;
	scanf_s("%c", &cData, 1);

	if (cData == ' ') *pBinaryTree = NULL;
	else {
		*pBinaryTree = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
		(*pBinaryTree)->data = cData;
		fnCreateBinaryTree(&(*pBinaryTree)->leftChild);
		fnCreateBinaryTree(&(*pBinaryTree)->rightChild);
	}
}

void fnTraverse(BinaryTreeNode* pBinaryTree, int iLevel) {
	if (pBinaryTree) {
		fnVisit(pBinaryTree->data, iLevel);
		fnTraverse(pBinaryTree->leftChild, iLevel + 1);
		fnTraverse(pBinaryTree->rightChild, iLevel + 1);
	}
}

void fnVisit(char cData, int iLevel) {
	printf("%c in level: [%d]\n", cData, iLevel);
}
