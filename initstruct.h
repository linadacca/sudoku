/*
 * initstruct.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Lina
 */
#include<stdio.h>

#ifndef INITSTRUCT_H_
#define INITSTRUCT_H_

/*
 * this class holds all the structures that is used in the project, and all of their functions
 */

int n, m,mode;
#define N n*m

int markErrors,erroneousBoard;

typedef struct cell {
	int fixed;
	int onBoard;
	int err;
	int val;
} cell;

typedef struct node {
	int val;
	struct node *next;
	struct node *prev;
} node;

typedef struct stack {
	node *head;
} stack;

typedef struct lstNode {
	int x;
	int y;
	int val;
	int counter;
	struct lstNode *next;
	struct lstNode *prev;
} lstNode;


typedef struct linkedList {
	lstNode *head;
} linkedList;

typedef struct tupple {
	int shora;
	int amoda;
} tupple;

cell *** board ;
cell* initCell();
stack* initStack();
linkedList * history ;

void initList();
void initBoard();
void initGame();
void lstRem(lstNode* old);
void lstAdd(int shora, int amoda, int value, int c);
void add(stack* stc, int a);
void rem(stack* stc, node* old);
void freeData();
void freeList();
void freeStack(stack* stc);
void freeBoard();
void freeData();


#endif /* INITSTRUCT_H_ */
