#include "Astar.h"

Astar::Astar()
{
	POINT pt = { 0,0 };
	g = 0;
	h = 0;
	f = 0;

	road = false;
}

Astar::Astar(POINT ptCenter, POINT pt, POINT ptGoal, bool start, int g, int h)
{
	int xTemp, yTemp, xyTemp;

	this->pt = pt;
	this->g = g;
	this->h = h;
	road = false;

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
		while (xTemp != 0 || yTemp != 0)
		{
			if (xTemp == 0 || yTemp == 0) break;
			this->h += 14;
			xTemp--; yTemp--;
		}

		xyTemp = xTemp + yTemp;

		while (xyTemp != 0)
		{
			if (xyTemp == 0) break;
			this->h += 10;
			xyTemp--;
		}

		f = this->g + this->h;
	}
}

Astar::~Astar()
{
}

POINT Astar::getPoint()
{
	return pt;
}

int Astar::getG()
{
	return g;
}

int Astar::getH()
{
	return h;
}

int Astar::getF()
{
	return f;
}
