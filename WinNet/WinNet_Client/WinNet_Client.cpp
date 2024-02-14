// WinNet_Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "WinNet_Client.h"
#include <WinSock2.h>
#include <stdio.h>
#include <list>
#include <vector>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define WM_ASYNC WM_USER+1

// >> :
WSADATA wsadata;
SOCKET s, cs;
TCHAR msg[200] = { 0 };
SOCKADDR_IN addr = { 0 }, c_addr = { 0 };
int Size, msgLen;
char buffer[100] = { 0 };

TCHAR str[100] = { 0 };
int msgCount = 0;

int InitClient(HWND hWnd);
void CloseClient();
int SendMessageToServer();
// << :

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int WinSocketTest_Client();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //return WinSocketTest_Client();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINNETCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINNETCLIENT));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINNETCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINNETCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return InitClient(hWnd);
        break;

    case WM_ASYNC:
        switch (lParam)
        {
        case FD_READ:
            msgLen = recv(s, buffer, 100, 0);
            buffer[msgLen] = NULL;
#ifdef _UNICODE
            msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
            msg[msgLen] = NULL;
#else
            strcpy(msg, buffer);
#endif
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
        
    case WM_CHAR:
        if (wParam == VK_RETURN)
        {
            return SendMessageToServer();
        }
        else if (wParam == VK_BACK)
        {
            if (msgCount <= 0) break;
            str[--msgCount] = NULL;
        }
        else
        {
            str[msgCount++] = wParam;
            str[msgCount] = NULL;
        }
        InvalidateRect(hWnd, NULL, true);
        
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            TextOut(hdc, 10, 10, str, _tcslen(str));
            if (_tcscmp(msg, _T("")))
                TextOut(hdc, 10, 50, msg, _tcslen(msg));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        CloseClient();

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

int WinSocketTest_Client()
{
    WSADATA wsadata;
    SOCKET s;
    SOCKADDR_IN addr = { 0 };

    WSAStartup(MAKEWORD(2, 2), &wsadata);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        MessageBox(NULL, _T("socket failed"), _T("Error"), MB_OK);
        return 0;
    }
    else
    {
        MessageBox(NULL, _T("socket success"), _T("Success"), MB_OK);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = 20;
    addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.79");

    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        MessageBox(NULL, _T("connect failed"), _T("Error"), MB_OK);
        return 0;
    }
    else
    {
        MessageBox(NULL, _T("connect success"), _T("Success"), MB_OK);
    }

    send(s, "안녕 server!", 12, 0);

    closesocket(s);
    WSACleanup();

    return 1;
}

int InitClient(HWND hWnd)
{
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = 20;
    addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.79");

    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        return 0;
    }
    else
    {
        WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);
    }
    return 1;
}

void CloseClient()
{
    closesocket(s);
    WSACleanup();
}

int SendMessageToServer()
{
    if (s == INVALID_SOCKET) return 0;

#ifdef _UNICODE
    msgLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, msgLen, NULL, NULL);
    msg[msgLen] = NULL;

#else
    strcpy(buffer, str);
    msgLen = strlen(buffer);

#endif
    send(s, (LPSTR)buffer, msgLen + 1, 0);
    //msgCount = 0;

    return 1;
}
