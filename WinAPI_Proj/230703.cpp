

// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinAPI_Proj.h"
#include <cmath>
#include <commdlg.h>
#include <stdio.h>
using namespace std;

#define MAX_LOADSTRING 100
#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)

#define NONE 0
#define CIRCLE 1
#define RECT 2
#define STAR 3


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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

void OutFromFile(TCHAR filename[], HWND hWnd)
{
    FILE* fPtr;
    HDC hdc;
    int line;
    TCHAR buffer[500];
    line = 0;
    hdc = GetDC(hWnd);
#ifdef _UNICODE
    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
        while (_fgetts(buffer, 100, fPtr) != NULL)
        {
            if (buffer[_tcslen(buffer) - 1] == _T('\n'))
                buffer[_tcslen(buffer) - 1] = NULL;
            TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
            line++;       
        }
    fclose(fPtr);
    ReleaseDC(hWnd, hdc);
}

void SaveFromFile(TCHAR filename[], HWND hWnd)
{
    FILE* fPtr;
    HDC hdc;
    int line;
    TCHAR buffer[500];
    line = 0;
    hdc = GetDC(hWnd);
#ifdef _UNICODE
    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
    while (_fgetts(buffer, 100, fPtr) != NULL)
    {
        if (buffer[_tcslen(buffer) - 1] == _T('\n'))
            buffer[_tcslen(buffer) - 1] = NULL;
        TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
        line++;
    }
    fclose(fPtr);
    ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static TCHAR str[10][100];
    static TCHAR c[100] = _T("");

    static int count = 0, yPos = 240;
    static int cnt = 0;
    static SIZE size;

    static POINT ptCurPos;
    static int flag;
    static bool bFlag;

    static int selectedMenu = NONE;

    switch (message)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        bFlag = false;
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE:
        {
            // 원 그리기
            int temp = selectedMenu;

            int ans = MessageBox(hWnd, _T("원 그릴래?"), _T("도형 선택"), MB_YESNOCANCEL);
            if (ans == IDYES)
            {
                selectedMenu = CIRCLE;
            }
            else if(ans== IDNO)
            {
                selectedMenu = NONE;
            }
            else
            {
                selectedMenu = temp;
            }

            bFlag = true;
            InvalidateRgn(hWnd, NULL, TRUE);
            }
            break;
        case ID_DRAW_RECT:
            // 사각형 그리기
            selectedMenu = RECT;
            bFlag = true;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case ID_DRAW_STAR:
            // 별 그리기
            selectedMenu = STAR;
            bFlag = true;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;

        case ID_FILEOPEN:
        {
            TCHAR filter[] = _T("Every file(*.*) \0*.*\0Text file\0*.txt;*.doc\0");
            TCHAR lpstrFile[100] = _T("");

            OPENFILENAME ofn;
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = filter;
            ofn.lpstrFile = lpstrFile;
            ofn.nMaxFile = 100;
            ofn.lpstrInitialDir = _T(".");
            if (GetOpenFileName(&ofn) != 0)
            {
                TCHAR str[100];
                _stprintf_s(str, _T("%s 파일을 열겠습니까?"), ofn.lpstrFile);
                MessageBox(hWnd, str, _T("파일 선택"), MB_OK);
                OutFromFile(ofn.lpstrFile, hWnd);
            }
        }
        break;

        case ID_FILESAVE:
        {
            TCHAR filter[] = _T("Every file(*.*) \0*.*\0Text file\0*.txt;*.doc\0");
            TCHAR lpstrFile[100] = _T("");

            OPENFILENAME ofn;
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = filter;
            ofn.lpstrFile = lpstrFile;
            ofn.nMaxFile = 100;
            ofn.lpstrInitialDir = _T(".");
            if (GetOpenFileName(&ofn) != 0)
            {
                TCHAR str[100];
                _stprintf_s(str, _T("%s 파일을 열겠습니까?"), ofn.lpstrFile);
                MessageBox(hWnd, str, _T("파일 선택"), MB_OK);
                OutFromFile(ofn.lpstrFile, hWnd);
            }
        }
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

    case WM_CHAR:
    {
        if (wParam == VK_BACK)
        {
            count--;
        }
        else if (wParam == VK_RETURN)
        {
            count = 0;
            if (cnt < 10)
            {
                _tcscpy(str[cnt], c);
                cnt++;
            }
            else
            {
                for (int i = 0; i < 9; i++)
                {
                    _tcscpy(str[i], str[i + 1]);
                }
                _tcscpy(str[9], c);
            }
        }
        else
        {
            c[count++] = wParam;
        }
        c[count] = NULL;
        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        GetTextExtentPoint(hdc, c, _tcslen(c), &size);
        int r = 10;
        ptCurPos.x = 100; ptCurPos.y = 100;

        if (bFlag == true)
        {
            switch (selectedMenu)
            {
            case CIRCLE:
                DrawCircle(hdc, ptCurPos, r);
                break;
            case RECT:
                DrawRect(hdc, ptCurPos, r);
                break;
            case STAR:
                DrawStar(hdc, ptCurPos, r);
                break;
            case NONE:
                break;
            };
            bFlag = false;
        }

        for (int i = 0; i < 10; i++)
        {
            TextOut(hdc, 0, i * 20, str[i], _tcslen(str[i]));
        }

        TextOut(hdc, 0, yPos, c, _tcslen(c));

        SetCaretPos(size.cx, yPos);
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