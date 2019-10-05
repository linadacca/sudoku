#include<stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include"helpingfunc.h"
#include"parser.h"
#include"gameCommands.h"
#include"initstruct.h"

extern int n;
extern int m;
extern int mode;
extern int erroneousBoard;


/*
 * as its name, it prints the dashes of the puzzle, used by printBoard();
 */
void printDashes(int num) {
	int i;
	for (i = 0; i < num; i++) {
		printf("-");
	}
	printf("\n");
}

/*
 * used to calculate indexes for the previous cell and called by num_solutions();
 */
void decIndexes(int *i, int *j) {
	if (*j == 0) {
		*j = N - 1;
		*i -= 1;
	} else
		*j -= 1;
}

/*
 * used to calculate indexes for the next cell and called by num_solutions();
 */
void incIndexes(int *i, int *j) {
	if (*j == N - 1) {
		*j = 0;
		*i += 1;
	} else
		*j += 1;
}

/*
 * clears values that was written on the board by algorithmic functions such as num_solutions(), generate() and many more
 */
void cleanBoard() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j]->onBoard == 0)
				board[i][j]->val = 0;
		}
	}
}

/*
 * as its name, on a given string *s, checks if its a number, used by loadB() -> edit();
 */
int isnumber(char *s) {
	unsigned int i;
	for (i = 0; i < strlen(s); i++) {
		if (!isdigit(s[i]))
			return 0;
	}
	return 1;
}

/*
 * saves the current state of the board to the undo/redo list which is called "history", used by generate() and guess();
 */
void saveMove() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j]->onBoard)
				lstAdd(i, j, board[i][j]->val, N * N);
			else
				lstAdd(i, j, 0, N * N);
		}
	}
}

/*
 * used as a reverse of the above function "saveMove" in case generate() failed
 */
void deleteMove() {
	int i;
	for (i = 0; i < N * N; i++) {
		history->head = history->head->prev;
	}
	lstRem(history->head);
}

/*
 * as its name, counts the number of empty cells in the puzzle, used to check if Generate X Y is possible (as for X),
 * used for generate();
 */
int countEmptyCells() {
	int i, j, count = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j]->onBoard == 0)
				count++;
		}
	}
	return count;
}


/*
 * on a given array arr, coppies the indexes of the emptyCells to arr, used for generate();
 */
void getEmptyCells(tupple* arr) {
	int i, j, c = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j]->onBoard == 0) {
				arr[c].shora = i;
				arr[c].amoda = j;
				c++;
			}
		}
	}
}

/*
 * given an array arr and to indexes, the function swaps the cells between the indexes in the arr, used for generate();
 */
void swap(tupple* arr, int i, int j) {
	int x, y;
	x = arr[j].shora;
	y = arr[j].amoda;
	arr[j].shora = arr[i].shora;
	arr[j].amoda = arr[i].amoda;
	arr[i].shora = x;
	arr[i].amoda = y;
}

/*
 * randomly fills data from arr2 in arr1, for x cells, used for generate();
 */
void fillChosenCells(int x, int index, tupple* arr1, tupple* arr2) {
	int r, i;
	for (i = 0; i < x; i++) {
		r = rand() % index;
		arr1[i].shora = arr2[r].shora;
		arr1[i].amoda = arr2[r].amoda;
		swap(arr2, r, index - 1);
		index--;
	}
}


/*
 * given x, y (column, row) and array arr, the function fills all possible, legal values of cell (x,y) in arr,
 * cleans all cells of arr if clean = 1, used for generate();
 */
int fillPosVals(int clean, int x, int y, int arr[]) {
	int i, c = 0, count = 0;
	if (clean == 1) {
		for (i = 0; i < N; i++) {
			arr[i] = 0;
		}
		return 0;
	} else {
		for (i = 1; i <= N; i++) {
			if (cellCheckValidity(0, i, x, y)) {
				arr[c] = i;
				c++;
				count++;
			}
		}
		return count;
	}
}

/*
 * fills x cells of chosen cells in arr1 by random legal value, used for generate();
 */
