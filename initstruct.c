#include<stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include"helpingfunc.h"
#include"parser.h"
#include"gameCommands.h"
#include"initstruct.h"

extern cell *** board;
extern linkedList * history;

/*
 * initializes the cell of the puzzle
 */
cell* initCell() {
	cell* c = malloc(sizeof(cell));
	c->fixed = 0;
	c->onBoard = 0;
	c->err = 0;
	c->val = 0;
	return c;
}

/*
 * initializes the cell of the stack that is used for the backtrack algorithims (num_solutions();)
 */
stack* initStack() {
	stack* stc = (stack*) malloc(sizeof(stack));
	stc->head = NULL;
	return stc;
}

/*
 * initializes the undo/redo list
 */
void initList() {
	lstNode* start = (lstNode*) malloc(sizeof(lstNode));
	start->next = NULL;
	start->prev = NULL;
	history = (linkedList*) malloc(sizeof(linkedList));
	start->val = -1;
	history->head = start;
}

/*
 * initializes the board that represents the game/puzzle and used to play
 */
void initBoard() {
	int i, j;
	board = malloc(N * sizeof(cell *));
	for (i = 0; i < N; i++) {
		board[i] = malloc(N * sizeof(cell));
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			board[i][j] = initCell();
		}
	}
}

/*
 * initializes the game by calling above initialization functions
 */
void initGame() {
	initBoard();
	initList();
}

/*
 * given a stack and a value "a", adds "a" to the head of the stack
 */
void add(stack* stc, int a) {
	node* new = (node*) malloc(sizeof(node));
	new->val = a;
	new->next = NULL;
	new->prev = stc->head;
	stc->head = new;
}

/*
 * given a stack and a value old, removes old from the head of the stack
 */
void rem(stack* stc, node* old) {
	stc->head = old->prev;
	if (stc->head != NULL)
		stc->head->next = NULL;
	free(old);
}

/*
 * given a listNode old, removes old from the undo/redo list
 */
void lstRem(lstNode* old) {
	if (old->prev != NULL)
		old->prev->next = old->next;
	if (old->next != NULL) {
		old->next->prev = old->prev;
	}
	if (old == history->head)
		history->head = NULL;
	free(old);
}

/*
 * given the parameters of a cell, ads a list node that holds these parameters to the undo/redo list
 */
void lstAdd(int shora, int amoda, int value, int c) {
	lstNode* new = (lstNode*) malloc(sizeof(lstNode));
	new->x = shora;
	new->y = amoda;
	new->val = value;
	new->counter = c;
	new->next = NULL;
	if (history->head != NULL) {
		while (history->head->next != NULL) {
			lstRem(history->head->next);
		}
		history->head->next = new;
	}
	new->prev = history->head;
	history->head = new;
}

/*
 * frees the undo/redo list from the memory
 */
void freeList() {
	while (history->head->prev != NULL) {
		history->head = history->head->prev;
	}
	while (history->head->next != NULL) {
		lstRem(history->head->next);
	}
	lstRem(history->head);
	free(history);
}

/*
 * frees a given stack from the memory
 */
void freeStack(stack* stc) {
	while (stc->head != NULL) {
		rem(stc, stc->head);
	}
	free(stc);
}

/*
 * frees the data that represents the playing board from the memory
 */
void freeBoard() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			free(board[i][j]);
		}
		free(board[i]);
	}
	free(board);
}

/*
 * frees data by calling above functions
 */
void freeData() {
	freeList();
	freeBoard();
}

