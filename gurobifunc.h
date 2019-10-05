/*
 * gurobif.h
 *
 *  Created on: Sep 17, 2019
 *      Author: Lina
 */

#ifndef GUROBIFUNC_H_
#define GUROBIFUNC_H_
#include "gurobi_c.h"

int ILPfunc(char var_type, double* arr);/* initializes gurobi env and the objective function and its variables*/

#endif
