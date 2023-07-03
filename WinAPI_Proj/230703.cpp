

// WindowsProject1.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
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


// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE:
        {
            // �� �׸���
            int temp = selectedMenu;

            int ans = MessageBox(hWnd, _T("�� �׸���?"), _T("���� ����"), MB_YESNOCANCEL);
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
            // �簢�� �׸���
            selectedMenu = RECT;
            bFlag = true;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case ID_DRAW_STAR:
            // �� �׸���
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
                _stprintf_s(str, _T("%s ������ ���ڽ��ϱ�?"), ofn.lpstrFile);
                MessageBox(hWnd, str, _T("���� ����"), MB_OK);
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
                _stprintf_s(str, _T("%s ������ ���ڽ��ϱ�?"), ofn.lpstrFile);
                MessageBox(hWnd, str, _T("���� ����"), MB_OK);
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