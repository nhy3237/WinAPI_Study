#include "CNode.h"

CNode::CNode()
{
    pt = { 0,0 };
    g = 0;
    h = 0;
    f = 0;
}

CNode::CNode(POINT ptCenter, POINT pt, POINT ptGoal, bool start, int g, int h)
{
	int xTemp, yTemp, xyTemp;

	this->pt = pt;
	this->g = g;
	this->h = h;

	if (start)
	{
		this->g = 0; this->h = 0; this->f = 0;
	}
	else
	{
		if (pt.x - ptCenter.x == 0 || pt.y - ptCenter.y == 0) this->g += 10;
		else this->g += 14;

		xTemp = abs(ptGoal.x - pt.x);
		yTemp = abs(ptGoal.y - pt.y);
		while (xTemp != 0 && yTemp != 0)
		{
			this->h += 14;
			xTemp--; yTemp--;
		}

		xyTemp = xTemp + yTemp;

		while (xyTemp != 0)
		{
			this->h += 10;
			xyTemp--;
		}

		f = this->g + this->h;
	}
}

CNode::~CNode()
{
}

POINT CNode::getPoint()
{
	return pt;
}

int CNode::getG()
{
	return g;
}

int CNode::getH()
{
	return h;
}

int CNode::getF()
{
	return f;
}

