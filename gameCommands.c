#include"helpingfunc.h"
#include"gameCommands.h"
#include"parser.h"
#include"initstruct.h"
#include"gurobifunc.h"
#include<stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
 * gameCommands.c module includes all the main functions,algorithms and commands executions needed to solve and edit the sudoku puzzle
 * for every command there exists a function in this module that executes it 
 */
extern int mode;
extern int m;
extern int n;
extern int erroneousBoard;


/* calculates the number of possible solutions of the current board using exhaustive backtracking algorithm
 * if the board is erroneous it prints a proper error message
 * otherwise return the number of possible solutions 
 */
void num_solutions() {
	int i = 0, j = 0, posVal, flag = 0, ret = 0, numSols = 0;
	if (erroneousBoard == 1)
		printf("The board is erroneous\n");
	else {
		stack *st = initStack();
		if (board[i][j]->onBoard == 0) {
			posVal = 1;
			while (posVal <= N) {
				if (cellCheckValidity(0, posVal, i, j)) {
					add(st, posVal);
					board[i][j]->val = posVal;
					ret = 0;
					break;
				}
				++posVal;
			}
		} else {
			add(st, board[i][j]->val);
		}
		incIndexes(&i, &j);
		while (st->head != NULL) {
			flag = 0;
			if (i == N) {
				++numSols;
				ret = 1;
				decIndexes(&i, &j);
			} else if (ret == 0) {
				if (board[i][j]->onBoard == 0) {
					posVal = 1;
					while (posVal <= N) {
						if (cellCheckValidity(0, posVal, i, j)) {
							add(st, posVal);
							board[i][j]->val = posVal;
							ret = 0;
							flag = 1;
							break;
						}
						++posVal;
					}
					if (flag == 0) {
						ret = 1;
						decIndexes(&i, &j);
					} else
						incIndexes(&i, &j);
				} else {
					add(st, board[i][j]->val);
					incIndexes(&i, &j);
				}
			} else {
				if (board[i][j]->onBoard == 0) {
					posVal = st->head->val + 1;
					while (posVal <= N) {
						if (cellCheckValidity(0, posVal, i, j)) {
							st->head->val = posVal;
							board[i][j]->val = posVal;
							flag = 1;
							ret = 0;
							break;
						}
						++posVal;
					}
					if (flag == 0) {
						rem(st, st->head);
						board[i][j]->val = 0;
						ret = 1;
						decIndexes(&i, &j);
					} else
						incIndexes(&i, &j);
				} else {
					rem(st, st->head);
					ret = 1;
					decIndexes(&i, &j);
				}
			}
		}
		freeStack(st);
	}
	cleanBoard();
	printf("number of solutions = %d\n ", numSols);
}




/* set the given value to the given cell
 * if operation cannot be done a proper message containing the reason of failure is printed 
 * otherwise the value is added to the cell, and a board check is executed to see if it's erroneous after the set 
 */
void set(int y, int x, int given) {
	if (mode == 1 && board[x - 1][y - 1]->fixed == 1)
		printf("Error, cell is fixed\n");
	else {
		if(given )
		lstAdd(x - 1, y - 1, board[x - 1][y - 1]->val, 1);
		board[x - 1][y - 1]->val = given;
		board[x - 1][y - 1]->onBoard = given;
		undoHeadFlag = 0;
		lstAdd(x - 1, y - 1, board[x - 1][y - 1]->val, 1);
		checkErrors();
	}
	printBoard();
	checkSolved();
}



/* Saves the current game board to the specified file, where a full or relative path to the file is provided
 * if the path doesn't exist then it creates one and saves the board and it's dimensions to it
 * if the board saved succefully a proper message will be printed
 */
void save(char *filename) {
	int i, j;
	if (mode == 2 && erroneousBoard == 1)
		printf("Error, can not save an erroneous board in edit mode\n");
	else if (mode == 2 && validate(0) == 0)
		printf("Error, can not save an unsolvable board in edit mode\n");
	else {
		FILE *ifp = fopen(filename, "w+");
		fprintf(ifp, "%d %d", m, n);
		for (i = 0; i < N; i++) {
			fputc('\n', ifp);
			for (j = 0; j < N - 1; j++) {
				fputc(board[i][j]->val + '0', ifp);
				if ((board[i][j]->fixed == 1 || mode == 2)
						&& board[i][j]->val != 0)
					fputc('.', ifp);
				fputc(' ', ifp);
			}
			fputc(board[i][j]->val + '0', ifp);
			if ((board[i][j]->fixed == 1 || mode == 2) && board[i][j]->val != 0)
				fputc('.', ifp);
		}
		printf("board saved successfully!\n");
		fclose(ifp);
	}
}




/*it receives a file, loads it by calling loadB() function so it could be solved 
 * sets the mode to solve mode, and prints the board after the loading is done 
 */
void solve(char* file) {
	if (loadB(file)) {
		mode = 1;
		printBoard();
		checkSolved();
	}
}



