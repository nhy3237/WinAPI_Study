#pragma once
#include <vector>
using namespace std;

#include "framework.h"
BOOL                InitInstance(HINSTANCE, int);

class CObject
{
protected:
	POINT center;
	double speed;
	double Xdirection, Ydirection;

	int type;

public:
	virtual void Update(RECT *rectView) = 0;
	virtual void Draw(HDC hdc, RECT* rectView) = 0;
	virtual BOOL Collision(double r1, double r2) = 0;
	virtual double getR() = 0;
};

class CCircle :public CObject
{
private:
	double r;

public:
	CCircle();
	CCircle(POINT center, double r);
	~CCircle();
	virtual void Update(RECT* rectView) override;
	virtual void Draw(HDC hdc, RECT* rectView) override;
	virtual BOOL Collision(double r1, double r2) override;
	virtual double getR() override;
};

class CRect :public CObject
{
private:
	double r;

public:
	CRect();
	CRect(POINT center, double r);
	~CRect();
	virtual void Update(RECT* rectView) override;
	virtual void Draw(HDC hdc, RECT* rectView) override;
	virtual BOOL Collision(double r1, double r2) override;
	virtual double getR() override;
};

class CStar :public CObject
{
private:
	double r;

public:
	CStar();
	CStar(POINT center, double r);
	~CStar();
	virtual void Update(RECT* rectView) override;
	virtual void Draw(HDC hdc, RECT* rectView) override;
	virtual BOOL Collision(double r1, double r2) override;
	virtual double getR() override;
};
//class CObject
//{
//protected:
//좌표
//이동 스피드
//이동 방향(앵글 또는 벡터)
//타입(원, 별, 사각형)
//
//public:
//    virtual void Update() = 0;
//    virtual void Draw() = 0;
//    virtual BOOL Collision(..) = 0;
//    // : 그 외 함수 .. setposition
//};
//
//CCircle: public CObject
//{
//private:
//    반지름
//public:
//    virtual void Update() override;
//    virtual void Draw() override;
//    virtual BOOL Collision(..) override;
//}