
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

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE:
            // 원 그리기
            break;
        case ID_DRAW_RECT:
            // 사각형 그리기
            break;
        case ID_DRAW_STAR:
            // 별 그리기
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
