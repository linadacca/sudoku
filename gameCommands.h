/*
 * solverf.h
 *
 *  Created on: Sep 17, 2019
 *      Author: Lina
 */

#ifndef GAMECOMMANDS_H_
#define GAMECOMMANDS_H_

int redoHeadFlag;
int undoHeadFlag;

void printBoard();
void exitfunc();
void set(int given, int x, int y);
void save(char *filename);
void edit(char* file);
void solve(char* file);
int validate(int print);
void redo();
int undo(int print);
void guess(float x);
void reset();
void hint(int y,int x);
void guess_hint(int x,int y);
void generate(int x, int y);
void autofill();
void num_solutions();

#endif
