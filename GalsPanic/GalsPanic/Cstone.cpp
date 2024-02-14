#include "Cstone.h"

Cstone::Cstone()
{
    ptStonePos = { 0, 0 };
}

Cstone::Cstone(POINT ptStonePos)
{
    this->ptStonePos = { ptStonePos.x, ptStonePos.y };
}

Cstone::~Cstone()
{
}

void Cstone::Draw(HDC hdc, bool chkHead)
{
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;

    if (chkHead)
    {
        hPen = CreatePen(PS_ALTERNATE, 1, RGB(75, 132, 69));
        hBrush = CreateSolidBrush(RGB(75, 132, 69));
    }
    else
    {
        hPen = CreatePen(PS_ALTERNATE, 1, RGB(93, 150, 87));
        hBrush = CreateSolidBrush(RGB(93, 150, 87));
    }
    oldPen = (HPEN)SelectObject(hdc, hPen);
    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, 115 + ptStonePos.x, 120 + ptStonePos.y, 150 + ptStonePos.x, 155 + ptStonePos.y);

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void Cstone::DrawMap(HDC hdc, RECT rectView)
{
    HPEN hPen = CreatePen(PS_ALTERNATE, 1, RGB(237, 250, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(RGB(243, 252, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // 0 0 35 35 / 35 35 70 70
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Rectangle(hdc, 10 + (70 * j), 50 + (70 * i), 45 + (70 * j), 85 + (70 * i));
            Rectangle(hdc, 45 + (70 * j), 85 + (70 * i), 80 + (70 * j), 120 + (70 * i));
        }
    }

    hPen = CreatePen(PS_ALTERNATE, 2, RGB(219, 236, 242));
    oldPen = (HPEN)SelectObject(hdc, hPen);
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, rectView.left, rectView.top, rectView.right, rectView.bottom);

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void Cstone::HeadUpdate(int ckey, int* hkey)
{
    switch (ckey)
    {
    case RIGHT:
        *hkey = RIGHT;
        ptStonePos.x += 35;
        break;
    case LEFT:
        *hkey = LEFT;
        ptStonePos.x -= 35;
        break;
    case UP:
        *hkey = UP;
        ptStonePos.y -= 35;
        break;
    case DOWN:
        *hkey = DOWN;
        ptStonePos.y += 35;
        break;
    }
}

void Cstone::LineUpdate(POINT ptStonePos)
{
    this->ptStonePos = { ptStonePos };
}

POINT Cstone::getStonePos()
{
    return ptStonePos;
}
