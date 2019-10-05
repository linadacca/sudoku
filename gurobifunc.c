#include "gurobi_c.h"
#include "gurobifunc.h"
#include <stdlib.h>
#include <stdio.h>
#include "helpingfunc.h"
#include"initstruct.h"
/*
 * gurobifunc.c module includes all the functions needed to initialize the gurobi environment, set constraints that match
 * the current board, sets the objective function and tries to find an optimal solution for the function
 * this module finds and returns a solution to the board using LP and ILP , where both are implemented in ILPFunc function
 * the ILP uses (max 0) as an objective function, whilest LP generates random coefficient from 1-N for each of the N^3 variables
 * and the objective function is (max sum of all coefficient)
 */
extern int m;
extern int n;

void freeOV(double* obj, char* vtype);/* frees two array obj[] and vtype[]*/
int forCell(GRBmodel *model, GRBenv *env);/* set a constraint for each cell to get one value only*/
int forRow(GRBmodel *model, GRBenv *env);/* set a constraint for each value to appear once in each row*/
int forCol(GRBmodel *model, GRBenv *env);/* set a constraint for each value to appear once in each column*/
int forBlock(GRBmodel *model, GRBenv *env);/* set a constraint for each value to appear once in each block*/
int existingVal(GRBmodel *model, GRBenv *env);/* set a constraint for each value that's already in the board*/
int solveModel(GRBmodel *model, GRBenv *env, double objval, double* arr);/*receives all constraints and objective function and finds optimal solution*/

void freeOV(double* obj, char* vtype) {
	free(obj);
	free(vtype);
}

int ILPfunc(char var_type, double* arr) {
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int error = 0, i, j;
	double objval = 0.0;
	double *obj = (double*) calloc(N * N * N, sizeof(double));
	char *vtype = (char*) calloc(N * N * N, sizeof(char));

	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintparam(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}

	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}

	for (i = 0; i < N * N * N; i++) {
		vtype[i] = var_type;
	}
	for (j = 0; j < N * N * N; j++) {
		if (var_type != GRB_CONTINUOUS) {
			obj[j] = 0;
		} else {
			obj[j] = (double) (rand() % N) + 1;
		}
	}

	error = GRBaddvars(model, N * N * N, 0, NULL, NULL, NULL, obj, NULL, NULL,
			vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}

	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}
	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		freeOV(obj, vtype);
		return 0;
	}

	if (!existingVal(model, env) || !forCell(model, env) || !forRow(model, env)
			|| !forCol(model, env) || !forBlock(model, env)
			|| !solveModel(model, env, objval, arr)) {
		freeOV(obj, vtype);
		return 0;
	}
	GRBfreemodel(model);
	GRBfreeenv(env);
	freeOV(obj, vtype);
	return 1;
}

int forCell(GRBmodel *model, GRBenv *env) {
	int i, j, k, error = 0;
	int *ind = (int*) calloc(N, sizeof(int)); /* which var appear in constraint */
	double *val = (double *) calloc(N, sizeof(double)); /* coeff of var in constraint */

	/* Each cell gets a value */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind[k] = i * N * N + j * N + k;
				val[k] = 1.0;
			}

			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d in 1st GRBaddconstr() : %s\n", error,
						GRBgeterrormsg(env));
				free(val);
				free(ind);
				return 0;
			}
		}
	}
	free(val);
	free(ind);
	return 1;

}

int forRow(GRBmodel *model, GRBenv *env) {
	int i, j, k, error = 0;
	int *ind = (int*) calloc(N, sizeof(int)); /* which var appear in constraint */
	double *val = (double *) calloc(N, sizeof(double)); /* coeff of var in constraint */

	/* Each value must appear once in each row */

	for (k = 0; k < N; k++) {
		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				ind[i] = i * N * N + j * N + k;
				val[i] = 1.0;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d in 2nd GRBaddconstr() : %s\n", error,
						GRBgeterrormsg(env));
				free(val);
				free(ind);
				return 0;
			}
		}

	}

	free(val);
	free(ind);
	return 1;

}

int forCol(GRBmodel *model, GRBenv *env) {
	int i, j, k, error = 0;
	int *ind = (int*) calloc(N, sizeof(int)); /* which var appear in constraint */
	double *val = (double *) calloc(N, sizeof(double)); /* coeff of var in constraint */

	/* Each value must appear once in each column */

	for (k = 0; k < N; k++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				ind[j] = i * N * N + j * N + k;
				val[j] = 1.0;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d in 3rd GRBaddconstr() : %s\n", error,
						GRBgeterrormsg(env));
				free(val);
				free(ind);
				return 0;
			}
		}
	}

	free(val);
	free(ind);
	return 1;

}

int forBlock(GRBmodel *model, GRBenv *env) {
	int error = 0, a, b, c, d, i, j, k;
	int *ind = (int*) calloc(N, sizeof(int));
	double *val = (double *) calloc(N, sizeof(double));
	/* Each value must appear once in each block */
	for (k = 0; k < N; k++) {
		for (c = 1; c <= m; c++) {
			for (d = 1; d <= n; d++) {
				b = 0;
				for (j = 0; j < m; j++) {
					for (i = 0; i < n; i++) {
						a = (i * N + j * N * N + (c - 1) * N * n
								+ (d - 1) * N * N * m) + k;
						ind[b] = a;
						val[b] = 1;
						b++;
					}
				}
				error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d in 4th GRBaddconstr(): %s\n", error,
							GRBgeterrormsg(env));
					free(val);
					free(ind);
					return 0;
				}
			}
		}
	}
	free(val);
	free(ind);
	return 1;
}

/* Sets 1 for every value that's already in the model*/
int existingVal(GRBmodel *model, GRBenv *env) {
	int error = 0, i, j, k;
	int *ind = (int*) calloc(1, sizeof(int));
	double *val = (double *) calloc(1, sizeof(double));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				if (board[i][j]->val == 1 + k) {
					ind[0] = i * N * N + j * N + k;
					val[0] = 1;
					error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 1.0,
							NULL);
					if (error) {
						printf("ERROR %d in 5th GRBaddconstr(): %s\n", error,
								GRBgeterrormsg(env));
						free(val);
						free(ind);
						return 0;
					}
				}

			}
		}
	}

	free(val);
	free(ind);
	return 1;
}

int solveModel(GRBmodel *model, GRBenv *env, double objval, double* arr) {
	int error = 0;
	int optimstatus;
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	error = GRBwrite(model, "mip1.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/*checking if problem is solvable*/
	if (optimstatus == GRB_INF_OR_UNBD) {
		return 0;
	} else if (optimstatus != GRB_OPTIMAL) {
		return 0;
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
		printf("objval=%f\n", objval);
		printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, arr);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error,
				GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}
