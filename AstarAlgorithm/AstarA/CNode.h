#pragma once
#include "framework.h"

class CNode
{
private:
	POINT pt;
	int g, h, f;

public:
	CNode();
	CNode(POINT ptCenter, POINT pt, POINT ptGoal, bool start, int g, int h);
	~CNode();
	POINT getPoint();
	int getG();
	int getH();
	int getF();
};
