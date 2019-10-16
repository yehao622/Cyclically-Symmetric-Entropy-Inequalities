#include<stdio.h>
#include "Partition.h"
#pragma warning(disable : 4013)

int *CovToBin(int lead, int n, int *valid_mi_onerow, int num_zi, int sum_mi) {
	int temp_lead = lead;
	int *tran = (int*)malloc(n*sizeof(int));
	int *temp_tran = (int*)malloc(sum_mi*sizeof(int));

	for (int i = n - 1; i >= 0; i--) {
		if ((int)(temp_lead / power(i)) > 0) {
			tran[n - 1 - i] = 1;
			temp_lead = temp_lead - power(i);
		}
		else {
			tran[n - 1 - i] = 0;
		}
	}

	int temp_pos = 0;
	for (int i = 0; i < num_zi; i++) {
		int j = 0;
		while (j < valid_mi_onerow[i]) {
			temp_tran[temp_pos] = i + 1;
			temp_pos = temp_pos + 1;
			j = j + 1;
		}
	}

	int pos = 0;
	for (int i = 0; i < n; i++) {
		if (tran[i] == 1) {
			tran[i] = temp_tran[pos];
			pos = pos + 1;
		}
	}
	free(temp_tran);
	temp_tran = NULL;
	return tran;
}

int *PosInd(int *trans, int n, int sum_mi) {
	int *posind = (int*)malloc(sum_mi * sizeof(int));
	int ind = 0;
	for (int i = 0; i < n; i++) {
		if (trans[i] != 0) {
			posind[ind] = i + 1;
			ind = ind + 1;
		}
	}

	return posind;
}

int **decTobin(int *lead_map, int orb_rc, int **leader, int n, int col_num) {
	int num_mem = leader[orb_rc][1];
	int **conv = (int**)malloc(num_mem*sizeof(int*));
	for (int j = 0; j < num_mem; j++) {
		conv[j] = (int*)malloc(n * sizeof(int));
	}

	int row_conv = 0;
	for (int r_lmap = 0; r_lmap < col_num; r_lmap++) {
		if (lead_map[r_lmap] == leader[orb_rc][0]) {
			int temp_ind = r_lmap + 1;
			for (int c_lmap = n - 1; c_lmap >= 0;c_lmap--) {

				if ( (int)(temp_ind / power(c_lmap))>0 ) {
					conv[row_conv][n - 1 - c_lmap] = 1;
					temp_ind = temp_ind - power(c_lmap);
				}
				else {
					conv[row_conv][n - 1 - c_lmap] = 0;
				}

			}
			row_conv = row_conv + 1;
		}
	}
	return conv;
}