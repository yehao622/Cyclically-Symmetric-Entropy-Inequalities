c#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "Partition.h"
#include "Select_par.h"
#include "Leader.h"
#include "Convertion.h"
#include "Calculate.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4013)

int main() {
	printf("********************************************************************\n");
	printf("* This program runs lower bound based on MDS code with repetition. *\n");
	printf("********************************************************************\n");
	printf("Please type in an integer no less than 3: ");
	int n; // n -- random variable
	scanf("%d", &n);
	int col_num = power(n) - 1;   // Initialize number of entropy variable 2^n-1

	int *lead_map, **leader;
	lead_map = LeaderList(n, col_num); // mapping each number with its leader
	int orbit_num = OrbitNum(lead_map, col_num); // Number of orbit
	leader = Leader(lead_map, orbit_num, col_num); // Leader contains Oi and li
	for (int i = 0; i < orbit_num; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d\t", leader[i][j]);
		}
		printf("\n");
	}
	printf("Total number of orbits is %d\n", orbit_num);

	// Begin to load lower-bound and upper-bound from txt file
	char *fname_lb, *fname_ub;
	FILE *pfile_lb, *pfile_ub;
	char fname_lb_1[] = { "lb_" }, fname_ub_1[] = { "ub_" }, fname_post[] = { ".txt" };

	double **c_matrix_lb = (double**)malloc(orbit_num*sizeof(double*));
	double **c_matrix_ub = (double**)malloc(orbit_num*sizeof(double*));
	for (int j = 0; j < orbit_num; j++) {
		c_matrix_lb[j] = (double*)malloc(orbit_num * sizeof(double));
		c_matrix_ub[j] = (double*)malloc(orbit_num * sizeof(double));
	}
	char n_char[2] = { "00" };
	itoa(n, n_char, 10);
	if (n <= 9) {
		fname_lb = (char*)malloc(8 * sizeof(char));
		fname_ub = (char*)malloc(8 * sizeof(char));
		snprintf(fname_lb, 9, "%s%s%s", fname_lb_1, n_char, fname_post);
		snprintf(fname_ub, 9, "%s%s%s", fname_ub_1, n_char, fname_post);
	}
	else {
		fname_lb = (char*)malloc(9 * sizeof(char));
		fname_ub = (char*)malloc(9 * sizeof(char));
		snprintf(fname_lb, 10, "%s%s%s", fname_lb_1, n_char, fname_post);
		snprintf(fname_ub, 10, "%s%s%s", fname_ub_1, n_char, fname_post);
	}
	pfile_lb = fopen(fname_lb, "r");
	if (pfile_lb == NULL) {
		printf("Wrong openning lb!\n");
	}
	pfile_ub = fopen(fname_ub, "r");
	if (pfile_ub == NULL) {
		printf("Wrong openning ub!\n");
	}
	int count_diff = 0;
	for (int i = 0; i < orbit_num; i++) {
		for (int j = 0; j < orbit_num; j++) {
			fscanf(pfile_lb, "%lf", &c_matrix_lb[i][j]);
			fscanf(pfile_ub, "%lf", &c_matrix_ub[i][j]);
			if (c_matrix_ub[i][j] - c_matrix_lb[i][j] > 0.0001)
				count_diff += 1;
		}
	}
	printf("Number of different pair of orbits is %d\n", count_diff);
	system("pause");
	fclose(pfile_lb);
	fclose(pfile_ub);

	FILE *record_brkpint, *comp_matrix;

	for (int orb_col = 3; orb_col < orbit_num; orb_col++) {	
		int **orb_col_mem = decTobin(lead_map, orb_col, leader, n, col_num);		

		for (int orb_row = 25; orb_row < orbit_num; orb_row++) {	
			comp_matrix = fopen("Computing.txt", "a+");

			if (c_matrix_ub[orb_row][orb_col] - c_matrix_lb[orb_row][orb_col] <= 0.0001) {
				fprintf(comp_matrix, "%.4f\t", c_matrix_lb[orb_col][orb_row]);
				fclose(comp_matrix);
				continue;
			}

			record_brkpint = fopen("beakpoint.txt", "a+");
			fprintf(record_brkpint, "Computing on column index %d to row index %d\n", orb_col + 1, orb_row + 1);
			fclose(record_brkpint);
			int **orb_row_mem = decTobin(lead_map, orb_row, leader, n, col_num);
			double ratio = c_matrix_lb[orb_row][orb_col];

			printf("running on ratio of orbit %d to orbit %d\n", orb_col + 1, orb_row + 1);			

			for (int k = 1; k <= n; k++) {
				for (int r = 0; r <= n - k; r++) {
					
					int **table, **valid_mi;
					int poss = factorial(n) / (factorial(k + r)*factorial(n - k - r)); // number of combination
					table = Partition(n, k, r); // Generate all possible combination

					int num_mi;
					if (k + r == 1) {
						valid_mi = table;
						num_mi = n;
					}
					else {
						valid_mi = valid(table, poss, k + r);
						num_mi = Count(table, poss, k + r);						
						free(table);
					}				
						
					for (int r_val_mi = 0; r_val_mi < num_mi; r_val_mi++) {

						int sum_mi = 0;
						for (int c_val_mi = 0; c_val_mi < k + r; c_val_mi++) {
							sum_mi = sum_mi + valid_mi[r_val_mi][c_val_mi];
						}
						
						for (int r_lead = 0; r_lead < orbit_num; r_lead++) {
							
							if (leader[r_lead][2] == sum_mi) {		
								int *trans = CovToBin(leader[r_lead][0], n, valid_mi[r_val_mi], k+r, sum_mi);								
								int *pos_vec = PosInd(trans, n, sum_mi);
								int beg = 0;

								double temp_ratio = perm_cal(pos_vec, beg, sum_mi, trans, leader, sum_mi, orb_col_mem, orb_col, orb_row_mem, orb_row, k, r, n);

								free(pos_vec);
								pos_vec = NULL;

								if (temp_ratio > ratio) {
									ratio = temp_ratio;
								}
								temp_ratio = 0.0;
								free(trans);
								trans = NULL;

							}

						}

					}
						free(valid_mi);
						valid_mi = NULL;
						table = NULL;

				}
			}

			c_matrix_lb[orb_row][orb_col] = ratio;
			fprintf(comp_matrix, "%.4f\t", c_matrix_lb[orb_col][orb_row]);
			fclose(comp_matrix);
			free(orb_row_mem);
			orb_row_mem = NULL;

		}
		comp_matrix = fopen("Computing.txt", "a+");
		fprintf(comp_matrix, "\n");
		fclose(comp_matrix);
		free(orb_col_mem);
		orb_col_mem = NULL;
	}
	free(lead_map);
	lead_map = NULL;
	free(leader);
	leader = NULL;

	FILE *pfile;
	pfile = fopen("Output.txt", "w");
	for (int i = 0; i < orbit_num; i++) {
		for (int j = 0; j < orbit_num; j++) {
			fprintf(pfile, "%.4f\t", c_matrix_lb[i][j]);
		}
		fprintf(pfile, "\n");
	}
	fclose(pfile);
	free(c_matrix_lb);
	c_matrix_lb = NULL;
	free(c_matrix_ub);
	c_matrix_ub = NULL;
	printf("Calculation Done!\n");

	system("pause");
}
