/*
Q1. 격자 그리기
    DrawGrid(...) 함수 구현하라.
    격차 위치, 격자 width, height,
    격자 개수 또는 격자 간격을 인자로 한다.
    
    
Q2. 원 그리기
    DrawCircle(...) 함수를 구현하라.
    원의 위치(center), 반지름을 인자로 한다.
    
Q3. 해바라기 그리기 함수를 구현하라.
    원을 그리기 위한 기본 정보에 외각에 그려질 원의 개수를 입력 받아
    해바라기 형식으로 그려지도록 한다. 
    
Q4. 별을 그리는 함수를 구현하라.
    별의 위치, 반지름을 인자로 받도록 한다.*/


// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinAPI_Proj.h"
#include <cmath>

#define MAX_LOADSTRING 100
#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//230628
void DrawGrid(HDC hdc, POINT center, int Width, int Height, int Cnt=0);
void DrawCircle(HDC hdc, POINT center, int r);
void DrawFlower(HDC hdc, POINT center, int r, int cnt);
void DrawStar(HDC hdc, POINT center, int r);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
    {
    }
    break;

    case WM_KEYDOWN:
    {
    }
    break;

    case WM_KEYUP:
    {
    }
    break;
    case WM_CHAR:
    {
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
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

        //HPEN hPen, oldPen;
        //hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
        //oldPen = (HPEN)SelectObject(hdc, hPen);

        //HBRUSH hBrush, oldBrush;
        ////hBrush = CreateSolidBrush(RGB(0, 255, 255));
        //hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        //oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    
        POINT center = { 300, 300 };
        //DrawGrid(hdc, center, 400, 200, 5);
        //DrawCircle(hdc, center, 50);
        DrawFlower(hdc, center, 80, 9);
        //Rectangle(hdc, 500, 400, 700, 500);
        //DrawStar(hdc, center, 100);


        /*POINT pt[5] = {{10, 150},
            {250, 30}, {500, 150},
            {350,300}, {150,300} };
        Polygon(hdc, pt, 5);*/

        /*SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);*/

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

// 정보 대화 상자의 메시지 처리기입니다.
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

void DrawGrid(HDC hdc, POINT center, int Width, int Height, int Cnt)
{
    
    for (int i = 0; i <= Cnt; i++)
    {
        int Gap = Height / Cnt;
        MoveToEx(hdc, center.x - Width / 2, center.y - Height / 2 + Gap * i, NULL);
        LineTo(hdc, center.x + Width / 2, center.y - Height / 2 + Gap * i);

        Gap = Width / Cnt;
        MoveToEx(hdc, center.x - Width / 2 + Gap * i, center.y - Height / 2, NULL);
        LineTo(hdc, center.x - Width / 2 + Gap * i, center.y + Height / 2);
    }
}

void DrawCircle(HDC hdc, POINT center, int r)
{
    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

void DrawFlower(HDC hdc, POINT center, int r, int cnt)
{
    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);

    int angle = 360 / cnt;
    POINT XY;
    int r2, d;

    r2 = r * sin(degreeToRadian(angle / 2)) / (1 - sin(degreeToRadian(angle / 2)));
    d = r + r2;

    for (int i = 0; i < cnt; i++)
    {
        XY.x = d * cos(degreeToRadian(angle * i)) + center.x;
        XY.y = d * sin(degreeToRadian(angle * i)) + center.y;
        Ellipse(hdc, XY.x - r2, XY.y - r2, XY.x + r2, XY.y + r2);
    }
}

void DrawStar(HDC hdc, POINT center, int r)
{
    int cnt = 5, angle = 360 / cnt; 
    POINT XY1, XY2, point[10];

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
