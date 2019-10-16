#include<stdio.h>
#pragma warning(disable : 4013)

int *LeaderList(int var_num, int col_num) {
	int *leader_map;
	leader_map = (int*)malloc(col_num*sizeof(int));

	for (int i = 0; i < col_num; i++) {
		leader_map[i] = i + 1;
	} // Initialize the leader array

	for (int i = 0; i < col_num; i++) {

		if ((leader_map[i] == i + 1) && (i < (col_num - 1))) {
			int temp = leader_map[i];

			for (int j = 1; j < var_num; j++) {

				temp = temp * 2;
				if (temp < col_num)
					leader_map[temp - 1] = leader_map[i];
				else {
					temp = temp - col_num;
					leader_map[temp - 1] = leader_map[i];
				}

			}

		}//end for if new_leader[i] = i + 1

	}

	return leader_map;
}

int OrbitNum(int *leader_map, int col_num) {
	int orbit_num = 0;
	for (int i = 0; i < col_num; i++) {
		if (leader_map[i] == (i + 1)) {
			orbit_num = orbit_num + 1;
		}
	}
	return orbit_num;
}

int **Leader(int *lead_map, int orbit_num, int col_num) {
	int pos = 0;
	int **leader = (int**)malloc(orbit_num*sizeof(int*));
	for (int j = 0; j < orbit_num; j++) {
		leader[j] = (int*)malloc(3 * sizeof(int));
	}

	// 1st column records leader decimal representation
	// second column records number of members in each orbit (big O)
	for (int i = 0; i < col_num; i++) {

		if (lead_map[i] == (i + 1)) {
			leader[pos][0] = i + 1;			

			int Oi = 0;
			for (int j = 0; j < col_num; j++) {
				if (lead_map[j] == leader[pos][0]) {
					Oi = Oi + 1;
				}
			}
			leader[pos][1] = Oi;

			int li = 0;
			int temp_i = leader[pos][0];
			while ( temp_i>0 ) {
				if (temp_i % 2 == 1) {
					li = li + 1;
				}
				temp_i = (int)temp_i / 2;
			}
			leader[pos][2] = li;

			pos = pos + 1;
		}

	}

	return leader;
}