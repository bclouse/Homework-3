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
	int n = 10, m = 10;
	// int n = rand()%9+2, m = rand()%9+2;
	int x = rand()%(n-1)+2, y = rand()%(m-1)+2;
	GridWorld grid (n,m,x,y);
	grid.TestA();

	Agent dude ((n*m),0.1,0.1,0.5,&grid);
	char input;
	int dir;
	int state = (y-1)*n+x-1;
	bool user;
	
	dude.set_state(grid.get_state());

	cout << "Grid size:     " << n << ", " << m << endl;
	cout << "Goal location: " << x << ", " << y << endl;
	cout << "Goal state:    " << state << endl;

	cout << "Do you want user input? (Y/N) ";
	while(1) {
		cin >> input;
		if (input == 'Y') {
			user = true;
			cout << "\nUse \"wasd\" movement to select direction\n\n";
			break;
		} else if (input == 'N') {
			user = false;
			break;
		} else {
			cout << "INVALID INPUT. (Y/N) ";
		}
	}
	if (user) {
		while (1) {
			grid.display(dude.state);
			cout << "\n>> ";
			cin >> input;
			switch (input) {
				case 'w': dir = 0; break;
				case 'd': dir = 1; break;
				case 's': dir = 2; break;
				case 'a': dir = 3; break;
				default: dir = 0; cout << "Invalid input!\n";
			}
			dude.state = grid.new_state(dude.state,dir);
			if (dude.state == state) {
				break;
			}
			//cout << "state: " << dude.state << endl;
		}
		cout << "\n====YOU FOUND THE GOAL!====\n";
		grid.display(state);
		dude.TestB();
	} else {
		dude.ruleOfThumb();
		cout << "\n====THE AGENT FOUND THE GOAL!====\n";
	}

	return 0;
}