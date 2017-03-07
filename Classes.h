#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <random>
#include <iomanip>
#include <cmath>

using namespace std;

#define ZERO_TO_ONE (double)rand()/RAND_MAX

#ifndef CLASSES_H
#define CLASSES_H

//==============================
//	GridWorld Class
//===============================

class GridWorld {
private:
	int sizeX, sizeY;
	int sgoal;
	int lgoal[2];
	int agent[2];
public:
	GridWorld(int, int, int, int);
	int new_state(int,int);
	int give_reward(int);
	void display(int);
	bool found_goal(int);
	void TestA();
	int get_state();
};

//*==============================
//	Agent Class
//===============================

class Agent{
private:
	float **Q_Table;
	GridWorld *world;
	double e, a, g;	//Epsilon, Alpha, and Gamma
	int size;
public:
	int state;
	Agent(int,double,double,double,GridWorld*);
	void set_state(int);
	// void decide();
	// void action();
	int get_state();
	void get_array(int,int);
	void ruleOfThumb();
	void TestB();
	void TestC();
	// int greedy();
	// void update(int);
};//*/

//===============================
//	Functions
//===============================

void state2coord(int*,int,int);

#endif