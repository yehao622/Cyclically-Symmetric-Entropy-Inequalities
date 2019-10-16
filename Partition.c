#include<math.h>
#include<stdio.h>
#include "Calculate.h"
#pragma warning(disable : 4013)

int factorial(int fac) {
	int result = 1;
	for (int i = 1; i <= fac; i++) {
		result = result*i;
	}
	return result;
}

int power(int pow) {
	int result = 1;
	for (int i = 1; i <= pow; i++) {
		result = 2 * result;
	}
	return result;
}

int **comb(int **mi, int *start, int *end, int row, int n, int num_zi) {
	while (start[0] < end[0]) {
		int check_pos_eq_one = 0;
		for (int i = 0; i < num_zi; i++) {
			if (start[i] == end[i]) {
				check_pos_eq_one = i;
				break;
			}
		}

		if (check_pos_eq_one == 0) {
			row = row + 1;
			start[num_zi - 1] = start[num_zi - 1] + 1;
			for (int j = 0; j < num_zi; j++) {
				mi[row][j] = start[j];
			}
		}
		else {
			start[check_pos_eq_one - 1] = start[check_pos_eq_one - 1] + 1;
			for (int j = check_pos_eq_one; j < num_zi; j++) {
				start[j] = start[check_pos_eq_one - 1] + (j - check_pos_eq_one + 1);
			}
			row = row + 1;
			for (int j = 0; j < num_zi; j++) {
				mi[row][j] = start[j];
			}
		}

	}	
	return mi;
}

int **Partition(int n, int k, int r) {
	
	int num_zi = k + r;
	int poss_zi = factorial(n) / (factorial(num_zi)*factorial(n - num_zi));// Calculate total number of possible combination C(n,k+r)
	int **mi = (int**)malloc(poss_zi*sizeof(int*));
	for (int i = 0; i < poss_zi; i++) {
		mi[i] = (int*)malloc(num_zi*sizeof(int));
	}
	/*for (int i = 0; i < poss_zi; i++) {
		for (int j = 0; j < num_zi; j++) {
			mi[i][j] = 0;
		}
	}*/
	int row = 0;
	int pos = num_zi - 1;
	int *start = (int*)malloc(num_zi*sizeof(int));
	int *end   = (int*)malloc(num_zi*sizeof(int));
	for (int j = 0; j < num_zi; j++) {
		start[j] = j + 1;
		mi[0][j] = j + 1;
		end[j] = n - num_zi + 1 + j;
		mi[poss_zi - 1][j]= n - num_zi + 1 + j;
	}

	if (num_zi == 1) {
		for (int j = 0; j < poss_zi; j++) {
			mi[j][pos] = j + 1;
		}
	}
	else {
		mi = comb(mi, start, end, row, n, num_zi);
	}

	free(start);
	start = NULL;
	free(end);
	end = NULL;
	return mi;
}

void exch(int *pos_vec, int i, int j)
{
	int tmp = pos_vec[i];
	pos_vec[i] = pos_vec[j];
	pos_vec[j] = tmp;
}

double temp_ratio = 0.0;
double perm_cal(int *pos_vec, int beg, int end, int *trans, int **leader, int sum_mi, int **orb_col_mem, int orb_col, int **orb_row_mem, int orb_row, int k, int r, int n) {
	if (beg == end-1) {	
		//temp_ratio = 0.0;
		double coef_col = 1 / (double)(leader[orb_col][1] * leader[orb_col][2]);
		double coef_row = 1 / (double)(leader[orb_row][1] * leader[orb_row][2]);
		int *temp_trans = (int*)malloc(n*sizeof(int));
		for (int k = 0; k < n; k++) {
			temp_trans[k] = trans[k];
		}
		int row_perm_i = 0;
		int sum_row = 0, sum_col = 0;
		for (int j = 0; j < n; j++) {
			if (temp_trans[j] != 0) {
				int pos_noze = pos_vec[row_perm_i];
				temp_trans[j] = trans[pos_noze - 1];
				row_perm_i = row_perm_i + 1;
			}
		}

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

		double temp_result = (double)sum_col*coef_col / (sum_row*coef_row);

		if (temp_result > temp_ratio) {
			temp_ratio = temp_result;
		}
	
		free(temp_trans);
		temp_trans = NULL;

	}
	else {
		for (int i = beg; i < end; i++) {
			exch(pos_vec, beg, i);
			perm_cal(pos_vec, beg + 1, end, trans, leader, sum_mi, orb_col_mem, orb_col, orb_row_mem, orb_row, k, r, n);
			exch(pos_vec, beg, i);
		}
	}
	return temp_ratio;
}

