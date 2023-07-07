
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinAPI_Proj.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include "CObject.h"
#include <vector>
using namespace std;

#define MAX_LOADSTRING 100
#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//230629 - nhy
void DrawRect(HDC hdc, POINT center, int r);
void DrawCircle(HDC hdc, POINT center, int r);
void DrawStar(HDC hdc, POINT center, int r);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(236, 235, 255));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    HWND hWnd = CreateWindowW(szWindowClass, _T("TEST"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

//230629 - nhy
#define timer_ID_1 11
const int circleRadius = 50;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static int flag;
    static POINT ptCurPos;
    static POINT ptMousePos;
    static RECT rectView;

    static vector<CObject *> object;
    static CObject* obj;

    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rectView);
        ptCurPos.x = circleRadius; ptCurPos.y = circleRadius;
        flag = false;
        SetTimer(hWnd, timer_ID_1, 500, NULL);
    }
    break;

    case WM_TIMER:
        /*if (wParam == timer_ID_1)
        {
            ptMousePos.x -= 20;
            ptMousePos.y -= 20;
            InvalidateRgn(hWnd, NULL, TRUE);
        }*/
        

        //for (auto e : object)
        //{
        //    e->Update(&rectView);

        for(int i =0;i<object.size();i++)
        {
            object[i]->Update(&rectView);

            for (int j = i; j < object.size(); j++)
            {
                object[i]->Collision(object[i]->getR(), object[j]->getR());
            }

            InvalidateRgn(hWnd, NULL, TRUE);
        }

        break;

    case WM_LBUTTONDOWN:
    {
        int r = 50;

        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

        obj = new CCircle(ptMousePos, r);
        object.push_back(obj);

        flag = true;
        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;

    case WM_LBUTTONUP:
    {

    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE:
            // �� �׸���
            break;
        case ID_DRAW_RECT:
            // �簢�� �׸���
            break;
        case ID_DRAW_STAR:
            // �� �׸���
        break; 
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        for (auto e : object)
        {
            e->Draw(hdc, &rectView);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void DrawCircle(HDC hdc, POINT center, int r)
{
    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

void DrawRect(HDC hdc, POINT center, int r)
{
    Rectangle(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

void DrawStar(HDC hdc, POINT center, int r)
{
    int cnt = 5, angle = 360 / cnt;
    POINT XY1, XY2;
                
    for (int i = 0; i < cnt; i++)
    {
        XY1.x = r * cos(degreeToRadian(angle * i)) + center.x;
        XY1.y = r * sin(degreeToRadian(angle * i)) + center.y;

        XY2.x = r / 2 * cos(degreeToRadian(angle / 2 + angle * i)) + center.x;
        XY2.y = r / 2 * sin(degreeToRadian(angle / 2 + angle * i)) + center.y;

        MoveToEx(hdc, XY1.x, XY1.y, NULL);
        LineTo(hdc, XY2.x, XY2.y);
    }

    for (int i = 0; i < cnt; i++)
    {
        XY1.x = r * cos(degreeToRadian(angle * i)) + center.x;
        XY1.y = r * sin(degreeToRadian(angle * i)) + center.y;

        MoveToEx(hdc, XY1.x, XY1.y, NULL);
        LineTo(hdc, XY2.x, XY2.y);

        XY2.x = r / 2 * cos(degreeToRadian(angle / 2 + angle * i)) + center.x;
        XY2.y = r / 2 * sin(degreeToRadian(angle / 2 + angle * i)) + center.y;
    }
}
