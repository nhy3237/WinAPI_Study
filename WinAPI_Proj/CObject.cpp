#include "CObject.h"
#include <ctime>
#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)

CCircle::CCircle()
{   
    center.x = 0;
    center.y = 0;
    speed = 0;
    Xdirection = 0;
    Ydirection = 0;

    r = 0;
}

CCircle::CCircle(POINT center, double r)
{
    this->center = center;
    this->r = r;
    this->speed = 100;
    
    srand(time(NULL));

    this->Xdirection = cos(degreeToRadian(rand() % 360));
    this->Ydirection = sin(degreeToRadian(rand() % 360));
}

CCircle::~CCircle()
{

}

void CCircle::Update(RECT* rectView)
{
    if (center.x + r > rectView->right)
    {
        center.x = rectView->right - r;
        Xdirection = -Xdirection;
    }
    if (center.x - r < rectView->left)
    {
        center.x = rectView->left + r;
        Xdirection = -Xdirection;
    }
    if (center.y - r < rectView->top)
    {
        center.y = rectView->top + r;
        Ydirection = -Ydirection;
    }
    if (center.y + r > rectView->bottom)
    {
        center.y = rectView->bottom - r;
        Ydirection = -Ydirection;
    }

    center.x += speed * Xdirection;
    center.y += speed * Ydirection;
}

void CCircle::Draw(HDC hdc, RECT* rectView)
{
    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

BOOL CCircle::Collision()
{
    return 0;
}

CRect::CRect()
{
    center.x = 0;
    center.y = 0;
    speed = 0;
    Xdirection = 0;
    Ydirection = 0;

    r = 0;
}

CRect::CRect(POINT center, double r)
{

    this->center = center;
    this->r = r;
    this->speed = 100;

    srand(time(NULL));

    this->Xdirection = cos(degreeToRadian(rand() % 360));
    this->Ydirection = sin(degreeToRadian(rand() % 360));
}

CRect::~CRect()
{

}

void CRect::Update(RECT* rectView)
{

}

void CRect::Draw(HDC hdc, RECT* rectView)
{
}

BOOL CRect::Collision()
{
    return 0;
}

CStar::CStar()
{
}

CStar::CStar(POINT center, double r)
{
}

CStar::~CStar()
{
}

void CStar::Update(RECT* rectView)
{
}

void CStar::Draw(HDC hdc, RECT* rectView)
{
}

BOOL CStar::Collision()
{
    return 0;
}