/*it receives a file to be edited, if no file was provided a 9x9 sudoku board is initialized
 * if a file is provided the function loads it by calling loadB() function
 * sets the mode to edit mode, and prints out the board 
 */
void edit(char* file) {
	if (file == NULL) {
		if (mode != 0)
			freeData();
		n = 3;
		m = 3;
		initGame();
		erroneousBoard = 0;
		printBoard();
		mode = 2;
	} else {
		if (loadB(file)) {
			mode = 2;
			printBoard();
		}
	}
}




/*it checks if the board is erroneous, if so then print proper message 
 * otherwise it validates the current board using ILP, and prints a message saying it's solvable or not
 */
int validate(int print) {
	if (erroneousBoard == 1) {
		if (print)
			printf("command cannot be executed because board is erroneous.\n");
		return 0;
	} else {
		double *arr = (double *) calloc(N * N * N, sizeof(double));
		int i;
		i = ILPfunc(GRB_BINARY, arr);
		free(arr);
		if (i) {
			if (print)
				printf("board has been validated and it's solvable!\n");
			return 1;
		} else {
			if (print)
				printf("validation failed, board is unsolvable!\n");
			return 0;
		}
	}

}

/* it checks if the board is erroneous, if so then print proper message 
 * if it's not erroneous, it guesses a solution to the current board using LP, with given threshold X, 
 * and fills the board with valid values according to their scores
 * for a value to be set in any cell it has to be valid and have the highest score of other values valid for the same cell 
 */ 
void guess(float x) {
	int i, j, k, s, valid;
	float max;
	double *arr = (double *) calloc(N * N * N, sizeof(double));
	if (erroneousBoard == 1) {
		printf("command cannot be executed because board is erroneous.\n");
		free(arr);
		return;
	}
	s = ILPfunc(GRB_CONTINUOUS, arr);
	if (s) {
		saveMove();
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				if (board[i][j]->val == 0) {
					max = arr[i * N * N + j * N];
					for (k = 0; k < N; ++k) {
						valid = cellCheckValidity(0, k + 1, i, j);
						if ((arr[i * N * N + j * N + k] >= x)
								&& (arr[i * N * N + j * N + k] >= max)
								&& valid) {
							board[i][j]->val = k + 1;
							board[i][j]->onBoard = 1;
							max = arr[i * N * N + j * N + k];
						}
					}
				}
				continue;
			}
		}
		saveMove();
	}
	free(arr);
	checkSolved();
}

/*it undos a move previously done by the user(if exists),checks errors and prints the board when done
 * if there are no operations to undo, the operation is not executed and proper message is printed
 */
int undo(int print) {
	if (history->head->val == -1 || history->head->prev->val == -1) {
		if (print)
			printf("no operation to undo\n");
		return 0;
	} else {
		moveHead(1, print);
	}
	return 1;
}

/*it redos a move previously undone by the user(if exists), checks errors and prints the board when done
 * if there are no operations to redo, the operation is not executed and proper message is printed
 */
void redo() {
	if (history->head->next == NULL)
		printf("no operation to redo\n");
	else {
		moveHead(0, 1);
	}
}

/*it receives cell coordinates, checks if the cell is empty and board is not erroneous
 * and runs ILP to find a solution to the current board
 * if a solutions is found then the solution of the given cell is printed as a hint to the user
 * other wise an error message is printed saying the board is not solvable
 */
void hint(int y, int x) {
	double *arr = (double *) calloc(N * N * N, sizeof(double));
	if (erroneousBoard) {
		printf("Error, can not give a hint on erroneous board\n");
		free(arr);
		return;
	} else if (board[x - 1][y - 1]->fixed) {
		printf("Error, can not give a hint for a fixed cell\n");
		free(arr);
		return;
	} else if (board[x - 1][y - 1]->onBoard) {
		printf("Error, can not give a hint for non-empty cell\n");
		free(arr);
		return;
	} else {
		int i, k;
		i = ILPfunc(GRB_BINARY, arr);
		if (i) {
			for (k = 0; k < N; ++k) {
				if (arr[N * N * (x - 1) + N * (y - 1) + k] == 1) {
					printf("possible solution = %d\n", k + 1);
					free(arr);
					return;
				}
			}
		} else {
			printf("error,board unsolvable!\n");
			free(arr);
			return;
		}
	}
}

/*it receives cell coordinates, checks if the cell is empty and board is not erroneous
 * and runs ILP to find a solution to the current board
 * if a solutions is found then all legal values of the given cell are printed
 * other wise an error message is printed saying the board is not solvable
 */
