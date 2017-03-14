#include "Classes.h"

//===============================
//	GridWorld Class
//===============================

GridWorld::GridWorld(int sx, int sy, int gx, int gy) {
	sizeX = sx; sizeY = sy;
	goalX = gx-1; goalY = gy-1;
	agentX = 2; agentY = 2;
	goal_state = coord2state(goalX, goalY, sizeX);

	int count = 0;
	state_list = new int*[sx]();
	walls = new int*[sx]();
	for (int i = 0; i < sx; i++) {
		state_list[i] = new int[sy]();
		walls[i] = new int[sx]();
	}
	FILE *fp = fopen("walls.txt", "r");
	for (int i = 0; i < sy; i++) {
		for (int j = 0; j < sx; j++) {
			fscanf(fp, "%d ", &(walls[j][i]));
		}
	}
	fclose(fp);
}
//*
int GridWorld::new_state(int agent_state, int direction) {
	switch (direction) {
		case 0: 				//UP
			if (agentY != 0 && !walls[agentX][agentY-1]) {
				agentY--;
			}
			break;
		case 1: 				//RIGHT
			if (agentX != sizeX-1 && !walls[agentX+1][agentY]) {
				agentX++;
			}
			break;
		case 2: 				//DOWN
			if (agentY != sizeY-1 && !walls[agentX][agentY+1]) {
				agentY++;;
			}
			break;
		case 3: 				//LEFT
			if (agentX != 0 && !walls[agentX-1][agentY]) {
				agentX--;;
			}
			break;
	}
	return state_list[agentX][agentY];
}

int GridWorld::get_reward(int agent_state) {
	int reward = 0;
	if (goal_state == agent_state) {
		reward += 100;
	}
	return reward-1;
}

void GridWorld::display(int agent_state) {
	int current_state;

	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX; i++) {
			if (goalX == i && goalY == j) {
				cout << "G ";
			} else if (agentX == i && agentY == j) {
				cout << "A ";
			} else if (walls[i][j]) {
				cout << "W ";
			} else {
				cout << ". ";
			}
		}
		cout << endl;
	}
}

bool GridWorld::found_goal(int agent_state) {
	if (goal_state == agent_state) {
		agentX = 2; agentY = 2;
		return true;
	} else {
		return false;
	}
}

void GridWorld::set_representation(bool grid_representation) {
	int count = 0;

	if (!grid_representation) goal_state = 4;

	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX; i++) {
			if (grid_representation) {
				state_list[i][j] = count++;
			} else {
				if (i < goalX && j < goalY) {				//NW
					state_list[i][j] = 0;
				} else if (i == goalX && j < goalY) {	//N
					state_list[i][j] = 1;
				} else if (i > goalX && j < goalY) {	//NE
					state_list[i][j] = 2;
				} else if (i < goalX && j == goalY) {	//W
					state_list[i][j] = 3;
				} else if (i == goalX && j == goalY) {	//GOAL
					state_list[i][j] = 4;
				} else if (i > goalX && j == goalY) {	//E
					state_list[i][j] = 5;
				} else if (i < goalX && j > goalY) {	//SW
					state_list[i][j] = 6;
				} else if (i == goalX && j > goalY) {	//S
					state_list[i][j] = 7;
				} else if (i > goalX && j > goalY) {	//SE
					state_list[i][j] = 8;
				}
			}
		}
	}
}

void GridWorld::clear() {
	for (int i = 0; i < sizeX; i++) {
		delete[] state_list[i];
		delete[] walls[i];
	}
	delete[] state_list;
	delete[] walls;
}

//===============================
//	Agent Class
//===============================

Agent::Agent(int n, double ep, double alpha, double gamma, GridWorld* grid) {
	e = ep;
	a = alpha;
	g = gamma;
	world = grid;
	size = n;
	Q_Table = new float*[n]();
	for (int i = 0; i < n; i++) {
		Q_Table[i] = new float[4]();
	}
	state = 0;
	time = 0;
}

