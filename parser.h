/*
 * parserf.h
 *
 *  Created on: Sep 17, 2019
 *      Author: Lina
 */

#ifndef PARSER_H_
#define PARSER_H_

void getCommand();
/*
 * getCommand() receives a command from user using fgets(), allocates it in memory
 * with a pointer called "command", checks if the command is valid and executes
 * accordingly, at the end the function frees "command" from memory.
 */


int validSolveCommand(char command[]);
/*
 * checks whether the command is a valid set command by dividing it into the main
 * tokens a valid set command must include, and checks their range and values.
 * if command is valid then a call to 'set' function is made, else print proper error message.
 */


int validEditCommand(char command[]);
/*
 * checks whether the command is a valid edit command by dividing it into the main
 * tokens a valid edit command must include.and checks number of parameters entered.
 * if command is valid then a call to 'edit' function is made.
 */


int validMarkErrorsCommand(char command[]);
/*
 * checks whether the command is a valid mark_errors command by dividing it into the main
 * tokens a valid mark_errors command must include, and checks the range of the parameter provided.
 * if command is valid then the value of mark_errors is updated.
 */


int validPrintBoardCommand(char command[]);
/*
 * checks whether the command is a valid print_board command by dividing it into the main
 * tokens a valid print_board command must include.
 * if command is valid then a call to 'printBoard()' function is made.
 */


int validSetCommand(char command[]);
/*
 * checks whether the command is a valid set command by dividing it into the main
 * tokens a valid set command must include, and checks their range and values and current mode.
 * if command is valid then a call to 'set' function is made, else print proper error message.
 */


int validValidateCommand(char command[]);
/*
 * checks whether the command is a valid validate command by dividing it into the main
 * tokens a valid validate command must include. and checks current mode
 * if command is valid then a call to 'validate' function is made.
 */


int validGuessCommand(char command[]);
/*
 * checks whether the command is a valid guess x command by dividing it into the main
 * tokens a valid guess command must include, and checks the range and values of provided parameters and current mode.
 * if command is valid then a call to 'guess(x)' function is made, else print proper error message.
 */


int validGenerateCommand(char command[]);
/*
 * checks whether the command is a valid generate x y command by dividing it into the main
 * tokens a valid generate command must include.and checks number of parameters entered, their range,and the current mode.
 * if command is valid then a call to 'generate(x,y)' function is made.
 */
int validUndoCommand(char command[]);
/*
 * checks whether the command is a valid undo command by dividing it into the main
 * tokens a valid undo command must include.and checks number of parameters entered and the current mode.
 * if command is valid then a call to 'undo(1)' function is made.
 */


int validRedoCommand(char command[]);
/*
 * checks whether the command is a valid redo command by dividing it into the main
 * tokens a valid redo command must include.and checks number of parameters entered and the current mode.
 * if command is valid then a call to 'redo()' function is made.
 */


int validSaveCommand(char command[]);
/*
 * checks whether the command is a valid save command by dividing it into the main
 * tokens a valid save command must include.and checks number of parameters entered and the current mode.
 * if command is valid then a call to 'save' function is made.
 */


int validHintCommand(char command[]);
/*
 * checks whether the command is a valid hint command by dividing it into the main
 * tokens a valid hint command must include.and checks their range and values.
 * if command is valid then a call to 'hint' function is made.
 */


int validGuessHintCommand(char command[]);
/*
 * checks whether the command is a valid guess_hint x y command by dividing it into the main
 * tokens a valid guess_hint command must include, and checks the range and values of provided parameters and current mode.
 * if command is valid then a call to 'guess_hint(x,y)' function is made, else print proper error message.
 */


int validNumSolutionsCommand(char command[]);
/*
 * checks whether the command is a valid num_solutions command by dividing it into the main
 * tokens a valid num_solutions command must include, and checks the number of provided parameters and current mode.
 * if command is valid then a call to 'num_solutions()' function is made, else print proper error message.
 */


int validAutofillCommand(char command[]);
/*
 * checks whether the command is a valid autofill command by dividing it into the main
 * tokens a valid autofill command must include.and checks number of parameters entered and the current mode.
 * if command is valid then a call to 'autofill()' function is made.
 */

int validResetCommand(char command[]);
/*
 * checks whether the command is a valid reset command by dividing it into the main
 * tokens a valid reset command must include.
 * if command is valid then a call to 'reset' function is made.
 */


int validExitCommand(char command[]);
/*
 * checks whether the command is a valid exit command by dividing it into the main
 * tokens a valid exit command must include.
 * if command is valid then a call to 'exitfun' function is made in getCommand().
 */


int is_empty(const char *s);
/*
 * This function will return 0 if any character is not whitespace
 * (i.e. line is not empty), or 1 otherwise.
 * This is to check if a command consists of white only spaces/ new lines / tabs
 * or a combination of them!
 */


#endif