void guess_hint(int x, int y) {
	double *arr = (double *) calloc(N * N * N, sizeof(double));
	if (erroneousBoard) {
		printf("Error, can not give a hint on erroneous board\n");
		free(arr);
		return;
	} else if (board[x - 1][y - 1]->fixed) {
		printf("Error, can not give a hint for a fixed cell\n");
		free(arr);
		return;
	} else if (board[x - 1][y - 1]->onBoard) {
		printf("Error, can not give a hint for non-empty cell\n");
		free(arr);
		return;
	} else {
		int i, k, valid;
		i = ILPfunc(GRB_CONTINUOUS, arr);
		if (i) {
			for (k = 0; k < N; k++) {
				valid = cellCheckValidity(0, k + 1, x - 1, y - 1);
				if (arr[N * N * (x - 1) + N * (y - 1) + k] > 0 && valid) {
					printf(
							"possible legal value for cell= %d , with score= %f\n",
							k + 1, arr[N * N * (x - 1) + N * (y - 1) + k]);
				}
			}
			free(arr);
			return;
		} else {
			printf("error,board unsolvable!\n");
			free(arr);
			return;
		}
	}
}

/*it undos all moves, reverting the board to its original loaded state, and prints board when done
 * if there are no operations to undo, the operation is not executed and proper message is printed
 */
void reset() {
	if (undo(0) == 0) {
		printf("no operations to reset\n");
	} else {
		while (undo(0)) {
			continue;
		}
		printBoard();
	}
}

/*it receives two int numbers, checks if they're in the correct range, if so it generates a puzzle
 * by randomly filling X empty cells with legal values and running ILP to find a solution to the current board
 * and then clearing all but Y random cells.
 * if board cannot be generated a proper message is printed
 */
void generate(int x, int y) {
	int cec, i = 0, success = 0;
	double *arr = (double *) calloc(N * N * N, sizeof(double));
	tupple* posCells;
	tupple* chosenCells = malloc(x * sizeof(tupple));
	cec = countEmptyCells();
	posCells = malloc(cec * sizeof(tupple));
	if (cec < x)
		printf("Error, there's no %d empty cells in the board\n", x);
	else if (erroneousBoard)
		printf("Error, can not generate an erroneous board\n");
	else if (y > N * N)
		printf("Error, cells to fill should be less than %d\n", N * N);
	else {
		getEmptyCells(posCells);
		saveMove();
		while (i < 1000 && success == 0) {
			fillChosenCells(x, cec, chosenCells, posCells);
			success = fillBoard(x, chosenCells);
			if (success)
				success = success && ILPfunc(GRB_BINARY, arr);
			i++;
		}
		if (success == 0) {
			cleanBoard();
			deleteMove();
			printf("Error, board can not be generated\n");
		} else {
			setCells(x, chosenCells);
			fillILPVals(arr);
			emptyCells(N * N - y);
			saveMove();
			printBoard();
		}
	}
	free(arr);
	free(posCells);
	free(chosenCells);
	checkSolved();
}

/*checks whether board is erroneous, if it's not then goes over the board and checks the legal values of each empty cell
 * if such a cell has a single legal obvoius value, it fills it with the value
 * when all obvoius cells are filled th board is printed
 */
void autofill() {
	int i, j, v, c = 0;
	tupple* obviousCells = malloc(N * N * sizeof(tupple));
	int* val = malloc(N * N * sizeof(int));
	if (erroneousBoard)
		printf("Error, can not autofill an erroneous board\n");
	else {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (board[i][j]->onBoard == 0) {
					v = getObviousVal(i, j);
					if (v) {
						obviousCells[c].shora = i;
						obviousCells[c].amoda = j;
						val[c] = v;
						c++;
					}
				}
			}
		}
		for (i = 0; i < c; i++) {
			lstAdd(obviousCells[i].shora, obviousCells[i].amoda, 0, c);
		}
		for (i = 0; i < c; i++) {
			board[obviousCells[i].shora][obviousCells[i].amoda]->val = val[i];
			board[obviousCells[i].shora][obviousCells[i].amoda]->onBoard = 1;
			lstAdd(obviousCells[i].shora, obviousCells[i].amoda, val[i], c);
		}
		checkErrors();
		printBoard();
	}
	free(obviousCells);
	free(val);
	checkSolved();
}
 
/*prints out the board structure and values according to its dimensions
 */
void printBoard() {
	int i, j;
	for (i = 0; i < N; i++) {
		if (i % m == 0)
			printDashes(4 * n * m + m + 1);
		for (j = 0; j < N; j++) {
			if (j % n == 0)
				printf("| ");
			else
				printf(" ");
			if (board[i][j]->val == 0)
				printf("   ");
			else {
				printf("%2d", board[i][j]->val);
				if (board[i][j]->fixed == 1 && mode != 2)
					printf(".");
				else if (board[i][j]->err == 1
						&& (markErrors == 1 || mode == 2))
					printf("*");
				else
					printf(" ");
			}
			if (j == N - 1)
				printf("|\n");
		}
	}
	if (i == N) {
		printDashes(4 * n * m + m + 1);
	}
}

/* exists the program and prints a proper exit message */
void exitfunc() {
	printf("Exiting...\n");
	if (mode != 0)
		freeData();
	exit(0);
}