void Agent::set_state(int s) {
	origin = state = s;
}

void Agent::clear() {
	for (int i = 0; i < size; i++) {
		delete[] Q_Table[i];
	}
	delete[] Q_Table;
}

void Agent::display() {
	cout << "GRID INFO\n";
	world->display(state);
	cout << "\nAGENT INFO\n";
	cout << "Epsilon: " << e << "\nAlpha: " << a << "\nGamma: " << g << endl;
	/*
	cout << "Q_Table info:\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%9f\t",Q_Table[i][j]);
		}
		cout << endl;
	}//*/
}

int Agent::action() {
	int direction;
	int testing = 0;
	int nstate;

	while (!(world->found_goal(state))) {
		direction = decide();
		nstate = world->new_state(state,direction);
		update(nstate,direction);
		testing++;
	}
	TestB();
	state = origin;

	return testing;
}

int Agent::action(FILE *fp) {
	int direction;
	int testing = 0;
	int nstate;

	while (!(world->found_goal(state))) {
		fprintf(fp, "%d\t%d\n", state%15,state/15);
		direction = decide();
		nstate = world->new_state(state,direction);
		update(nstate,direction);
		testing++;
	}
	fprintf(fp, "%d\t%d\n", state%15,state/15);
	state = origin;
	return testing;	
}

int Agent::decide() {
	if (ZERO_TO_ONE >= e) {
		return rand_maximum(Q_Table[state], 4);
	} else {
		return (int)rand()%4;
	}
}

void Agent::update(int nstate, int dir) {
	int reward = world->get_reward(nstate);
	int Qmax = Q_Table[nstate][rand_maximum(Q_Table[nstate],4)];

	Q_Table[state][dir] += a*(reward + g*Qmax - Q_Table[state][dir]);
	state = nstate;
}

int Agent::man_move(int dir) {
	state = world->new_state(state,dir);
	return state;
}

void Agent::reset() {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < size; i++) {
			Q_Table[i][j] = 0;
		}
	}
}

void Agent::ruleOfThumb() {
	int old;
	int dir = 0;
	bool left = true;
	int count;

	while (!world->found_goal(state) && count < 10000) {
		old = state;
		if (dir == 0) {
			state = world->new_state(state,1);
		} else if (dir == 1) {
			state = world->new_state(state,2);
			if (left) {
				dir = 2; left = false;
			} else {
				dir = 0; left = true;
			}
		} else {
			state = world->new_state(state,3);
		}

		if (old == state) dir = 1;
		count++;
	}
	TestA();
	world->display(state);
}

void Agent::TestA() {
	assert(!world->found_goal(state));
	cout << "\nRule of Thumb Failed\n\n";
}

void Agent::TestB()  {
	assert(world->found_goal(state));
}

// void Agent::TestD() {
// 	for (int i = 0; i < size; i++) {
// 		for (int j = 0; j < 4; j++) {
// 			assert(Q_Table[i][j] <= 100);
// 		}
// 	}
// }

// void Agent::TestE() {
// 	assert(state == origin);
// }

// void Agent::TestF(int min,float val) {
// 	float range = min*0.5;
// 	assert(val >= min-range && val <= min+range);
// }

// void Agent::TestG() {
// 	assert(state == origin);
// }

//===============================
//	Functions
//===============================

void state2coord(int loc[2], int sizeX, int state) {
	loc[0] = state%sizeX;
	loc[1] = state/sizeX;
}

int coord2state(int x, int y, int xsize) {
	return y*xsize+x;	
}

int rand_maximum(float arr[], int n) {
	vector<int> max;

	max.push_back(0);
	for (int i = 1; i < n; i++) {
		if (arr[max[0]] < arr[i]) {
			max.clear();
			max.push_back(i);
		} else if (arr[max[0]] == arr[i]) {
			max.push_back(i);
		}
	}
	return max[rand()%max.size()];
}