#include<stdio.h>
#include "Partition.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4013)

int difcount(int *temp_colorb, int n) {
	int count = 0;
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j<n;j++){
			if (temp_colorb[i] > temp_colorb[j]) {
				exch(temp_colorb, i, j);
			}
		}
	}

	if (temp_colorb[0] != 0) {
		count = count + 1;
	}
	for (int i = 0; i < n - 1; i++) {
		if ( temp_colorb[i] != temp_colorb[i + 1]) {
			count = count + 1;
		}
	}

	return count;
}

double Calc(int *trans, int **perm_table, int **leader, int sum_mi, int **orb_col_mem, int orb_col, int **orb_row_mem, int orb_row, int k, int r, int n, double ratio) {
	double result = ratio;
	double coef_col = 1 / (double)(leader[orb_col][1] * leader[orb_col][2]);
	double coef_row = 1 / (double)(leader[orb_row][1] * leader[orb_row][2]);
	int *temp_trans = (int*)malloc(n*sizeof(int));
	for (int k = 0; k < n; k++) {
		temp_trans[k] = trans[k];
	}

	for (int i = 0; i < factorial(sum_mi); i++) {
		
		int row_perm_i = 0;
		int sum_row = 0, sum_col = 0;		

		for (int j = 0; j < n; j++) {
			if (temp_trans[j] != 0) {
				int pos_noze = perm_table[i][row_perm_i];
				temp_trans[j] = trans[ pos_noze - 1];
				row_perm_i = row_perm_i + 1;
			}
		}
		free(perm_table[i]);

		for (int kcol = 0; kcol < leader[orb_col][1]; kcol++) {
			int *temp_colorb = (int*)malloc(n*sizeof(int));
			for (int k = 0; k < n; k++) {
				temp_colorb[k] = trans[k];
			}
			for (int k = 0; k < n; k++) {
				temp_colorb[k] = temp_trans[k] * orb_col_mem[kcol][k];
			}
			int difnum = difcount(temp_colorb, n);
			
			if (difnum < k) {
				sum_col = sum_col + difnum;
			}
			else {
				sum_col = sum_col + k;
			}
			free(temp_colorb);
			temp_colorb = NULL;
		}

		for (int krow = 0; krow < leader[orb_row][1]; krow++) {
			int *temp_roworb = (int*)malloc(n*sizeof(int));
			for (int k = 0; k < n; k++) {
				temp_roworb[k] = trans[k];
			}

			for (int k = 0; k < n; k++) {
				temp_roworb[k] = temp_trans[k] * orb_row_mem[krow][k];
			}
			int difnum = difcount(temp_roworb, n);
			if (difnum < k) {
				sum_row = sum_row + difnum;
			}
			else {
				sum_row = sum_row + k;
			}
			free(temp_roworb);
			temp_roworb = NULL;
		}

		double temp_result = (double) sum_col*coef_col / (sum_row*coef_row);

		if (temp_result > result) {
			result = temp_result;
		}
	}

	free(temp_trans);
	temp_trans = NULL;
	return result;	
}