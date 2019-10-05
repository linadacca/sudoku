/*
 * gamef.h
 *
 *  Created on: Sep 17, 2019
 *      Author: Abdallah && Lina
 */
#ifndef HELPINGFUNC_H_
#define HELPINGFUNC_H_
#include"initstruct.h"

/*
 * In this class we have mini functions that are used by the "gameCommands" functions and is used to reduce code coppying and
 * for comfort
 */

void printDashes(int num);
void decIndexes(int *i, int *j);
void incIndexes(int *i, int *j);
void cleanBoard();
int isnumber(char *s);
void saveMove();
void deleteMove();
int countEmptyCells();
void getEmptyCells(tupple* arr);
void swap(tupple* arr, int i, int j);
void fillChosenCells(int x, int index, tupple* arr1, tupple* arr2);
int fillPosVals(int clean, int x, int y, int arr[]);
int fillBoard(int x, tupple* arr1);
void emptyCells(int del);
void fillILPVals(double *arr);
int getObviousVal(int x, int y);
void setCells(int x, tupple* arr1);
void moveHead(int back, int print);
void checkSolved();
int cellCheckValidity(int caller, int given, int x, int y);
void checkErrors();
int checkFileValidity(char *filename);
int loadB(char *filename);


#endif