int fillBoard(int x, tupple* arr1) {
	int i, r, numVals;
	int *posVals = malloc(N * sizeof(int));
	for (i = 0; i < x; i++) {
		numVals = fillPosVals(0, arr1[i].shora, arr1[i].amoda, posVals);
		if (numVals != 0) {
			r = rand() % numVals;
			board[arr1[i].shora][arr1[i].amoda]->val = posVals[r];
		} else {
			cleanBoard();
			return 0;
		}
	}
	free(posVals);
	return 1;
}

/*
 * given value del, deletes del values from the puzzle, used for generate();
 */
void emptyCells(int del) {
	int i, j, x = 0, r;
	tupple* indexes = malloc(N * N * sizeof(tupple));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			indexes[x].shora = i;
			indexes[x].amoda = j;
			x++;
		}
	}
	x = N * N;
	for (i = 0; i < del; i++) {
		r = rand() % x;
		board[indexes[r].shora][indexes[r].amoda]->val = 0;
		board[indexes[r].shora][indexes[r].amoda]->onBoard = 0;
		swap(indexes, r, x - 1);
		x--;
	}
	free(indexes);
}

/*
 * given arr that represents the solution of the board given from the ILP, the function fills the board with the values in arr
 * used for generate();
 */
void fillILPVals(double *arr) {
	int i, j, k;
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				if (arr[i * N * N + j * N + k] == 1) {
					board[i][j]->val = k + 1;
					board[i][j]->onBoard = 1;
				}

			}
		}
	}
}

/*
 * sets the onBoard field of the puzzle to 1 in the indexes that in arr, used for generate();
 */
void setCells(int x, tupple* arr1) {
	int i;
	for (i = 0; i < x; i++)
		board[arr1[i].shora][arr1[i].amoda]->onBoard = 1;
}

/*
 * returns 1 if cell (x,y) has an obvious value (1 legal value to fill only), used by autofill();
 */
int getObviousVal(int x, int y) {
	int i, v = 0, counter = 0;
	for (i = 1; i <= N; i++) {
		if (cellCheckValidity(0, i, x, y)) {
			counter++;
			v = i;
			if (counter > 1)
				break;
		}
	}
	if (counter == 1)
		return v;
	else
		return 0;
}

/*
 * moves the head of the undo/redo list to the currect possition, perform the operations need to be operated
 * (undo/redo) and prints the board if print = 1, used by undo() and redo();
 */
void moveHead(int back, int print) {
	int i, x, y, v, moves, flag = 0;
	if (back)
		moves = history->head->counter;
	else {
		history->head = history->head->next;
		moves = history->head->counter;
	}
	for (i = 0; i < moves; i++) {
		if (back)
			history->head = history->head->prev;
		else
			history->head = history->head->next;
	}
	for (i = 0; i < moves; i++) {
		flag = 0;
		x = history->head->x;
		y = history->head->y;
		v = history->head->val;
		board[x][y]->val = v;
		board[x][y]->onBoard = v;
		board[x][y]->fixed = board[x][y]->fixed && v;
		if (back)
			history->head = history->head->prev;
		else if (history->head->next != NULL) {
			history->head = history->head->next;
			flag = 1;
		}
	}
	if (back == 0 && flag)
		history->head = history->head->prev;
	checkErrors();
	if (print)
		printBoard();
}

/*
 * checks if the puzzle was solved successfully, used by all the the functions that performs changes in the puzzle
 * such as: set(), autofill(), generate(), guess()..
 */
void checkSolved(){
	int i, j;
	if(mode == 1){
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
				if(board[i][j]->onBoard == 0)
					return;
			}
		}
		if(erroneousBoard)
			printf("The solution is erroneouse\n");
		else{
			printf("Puzzle solved successfully, game is set to init mode\n");
			mode = 0;
			freeData();
		}
	}

}


/*
 * checks if a given value can fit as a legal value in cell x,y, returns 0 if not and 1 otherwise,
 * and updates the err feild for the errored cells if caller = 1
 */
