#pragma once
#include "framework.h"

class Astar
{
private:
	POINT pt;
	int g, h, f;

	bool road;

public:
	Astar();
	Astar(POINT ptCenter, POINT pt, POINT ptGoal, bool start, int g, int h);
	~Astar();
	POINT getPoint();
	int getG();
	int getH();
	int getF();
};
