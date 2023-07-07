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
//��ǥ
//�̵� ���ǵ�
//�̵� ����(�ޱ� �Ǵ� ����)
//Ÿ��(��, ��, �簢��)
//
//public:
//    virtual void Update() = 0;
//    virtual void Draw() = 0;
//    virtual BOOL Collision(..) = 0;
//    // : �� �� �Լ� .. setposition
//};
//
//CCircle: public CObject
//{
//private:
//    ������
//public:
//    virtual void Update() override;
//    virtual void Draw() override;
//    virtual BOOL Collision(..) override;
//}