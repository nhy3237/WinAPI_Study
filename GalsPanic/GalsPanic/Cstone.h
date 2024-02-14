#pragma once
#include <vector>
using namespace std;

#include "framework.h"
BOOL                InitInstance(HINSTANCE, int);

class Cstone
{
private:
	POINT ptStonePos;
	enum { RIGHT, LEFT, UP, DOWN };

public:
	Cstone();
	Cstone(POINT ptStonePos);
	~Cstone();
	void Draw(HDC hdc, bool chkHead);
	void DrawMap(HDC hdc, RECT rectView);
	void HeadUpdate(int ckey, int* hkey);
	void LineUpdate(POINT ptStonePos);
	POINT getStonePos();
	//BOOL FeedCollision(POINT ptFeedPos);
	//BOOL MapCollision(RECT rectView);
	//BOOL BodyCollision(POINT ptBodyPos);
};