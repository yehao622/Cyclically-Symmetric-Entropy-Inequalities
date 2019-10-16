#include<stdio.h>
#include "Partition.h"
#pragma warning(disable : 4013)

int num_col(int **matrix, int poss, int num_zi) {
	int count = 0;
	for (int row = 0; row < poss; row++) {
		int valid = 1;
		for (int col = 1; col < num_zi; col++) {
			if (matrix[row][col] < matrix[row][col - 1]) {
				valid = 0;
				break;
			}
		}
		if (valid == 1) {
			count = count + 1;
		}
	}
	return count;
}

int Count(int **table, int poss, int num_zi) {
	int count;
	int **copy = (int**)malloc(poss*sizeof(int*));
	for (int i = 0; i < poss; i++) {
		copy[i] = (int*)malloc(num_zi*sizeof(int));
	}
	for (int i = 0; i < poss; i++) {
		for (int j = 0; j < num_zi; j++) {
			copy[i][j] = 0;
		}
	}

	for (int row = 0; row < poss; row++) {
		for (int col = 0; col < num_zi; col++) {
			if (col == 0) {
				copy[row][col] = table[row][col];
			}
			else {
				copy[row][col] = table[row][col] - table[row][col - 1];
			}
		}
	}

	count = num_col(copy, poss, num_zi);
	free(copy);
	copy = NULL;
	return count;	
}

int **valid(int **table, int poss, int num_zi) {
	int count;
	int **copy = (int**)malloc(poss*sizeof(int*));
	for (int i = 0; i < poss; i++) {
		copy[i] = (int*)malloc(num_zi*sizeof(int));
	}
	/*for (int i = 0; i < poss; i++) {
		for (int j = 0; j < num_zi; j++) {
			copy[i][j] = 0;
		}
	}*/

	for (int row = 0; row < poss; row++) {		
		for (int col = 0; col < num_zi; col++) {
			if (col == 0) {
				copy[row][col] = table[row][col];
			}
			else {
				copy[row][col] = table[row][col] - table[row][col - 1];
			}						
		}
	}

	count = num_col(copy, poss, num_zi);

	// Initialize valid mi matrix
	int **valid_mi = (int**)malloc(count*sizeof(int*));
	for (int i = 0; i < count; i++) {
		valid_mi[i] = (int*)malloc(num_zi*sizeof(int));
	}
	/*for (int i = 0; i < count; i++) {
		for (int j = 0; j < num_zi; j++) {
			valid_mi[i][j] = 0;
		}
	}*/

	int *rec_val_ind = (int*)malloc(count*sizeof(int));
	count = 0;
	for (int row = 0; row < poss; row++) {
		int valid = 1;
		for (int col = 1; col < num_zi; col++) {
			if (copy[row][col] < copy[row][col - 1]) {
				valid = 0;
				break;
			}
		}
		if (valid == 1) {
			rec_val_ind[count] = row;
			count = count + 1;
		}
	}

	for (int row = 0; row < count; row++) {
		for (int col = 0; col < num_zi; col++) {
			valid_mi[row][col] = copy[rec_val_ind[row]][col];
			//printf("%d\t", valid_mi[row][col]);
		}
		//printf("\n");
	}
	
	free(rec_val_ind);
	rec_val_ind = NULL;
	free(copy);
	copy = NULL;
	return valid_mi;

}