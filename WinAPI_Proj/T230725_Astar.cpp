#include "framework.h"
#include "WinAPI_Proj.h"
#include <vector>
#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include "Astar.h"
using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 230726
void DrawMap(HDC hdc, RECT StartRect, RECT GoalRect);

vector<vector<char>> map(10,vector<char>(10,'1'));

vector<Astar*> Openlist;
vector<Astar*> Closedlist;
Astar* list;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));

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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 738, 840, nullptr, nullptr, hInstance, nullptr);

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
    static RECT StartRect, GoalRect;
    static POINT ptStart, ptGoal, ptCenter;
    static POINT ptMouse;
    static BOOL Fflag;

    switch (message)
    {
    case WM_CREATE:
        // 230727
        ptStart = { -1,-1 };
        ptGoal = { -1,-1 };

        break;

    case WM_LBUTTONDOWN:

        if (ptGoal.x > -1)
        {
            map[ptStart.y][ptStart.x] = '1';
            map[ptGoal.y][ptGoal.x] = '1';
            ptStart = { -1,-1 };
            ptGoal = { -1,-1 };
            Fflag = FALSE;
        }

        if (ptGoal.x == -1 && ptGoal.y == -1)
        {
            ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if (ptMouse.x >= 10 && ptMouse.x <= 710 && ptMouse.y >= 50 && ptMouse.y <= 750)
            {
                if (ptStart.x == -1 && ptStart.y == -1)
                {
                    ptStart.x = (ptMouse.x - 10) / 70;
                    ptStart.y = (ptMouse.y - 50) / 70;
                    map[ptStart.y][ptStart.x] = 'S';
                    StartRect = { 11 + (70 * ptStart.x),51 + (70 * ptStart.y),81 + (70 * ptStart.x),121 + (70 * ptStart.y) };
                    GoalRect = { 0,0,0,0 };
                    ptCenter = ptStart;

                    list = new Astar(ptStart, ptStart, { 0,0 }, true, 0, 0);
                    Closedlist.push_back(list);
                }
                else
                {
                    ptGoal.x = (ptMouse.x - 10) / 70;
                    ptGoal.y = (ptMouse.y - 50) / 70; 
                    map[ptGoal.y][ptGoal.x] = 'G';
                    GoalRect = { 11 + (70 * ptGoal.x),51 + (70 * ptGoal.y),81 + (70 * ptGoal.x),121 + (70 * ptGoal.y) };
                    Fflag = TRUE;
                }
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
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
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        // 230726
        DrawMap(hdc, StartRect, GoalRect);

        if (Fflag)
        {
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if ((i == 0 && j == 0) || 
                        ptCenter.x - i < 0 || ptCenter.x - i > 9 || ptCenter.y - j < 0 || ptCenter.y - j > 9 || 
                        (ptCenter.x - i == ptGoal.x && ptCenter.y - j == ptGoal.y) ) continue;

                    list = new Astar(ptCenter, { ptCenter.x - i,ptCenter.y - j }, ptGoal, false,
                                     Closedlist[Closedlist.size() - 1]->getG(), Closedlist[Closedlist.size() - 1]->getH());
                    if (i == -1 && j == -1)
                        Closedlist.push_back(list);
                    else
                    {
                        if (Closedlist[Closedlist.size() - 1]->getF() > list->getF())
                        {
                            Openlist.push_back(Closedlist[Closedlist.size() - 1]);
                            Closedlist[Closedlist.size() - 1] = list;
                        }
                        else if (Closedlist[Closedlist.size() - 1]->getF() == list->getF())
                        {
                            if (Closedlist[Closedlist.size() - 1]->getH() > list->getH())
                            {
                                Openlist.push_back(Closedlist[Closedlist.size() - 1]);
                                Closedlist[Closedlist.size() - 1] = list;
                            }
                        }
                        else
                            Openlist.push_back(list);
                    }
                }
            }
        }

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
        for (int i = 0; i < map.size(); i++)
        {
            for (int j = 0; j < map.size(); j++)
            {
                cout << map[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        for (int i = 0; i < Openlist.size(); i++)
        {
            cout << Openlist[i]->getPoint().x << "," << Openlist[i]->getPoint().y <<"  g : " << Openlist[i]->getG() << " h : " << Openlist[i]->getH() << " f : " << Openlist[i]->getF() << endl;
        }
        cout << endl;
        Openlist.clear();
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);

        // 230727
        map.clear();
        Openlist.clear();
        Closedlist.clear();
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

// 230726
void DrawMap(HDC hdc, RECT StartRect, RECT GoalRect)
{
    // 맵 전체 칸
    for (int i = 0; i < 9; i++)
    {   
        MoveToEx(hdc, 80 + (70 * i), 50, NULL);
        LineTo(hdc, 80 + (70 * i), 750);

        MoveToEx(hdc, 10, 120 + (70 * i), NULL);
        LineTo(hdc, 710, 120 + (70 * i));
    }

    // Start, Goal 칸
    SelectObject(hdc, CreateSolidBrush(RGB(217, 229, 255)));
    SetBkColor(hdc, RGB(217, 229, 255));
    SelectObject(hdc, CreatePen(PS_NULL,0,0));

    if (StartRect.left >= 10)
    {
        Rectangle(hdc, StartRect.left, StartRect.top, StartRect.right, StartRect.bottom);
        DrawText(hdc, _T("Start"), -1, &StartRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }

    if (GoalRect.left >= 10)
    {
        Rectangle(hdc, GoalRect.left, GoalRect.top, GoalRect.right, GoalRect.bottom);
        DrawText(hdc, _T("Goal"), -1, &GoalRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }

    // 맵 전체 테두리
    SelectObject(hdc, CreatePen(PS_ALTERNATE, 0, 0));
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, 10, 50, 710, 750);
}
