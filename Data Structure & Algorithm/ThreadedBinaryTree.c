// Reference: https://www.youtube.com/watch?v=qjAfV1XBRpQ&list=PLuRm_QtoJ4J-Ld1BPZ7F2VLfNgfgjpj17&index=48

#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;
// Link(0)	: show as child
// Thread(1): show as Predecessor
typedef enum {Link, Thread} TAG;

typedef struct ThreadBinaryTreeNode {
	char data;
	struct ThreadBinaryTreeNode* leftChild, * rightChild;
	TAG leftTag;
	TAG rightTag;
}ThreadBinaryTreeNode, *ptrThreadBinaryTreeNode;

// Function declaration
void CreateThreadBinaryTree(ptrThreadBinaryTreeNode*);
void InTreading(ThreadBinaryTreeNode*);
void InOrderThreading(ptrThreadBinaryTreeNode*, ptrThreadBinaryTreeNode);
void Traverse(ptrThreadBinaryTreeNode);
void fnVisit(char);

// Golble briable
ptrThreadBinaryTreeNode previousNode;

int main(void) {
	ptrThreadBinaryTreeNode TreeNode, ptrHead = NULL;

	CreateThreadBinaryTree(&TreeNode);
	InOrderThreading(&ptrHead, TreeNode);
	printf("RESULT: ");
	Traverse(ptrHead);

	printf("\n\n");
	system("pause");
	return 0;
}

void CreateThreadBinaryTree(ptrThreadBinaryTreeNode *TreeNode) {

	char cData;
	scanf_s("%c", &cData, 1);

	if (' ' == cData) {
		*TreeNode = NULL;
	}
	else {
		*TreeNode = (ThreadBinaryTreeNode*)malloc(sizeof(ThreadBinaryTreeNode));
		(*TreeNode)->data = cData;
		(*TreeNode)->leftTag = Link;
		(*TreeNode)->rightTag = Link;

		CreateThreadBinaryTree(&(*TreeNode)->leftChild);
		CreateThreadBinaryTree(&(*TreeNode)->rightChild);
	}
}

void InTreading(ThreadBinaryTreeNode *TreeNode) {
	if (TreeNode) {
		InTreading(TreeNode->leftChild);
		if (!(TreeNode->leftChild)) {		// No leftChild
			TreeNode->leftTag = Thread;
			TreeNode->leftChild = previousNode;
		}
		if (!(previousNode->rightChild)) {
			previousNode->rightTag = Thread;
			previousNode->rightChild = TreeNode;
		}
		previousNode = TreeNode;
		InTreading(TreeNode->rightChild);
	}
}

void InOrderThreading(ptrThreadBinaryTreeNode *ptrRoot, ptrThreadBinaryTreeNode TreeNode) {
	*ptrRoot = (ptrThreadBinaryTreeNode)malloc(sizeof(ThreadBinaryTreeNode));
	(*ptrRoot)->leftTag = Link;
	(*ptrRoot)->rightTag = Thread;
	(*ptrRoot)->rightChild = *ptrRoot;
	if (!TreeNode) {
		(*ptrRoot)->leftChild = *ptrRoot;
	}
	else {
		(*ptrRoot)->leftChild = TreeNode;
		previousNode = *ptrRoot;
		InTreading(TreeNode);
		previousNode->rightChild = *ptrRoot;
		previousNode->rightTag = Thread;
		(*ptrRoot)->rightChild = previousNode;
	}
}

void Traverse(ptrThreadBinaryTreeNode TreeNode) {
	ptrThreadBinaryTreeNode ptrTree;
	ptrTree = TreeNode->leftChild;

	while (ptrTree != TreeNode) {
		while (ptrTree->leftTag == Link) {
			ptrTree = ptrTree->leftChild;
		}
		fnVisit(ptrTree->data);

		while (ptrTree->rightTag == Thread && ptrTree->rightChild != TreeNode) {
			ptrTree = ptrTree->rightChild;
			fnVisit(ptrTree->data);
		}
		ptrTree = ptrTree->rightChild;
	}
}

void fnVisit(char cData) {
	printf("%c", cData);
}