int cellCheckValidity(int caller, int given, int x, int y) {
	int i, j, blockR, blockC;
	board[x][y]->err = 0;
	/*column check*/
	for (i = 0; i < N; i++) {
		if (given == board[x][i]->val && i != y) {
			if (caller == 1) {
				board[x][y]->err = 1;
				board[x][i]->err = 1;
			} else
				return 0;
		}
	}
	/*row check*/
	for (i = 0; i < N; i++) {
		if (given == board[i][y]->val && i != x) {
			if (caller == 1) {
				board[x][y]->err = 1;
				board[i][y]->err = 1;
			} else
				return 0;
		}
	}
	blockR = (int) (x / m) * m;
	blockC = (int) (y / n) * n;
	/*block check*/
	for (i = 0; i < m; i++) { /* shorat ha block */
		for (j = 0; j < n; j++) { /* amodat ha block */
			if (given == board[blockR + i][blockC + j]->val
					&& (blockR + i != x && blockC + j != y)) {
				if (caller == 1) {
					board[x][y]->err = 1;
					board[blockR + i][blockC + j]->err = 1;
				} else
					return 0;
			}
		}
	}
	return 1;
}

/*
 * checks if the board is erroneouse, and checks for every cell if its erroneouse by calling the above function with caller = 1
 */
void checkErrors() {
	int i, j;
	erroneousBoard = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j]->val != 0) {
				cellCheckValidity(1, board[i][j]->val, i, j);
				erroneousBoard = erroneousBoard || board[i][j]->err;
			}
		}
	}
}


/*
 * checks if the given *filename is in the right format, used by edit(), solve();
 */
int checkFileValidity(char *filename) {
	int n = 0, m = 0, total = 0, counter = -2;
	char str[20], str2[20];
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		return 0;
	}
	while (fscanf(fptr, "%s", str) == 1) {
		strncpy(str2, str, strlen(str) + 1);
		str2[strlen(str2) - 1] = 0;
		if (!isnumber(str)
				&& (!isnumber(str2) || str[strlen(str) - 1] != '.')) {
			fclose(fptr);
			return 1;
		}
		counter++;
	}
	fclose(fptr);
	fptr = fopen(filename, "r");
	if (fscanf(fptr, "%d", &m) && fscanf(fptr, "%d", &n)) {
	} else {
		return 0;
	}

	total = n * m * n * m;
	if (total <= 0) {
		fclose(fptr);
		return 0;
	}
	if (counter < total) {
		fclose(fptr);
		return 2;
	} else if (counter > total) {
		fclose(fptr);
		return 3;
	}
	fclose(fptr);
	return 4;
}


/*
 * the functions loads a file *filename and coppies the values in file to the board/puzzle, used by solve(), edit();
 */
int loadB(char *filename) {
	int i, j, m_new, n_new, fileChecker;
	char str[20], str2[20];
	FILE *fptr = fopen(filename, "r");
	fileChecker = checkFileValidity(filename);
	if (!fptr) {
		printf("Error, the file doesn't exist in the specific path.\n");
		return 0;
	} else if (fileChecker == 0) {
		printf("Error, block size should be greater than zero.\n");
		return 0;
	} else if (fileChecker == 1) {
		printf(
				"Error, the file should include numbers and whitespaces only.\n");
		return 0;
	} else if (fileChecker == 2) {
		printf("Error, too few numbers in the file.\n");
		return 0;
	} else if (fileChecker == 3) {
		printf("Error, too many numbers in the file.\n");
		return 0;
	} else {
		if (fscanf(fptr, "%d", &m_new) && fscanf(fptr, "%d", &n_new)) {
		}
		if (mode != 0)
			freeData();
		m = m_new;
		n = n_new;
		initGame();
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (fscanf(fptr, "%s", str)) {
				}
				if (!isnumber(str)) {
					strncpy(str2, str, strlen(str) - 1);
					board[i][j]->val = atoi(str2);
					board[i][j]->fixed = 1;
				} else {
					board[i][j]->val = atoi(str);
				}
				board[i][j]->onBoard = board[i][j]->val;
			}
		}
		checkErrors();
	}
	fclose(fptr);
	return 1;
}
