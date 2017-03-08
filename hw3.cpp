//
//	Bryant Clouse
//	Grid-World
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <random>
#include <iomanip>
#include <cmath>
#include "Classes.h"

using namespace std;

int main() {
	srand(time(NULL));
	int n = 20;
	int iter = 1000;
	int sr = 30;
	float *info1;//, *info2, *info3;
	FILE *learning, *path; 
	int min;
	char c;
	bool grid_rep = true;
	bool manual;
	int a_size;
	char dir;
	int dummy = 0;

	cout << "Do you want to control the agent (Y/N)? ";
	while (1) {
		cin >> c;
		if (c == 'Y') {
			manual = true;
			break;
		} else if (c == 'N') {
			manual = false;
			break;
		} else {
			cout << "Invalid Input. ";
		}
	}
	if (!manual) {
		cout << "Do you want a grid representation (Y/N)? ";
		while (1) {
			cin >> c;
			if (c == 'Y') {
				grid_rep = true;
				break;
			} else if (c == 'N') {
				grid_rep = false;
				break;
			} else {
				cout << "Invalid Input. ";
			}
		}
	}
	cout << "How large (6-15) do you want the square grid? ";
	cin >> n;
	if (!manual) {
		cout << "How many Statisical Runs (1-30)? ";
		cin >> sr;
	}	

	info1 = new float[iter]();

	min = 2*n-10;

	int size[2] = {n,n};
	int goal[2] = {n-2,n-2};
	if (grid_rep) {
		a_size = size[0]*size[1];	
	} else {
		a_size = 9;
	}
	GridWorld grid(size[0],size[1],goal[0],goal[1]);
	grid.set_representation(grid_rep);
	Agent Dexter(a_size, 0.1, 0.1, 0.9, &grid);
	//Agent Jeff  (size[0]*size[1], 0.01, 0.1, 0.9, &grid);
	// Agent Bobby (size[0]*size[1], 0.0, 0.1, 0.9, &grid);
	if (grid_rep) {
		Dexter.set_state(2*n+2);
		// Jeff.set_state(2*n+2);
		// Bobby.set_state(2*n+2);
	} else {
		Dexter.set_state(0);
		// Jeff.set_state(0);
		// Bobby.set_state(0);
	}
	if (!manual) {
		for (int j = 0; j < sr; j++) {
			for (int i = 0; i < 1000; i++) {
				if (i == 999 && j == 0) {
					path = fopen("Path.txt","w+");
					info1[i] += Dexter.action(path);
					fclose(path);
				} else {
					info1[i] += Dexter.action();
				}
				// info2[i] += Jeff.action();
				// info3[i] += Bobby.action();
				if (grid_rep) {
					Dexter.TestE();
				} else {
					Dexter.TestG();
				}
			}
			Dexter.TestD();
			Dexter.reset();
			// Jeff.reset();
			// Bobby.reset();
		}
		//if (grid_rep) Dexter.TestF(min,info1[999]/sr);

		learning = fopen("Learning.txt", "w+");
		for (int i = 0; i < 1000; i++) {
			//fprintf(learning, "%d\t%.3f\t%.3f\t%.3f\n", i, info1[i]/sr, info2[i]/sr, info3[i]/sr);
			fprintf(learning, "%d\t%.3f\n", i, info1[i]/sr);
		}
		Dexter.display();
		fclose(learning);
	} else {
		while (!grid.found_goal(dummy)) {
			Dexter.display();
			cout << ">> ";
			do {
				cin >> dir;
			} while (dir != 'w' && dir != 'a' && dir != 's' & dir != 'd');
			switch (dir) {
				case 'w': dummy = Dexter.man_move(0); break;
				case 'd': dummy = Dexter.man_move(1); break;
				case 's': dummy = Dexter.man_move(2); break;
				case 'a': dummy = Dexter.man_move(3); break;
			}
		}
		Dexter.display();
		cout << "Found Goal!\n";

	}
	grid.clear();
	Dexter.clear();
	// Jeff.clear();
	// Bobby.clear();
	delete info1;
	// delete info2;
	// delete info3;
	return 0;
}