#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include"parser.h"
#include"helpingfunc.h"
#include"gameCommands.h"
#define initMode 0
#define solveMode 1
#define editMode 2
#define M 256 /*max size of a single command*/
extern int n;
extern int m;
extern int mode;
extern int markErrors;

/*
 * parser module receives the commands from the user, interprets them, checks
 * whether they are valid or not and sends them to the appropriate functions
 * existing in other modules, if command isn't valid returns a proper message.
 */

void getCommand() {
	char *command = malloc(M);
	printf("Enter command:\n");
	if (fgets(command, M, stdin) != NULL) {
		char *p;
		if ((p = strchr(command, '\n'))) {/*check exist newline */
			*p = 0;
		} else {
			if (scanf("%*[^\n]")) {
			}
			if (scanf("%*c")) {
			}/*clear upto newline*/
		}
	} else {
		exitfunc(1);
	}

	if (is_empty(command)) { /*input was an "empty" command*/
		free(command);
		getCommand();
		return;
	}

	if (validExitCommand(command)) {
	}

	else if (validSolveCommand(command)) {

	} else if (validEditCommand(command)) {
	}

	else if (validMarkErrorsCommand(command)) {
	}

	else if (validPrintBoardCommand(command)) {
	}

	else if (validSetCommand(command)) {
	}

	else if (validValidateCommand(command)) {
	}

	else if (validGuessCommand(command)) {
	}

	else if (validGenerateCommand(command)) {
	}

	else if (validUndoCommand(command)) {
	}

	else if (validRedoCommand(command)) {
	}

	else if (validSaveCommand(command)) {
	}

	else if (validHintCommand(command)) {
	}

	else if (validGuessHintCommand(command)) {
	}

	else if (validNumSolutionsCommand(command)) {
	}

	else if (validAutofillCommand(command)) {
	}

	else if (validResetCommand(command)) {
	} else {
		printf("Error: invalid command, no such command exists.\n");
	}

	free(command);

	getCommand();

}

int validSolveCommand(char command[]) {
	char *token1, *token2, *token3;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);

	if (strcmp(token1, "solve") == 0) {
		if (token2 == '\0') {
			printf("Error: not enough parameters were entered!\n"
					"please add a path to the desired file.\n");
			return 1;
		}
		if (token3 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			solve(token2);
			return 1;
		}
	}
	return 0;

}

int validEditCommand(char command[]) {
	char *token1, *token2, *token3;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);

	if (strcmp(token1, "edit") == 0) {
		if (token3 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			edit(token2);
			return 1;
		}
	}
	return 0;

}

int validMarkErrorsCommand(char command[]) {
	char *token1, *token2, *token3;
	int k;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);

	if (strcmp(token1, "mark_errors") == 0) {
		if (mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode.\n");
			return 1;
		}
		if (token2 == '\0') { /* if no parameter was entered */
			printf("Error: not enough parameters were entered!\n"
					"please enter the mark_errors value={0,1}.\n");
			return 1;
		}
		if (strcmp(token2, "0") != 0 && strcmp(token2, "1") != 0) {
			printf(
					"Error: the value of mark_errors should be either 0 or 1.\n");
			return 1;
		}
		if (token3 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			k = (int) strtol(token2, (char **) NULL, 10); /* turn string to int */
			markErrors = k;
			return 1;
		}
	}
	return 0;

}

int validPrintBoardCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "print_board") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			printBoard();
			return 1;
		}
	}
	return 0;

}

int validSetCommand(char command[]) {
	char *token1, *token2, *token3, *token4, *token5;
	int k = 0, j, given, x, y;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);
	token4 = strtok(NULL, del);
	token5 = strtok(NULL, del);

	if (strcmp(token1, "set") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 == '\0' || token3 == '\0' || token4 == '\0'
				|| token5 != '\0') {
			printf("Error: you need to enter 3 integer values.\n");
			return 1;
		}

		while (k < (int) strlen(token2)) { /* check if token2 contains legal digits 0-9 and in correct range*/
			if (isdigit(token2[k]) == 0) {
				printf("Error: first value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token2, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: first value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		k = 0;
		while (k < (int) strlen(token3)) { /* check if token3 contains legal digits 0-9 and in correct range*/
			if (isdigit(token3[k]) == 0) {
				printf(
						"Error: second value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token3, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: second value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		k = 0;
		while (k < (int) strlen(token4)) { /* check if token4 contains legal digits 0-9 and in correct range*/
			if (isdigit(token4[k]) == 0) {
				printf("Error: third value is not in the correct range 0-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token4, (char**) NULL, 10);
				if (j < 0 || j > m * n) {
					printf(
							"Error: third value is not in the correct range 0-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		given = atoi(token4);
		y = atoi(token3);
		x = atoi(token2);
		set(x, y, given);
		return 1;
	}
	return 0;

}

int validValidateCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "validate") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			validate(1);
			return 1;
		}
	}
	return 0;
}

int validGuessCommand(char command[]) {
	char *token1, *token2, *token3;
	float k;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);

	if (strcmp(token1, "guess") == 0) {
		if (mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode.\n");
			return 1;
		}
		if (token3 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			k = atof(token2);
			if (k < 0 || k > 1) {
				printf("Error: parameter must be in range 0-1.\n");
				return 1;
			}
			guess(k);
			return 1;
		}
	}
	return 0;
}

int validGenerateCommand(char command[]) {
	char *token1, *token2, *token3, *token4;
	int x, y;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);
	token4 = strtok(NULL, del);

	if (strcmp(token1, "generate") == 0) {
		if (mode != editMode) {
			printf("Error: invalid command in current mode, "
					"available only in edit mode.\n");
			return 1;
		}
		if (token2 == '\0' || token3 == '\0' || token4 != '\0') {
			printf("Error: 2 integer numbers must be entered.\n");
			return 1;
		}
		y = atoi(token3);
		x = atoi(token2);
		generate(x, y);
		return 1;
	}
	return 0;
}

int validUndoCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "undo") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			undo(1);
			return 1;
		}
	}
	return 0;
}

int validRedoCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "redo") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			redo();
			return 1;
		}
	}
	return 0;
}

int validSaveCommand(char command[]) {
	char *token1, *token2, *token3;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);

	if (strcmp(token1, "save") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 == '\0') { /* if no parameter was entered */
			printf(
					"Error: you must enter the path to the file you wish to save the puzzle to.\n");
			return 1;
		}
		if (token3 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			save(token2);
			return 1;
		}
	}
	return 0;

}

int validHintCommand(char command[]) {
	char *token1, *token2, *token3, *token4;
	int k = 0, j, x, y;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);

	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);
	token4 = strtok(NULL, del);

	if (strcmp(token1, "hint") == 0) {
		if (mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode.\n");
			return 1;
		}
		if (token2 == '\0' || token3 == '\0' || token4 != '\0') {
			printf("Error: 2 integer numbers must be entered.\n");
			return 1;
		}
		while (k < (int) strlen(token2)) { /* check if token2 contains legal digits 0-9 and in correct range*/
			if (isdigit(token2[k]) == 0) {
				printf("Error: first value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token2, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: first value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		k = 0;
		while (k < (int) strlen(token3)) { /* check if token3 contains legal digits 0-9 and in correct range*/
			if (isdigit(token3[k]) == 0) {
				printf(
						"Error: second value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token3, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: second value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		x = atoi(token3);
		y = atoi(token2);
		hint(y, x);
		return 1;
	}
	return 0;
}

int validGuessHintCommand(char command[]) {
	char *token1, *token2, *token3, *token4;
	int k = 0, j, x, y;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);

	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);
	token3 = strtok(NULL, del);
	token4 = strtok(NULL, del);

	if (strcmp(token1, "guess_hint") == 0) {
		if (mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode.\n");
			return 1;
		}
		if (token2 == '\0' || token3 == '\0' || token4 != '\0') {
			printf("Error: 2 integer numbers must be entered.\n");
			return 1;
		}
		while (k < (int) strlen(token2)) { /* check if token2 contains legal digits 0-9 and in correct range*/
			if (isdigit(token2[k]) == 0) {
				printf("Error: first value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token2, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: first value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		k = 0;
		while (k < (int) strlen(token3)) { /* check if token3 contains legal digits 0-9 and in correct range*/
			if (isdigit(token3[k]) == 0) {
				printf(
						"Error: second value is not in the correct range 1-%d.\n",
						m * n);
				return 1;
			} else {
				j = (int) strtol(token3, (char**) NULL, 10);
				if (j < 1 || j > m * n) {
					printf(
							"Error: second value is not in the correct range 1-%d.\n",
							m * n);
					return 1;
				}
			}
			k++;
		}
		x = atoi(token3);
		y = atoi(token2);
		guess_hint(x, y);
		return 1;
	}
	return 0;
}

int validNumSolutionsCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "num_solutions") == 0) {
		if (mode != editMode && mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			num_solutions();
			return 1;
		}
	}
	return 0;
}

int validAutofillCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "autofill") == 0) {
		if (mode != solveMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			autofill();
			return 1;
		}
	}
	return 0;
}

int validResetCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "reset") == 0) {
		if (mode == initMode) {
			printf("Error: invalid command in current mode, "
					"available only in solve mode and edit mode.\n");
			return 1;
		}
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			reset();
			return 1;
		}
	}
	return 0;
}

int validExitCommand(char command[]) {
	char *token1, *token2;
	const char del[8] = " \t\r\n\v\f";
	char commandTmp[M];
	strcpy(commandTmp, command);
	token1 = strtok(commandTmp, del); /* get the first token */
	token2 = strtok(NULL, del);

	if (strcmp(token1, "exit") == 0) {
		if (token2 != '\0') {
			printf("Error: you entered too many parameters!\n");
			return 1;
		} else {
			free(command);
			exitfunc(1);
			return 0;
		}
	}
	return 0;
}

int is_empty(const char *s) {
	while (*s != '\0') {
		if (!isspace((unsigned char) *s))
			return 0;
		s++;
	}
	return 1;
}

