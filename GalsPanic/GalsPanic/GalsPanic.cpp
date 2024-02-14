// GalsPanic.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GalsPanic.h"
#include "Cstone.h"

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

//230713
#pragma comment(lib, "msimg32.lib")

HBITMAP hBackImage;
BITMAP bitBack;

HBITMAP hFrontImage;
BITMAP bitFront;

HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);

RECT rectView;

void CreateBitmap();
void DeleteBitmap();

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
    LoadStringW(hInstance, IDC_GALSPANIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALSPANIC));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALSPANIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALSPANIC);
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
      CW_USEDEFAULT, 0, 1055, 950, nullptr, nullptr, hInstance, nullptr);

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

// 230719
#define timer_ID_1 11

static int ckey, hkey;
enum { RIGHT, LEFT, UP, DOWN };
static POINT ptStonePos;

static vector<Cstone*> wStone;
static Cstone* uStone;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
    {
        ptStonePos = { 0, 0 };
        uStone = new Cstone(ptStonePos);
        wStone.push_back(uStone);
        
        GetClientRect(hWnd, &rectView);
        CreateBitmap();
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

    case WM_KEYDOWN:
        if (wParam == VK_RIGHT)
        {
            // 서로 반대 방향으로는 바뀔 수 없게 - 230709
            if (hkey != RIGHT && hkey != LEFT)
            {
                hkey = ckey;
                ckey = RIGHT;
            }
        }
        else if (wParam == VK_LEFT)
        {
            if (hkey != LEFT && hkey != RIGHT)
            {
                hkey = ckey;
                ckey = LEFT;
            }
        }
        else if (wParam == VK_UP)
        {
            if (hkey != UP && hkey != DOWN)
            {
                hkey = ckey;
                ckey = UP;
            }
        }
        else if (wParam == VK_DOWN)
        {
            if (hkey != DOWN && hkey != UP)
            {
                hkey = ckey;
                ckey = DOWN;
            }
        }

        InvalidateRgn(hWnd, NULL, FALSE);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            Rectangle(hdc, 140, 120, 160, 140);
            DrawBitmapDoubleBuffering(hWnd, hdc);
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteBitmap();
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

void CreateBitmap()
{
    // >> : 경수
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/경수.bmp"),
            IMAGE_BITMAP, 1009, 769, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러1"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    { // : front
        hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("images/경수1-1.bmp"),
            IMAGE_BITMAP, 1009, 769, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hFrontImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러4"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hFrontImage, sizeof(BITMAP), &bitFront);
    }
}

void DeleteBitmap()
{    
    DeleteObject(hBackImage);
    DeleteObject(hFrontImage);
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    /*hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);*/

    { // : 경수
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        HPEN hPen = CreatePen(PS_ALTERNATE, 10, RGB(219, 236, 242));
        HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        RECT rectView = { 10,100,1030,880 };

        Rectangle(hdc, rectView.left, rectView.top, rectView.right, rectView.bottom);

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        
        BitBlt(hdc, 15, 105, bx, by, hMemDC, 0, 0, RGB(255,255,255));

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    {   // >> : front
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hFrontImage);
        bx = bitFront.bmWidth;
        by = bitFront.bmHeight;

        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(219, 236, 242));
        HPEN oldPen = (HPEN)SelectObject(hMemDC, hPen);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);

        Rectangle(hMemDC, 0, 0, 150, 150);

        hPen = CreatePen(PS_ALTERNATE, 1, RGB(31, 81, 183));
        oldPen = (HPEN)SelectObject(hMemDC, hPen);
        hBrush = CreateSolidBrush(RGB(103, 153, 255));
        oldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);

        Rectangle(hMemDC, 140, 120, 160, 140);

        SelectObject(hMemDC, oldBrush);
        DeleteObject(hBrush);
        SelectObject(hMemDC, oldPen);
        DeleteObject(hPen);

        TransparentBlt(hdc, 15, 105, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}