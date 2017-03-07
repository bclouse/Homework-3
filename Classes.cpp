#include "Classes.h"

//===============================
//	GridWorld Class
//===============================

GridWorld::GridWorld(int sx, int sy, int gx, int gy) {
	sizeX = sx; sizeY = sy;
	sgoal = (gy-1)*sx+(gx-1);
	lgoal[0] = gx-1; lgoal[1] = gy-1;
	agent[0] = -5; agent[1] = 5;
}

int GridWorld::give_reward(int state) {
	int reward = 0;
	if (state == sgoal) {
		reward = 100;
	}
	return reward-1;
}

int GridWorld::new_state(int state, int direction) {
	int original = state;
	switch(direction) {
		case 0:								//UP 
			if (state/sizeX != 0) {
				state -= sizeX;
			}
			break;
		case 1:								//RIGHT
			if (state%sizeX != sizeX-1) {
				state++;
			}
			break;
		case 2: 								//DOWN
			if (state/sizeX != sizeY-1) {
				state += sizeX;
			}
			break;
		case 3: 								//LEFT
			if (state%sizeX != 0) {
				state--;
			}
			break;
	}
	state2coord(agent,sizeX,state);
	return state;
}

void GridWorld::display(int agent_state) {
	int current_state;
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			current_state = i*sizeX+j;
			if (current_state == sgoal) {
				if (agent_state == sgoal) {
					cout << "O ";
				} else {
					cout << "G ";
				}
			} else if (current_state == agent_state) {
				cout << "A ";
			} else {
				cout << ". ";
			}
		}
		cout << endl;
	}
}

void GridWorld::TestA() {
	if (agent[0] < 0) 		agent[0] = 0;
	if (agent[0] > sizeX) 	agent[0] = sizeX;
	if (agent[1] < 0) 		agent[1] = 0;
	if (agent[1] > sizeY) 	agent[1] = sizeY;
	assert(agent[0] >= 0 && agent[0] <= sizeX);
	assert(agent[1] >= 0 && agent[1] <= sizeY);
}

bool GridWorld::found_goal(int input) {
	if (input == sgoal) 
		return true;
	return false;
}

int GridWorld::get_state() {
	int output = agent[1]*sizeX+agent[0];
	cout << agent[0] << ',' << agent[1] << endl;
	cout << output << endl;
	return output;
}

 //===============================
//	Agent Class
//===============================
//*
Agent::Agent(int n, double x,double y,double z,GridWorld *grid) {
	e = x;
	a = y;
	g = z;
	size = n;
	state = 0;
	world = grid;
}

void Agent::get_array(int n, int m) {
	Q_Table = new float* [n]();
	for (int i = 0; i < n; i++) {
		Q_Table[i] = new float [m]();
	}
}

void Agent::set_state(int n) {
	state = n;
}

int Agent::get_state() {
	return state;
}

void Agent::ruleOfThumb() {
	int old;
	int dir = 0;
	bool left = true;
	while (!world->found_goal(state)) {
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

		world->display(state);
		cout << endl;
	}
	TestC();
}

void Agent::TestB() {
	assert(world->found_goal(state));
	cout << "Test B succeeded!\n";
}

void Agent::TestC() {
	assert(world->found_goal(state));
	cout << "Test C succeeded!\n";
}

/*
void Agent::decide() {
	if (ZERO_TO_ONE <= e) {
		return (rand()%4)+1;
	} else {
		return greedy();
	}
}

void Agent::greedy() {

}

void Agent::action() {
	
}

//*/
//===============================
//	Functions
//===============================

void state2coord(int loc[2], int sizeX, int state) {
	loc[0] = state%sizeX;
	loc[1] = state/sizeX;
}