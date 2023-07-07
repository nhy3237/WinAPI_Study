/*
 Q2. 1) 스네이크 게임 p.125 참고
     2) 벽돌 깨기
     3) 디펜스 게임
     게임 타이틀 출력 -> Press Any key
     본 게임 진행
      게임 결과 출력 -> 점수, 랭킹
                    파일 입/출력
 */

// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include <commdlg.h>

//>> :
#pragma comment(lib, "msimg32.lib")

HBITMAP hBackImage;
BITMAP bitBack;

HBITMAP hTransparentImage;
BITMAP bitTransparent;

HBITMAP hFrontImage;
BITMAP bitFront;

// >> : Ani
HBITMAP hAniImage;
BITMAP bitAni;
const int SPRITE_SIZE_X = 57;
const int SPRITE_SIZE_Y = 52;

int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;

int SPRITE_FRAME_COUNT_X = 0;
int SPRITE_FRAME_COUNT_Y = 0;
// << : Ani

// << : text
RECT rectView;
// >> : text

// >> : FUNCS..
void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();
void UpdateFrame(HWND hWnd);
void DrawRectText(HDC hdc);
void Update();
void Render();

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
// << : FUNC

// >> : double buffering
HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
// << : double buffering

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

#define timer_ID_2 22

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE :
        GetClientRect(hWnd, &rectView);
        break;

    case WM_CREATE:
    {
        CreateBitmap();
        SetTimer(hWnd, timer_ID_2, 20, AniProc);
        GetClientRect(hWnd, &rectView);
    }
        break;

    case WM_TIMER:
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
        //DrawBitmap(hWnd, hdc);
        DrawBitmapDoubleBuffering(hWnd, hdc);
        DrawRectText(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, timer_ID_2);
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
    // >> : 백현
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("백현.bmp"),
            IMAGE_BITMAP, 650, 800, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러1"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    //>> : 시공
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("image/sigong.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러2"), _T("에러"), MB_OK);
            return;
        }

        GetObject(hTransparentImage, sizeof(BITMAP), &bitTransparent);
    }

    // >> : Ani
    {
        hAniImage = (HBITMAP)LoadImage(NULL, TEXT("image/zero_run.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hAniImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러3"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hAniImage, sizeof(BITMAP), &bitAni);

        // :
        RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X - 1;
        RUN_FRAME_MIN = 2;
        curframe = RUN_FRAME_MIN;

        SPRITE_FRAME_COUNT_X = bitAni.bmWidth / SPRITE_SIZE_X;
        SPRITE_FRAME_COUNT_Y = bitAni.bmHeight / SPRITE_SIZE_Y;
    }

    { // : background
        hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("image/Background.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hFrontImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러4"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hFrontImage, sizeof(BITMAP), &bitFront);
    }
}

void DrawBitmap(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    { // : 백현
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

        //SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    { //>> : 시공
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        //BitBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, SRCCOPY);
        TransparentBlt(hdc, 150, 150, bx * 2, by * 2, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

        TransparentBlt(hdc, 250, 150, bx / 2, by / 2, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

        //SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    { //>> : Ani
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;

        int xStart = curframe * bx;
        int yStart = 0;

        TransparentBlt(hdc, 150, 300, bx, by, hMemDC, xStart, yStart, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}

void DeleteBitmap()
{
    DeleteObject(hAniImage);
    DeleteObject(hBackImage);
    DeleteObject(hTransparentImage);
}

void UpdateFrame(HWND hWnd)
{
    curframe++;
    if (curframe > RUN_FRAME_MAX)
    {
        curframe = RUN_FRAME_MIN;
    }
    InvalidateRect(hWnd, NULL, false);
}

static int yPos = 0;

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    UpdateFrame(hWnd);

    yPos += 5;
    if (yPos > rectView.bottom)yPos = 0;
}

void DrawRectText(HDC hdc)
{
    TCHAR strTest[] = _T("이미지 출력");
    TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
}

void Update()
{
    // : wm_keydown, wm_keyup       A키 0x8000(눌렸을 때) 0x8001(눌렸다 뗐을때..?)
    if (GetKeyState('A') & 0x8000)
    {

    }

    if (GetKeyState('D') & 0x8000)
    {

    }

    if (GetAsyncKeyState('W') & 0x8000)
    {

    }

    if (GetAsyncKeyState('S') & 0x8000)
    {

    }
}

void Render()
{
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    HDC hMemDC2;
    HBITMAP hOldBitmap2;

    hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);

    { // : 백현
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    { //>> : 시공
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        TransparentBlt(hMemDC, 0, 0, bx * 2, by * 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        TransparentBlt(hMemDC, 0, 100, bx / 2, by / 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    { //>> : Ani
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;

        int xStart = curframe * bx;
        int yStart = 0;

        TransparentBlt(hMemDC, 150, 300, bx, by, hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }
    // << : hMemDC에 그려주기
        
    {   // >> : front
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hFrontImage);
        bx = bitFront.bmWidth;
        by = bitFront.bmHeight;

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);

        Ellipse(hMemDC2, 250, 100, 600, 500);

        SelectObject(hMemDC2, oldBrush);
        DeleteObject(hBrush);

        TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC2, hOldBitmap2);

        DeleteDC(hMemDC2);
    }
    
    // >> : hdc에 그려주기
    //BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);
    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom,
        hMemDC, 0, 0, rectView.right, rectView.bottom,
        RGB(255, 0, 255));

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
