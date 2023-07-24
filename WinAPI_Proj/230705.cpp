/*
 Q2. 1) ������ũ ���� p.125 ����
     2) ���� ����
     3) ���潺 ����
     ���� Ÿ��Ʋ ��� -> Press Any key
     �� ���� ����
      ���� ��� ��� -> ����, ��ŷ
                    ���� ��/���
 */

// WindowsProject1.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include <commdlg.h>

// 0714
HWND ChildWnd[2];
LRESULT CALLBACK    ChildWndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc2(HWND, UINT, WPARAM, LPARAM);

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


// >> : GDI+ -230710
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

ULONG_PTR g_GdiToken;

void Gdi_Init();
void Gdi_Draw(HDC hdc);
void Gdi_End();

// << :

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
// << : FUNC

// >> : double buffering
HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
// << : double buffering

// << :

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    Gdi_Init();

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));

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

    Gdi_End();

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

    // 0714
    RegisterClassExW(&wcex);

    // split window 1
    wcex.lpfnWndProc = ChildWndProc1;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 1");
    RegisterClassExW(&wcex); // ������ ���
    // split window 2
    wcex.lpfnWndProc = ChildWndProc2;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 2");
    RegisterClassExW(&wcex); // ������ ���

    return NULL;
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
        //SetTimer(hWnd, timer_ID_2, 20, AniProc);
        GetClientRect(hWnd, &rectView);

        // >> : split window
        {
            ChildWnd[0] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 1"), NULL, WS_CHILD | WS_VISIBLE,
                0, 0, rectView.right, rectView.bottom / 2 - 1, hWnd, NULL, hInst, NULL);
            ChildWnd[1] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 2"), NULL, WS_CHILD | WS_VISIBLE,
                0, rectView.bottom / 2 + 1, rectView.right, rectView.bottom / 2 - 1, hWnd, NULL, hInst, NULL);
        }
    }
        break;

    case WM_TIMER:
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
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
    /*case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        //DrawBitmap(hWnd, hdc);
        DrawBitmapDoubleBuffering(hWnd, hdc);
        DrawRectText(hdc);
        EndPaint(hWnd, &ps);
    }
    break;*/
    case WM_DESTROY:
        //KillTimer(hWnd, timer_ID_2);
        DeleteBitmap();
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

void CreateBitmap()
{
    // >> : ����
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("����.bmp"),
            IMAGE_BITMAP, 650, 800, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����1"), _T("����"), MB_OK);
            return;
        }
        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    //>> : �ð�
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("image/sigong.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����2"), _T("����"), MB_OK);
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
            MessageBox(NULL, _T("�̹��� �ε� ����3"), _T("����"), MB_OK);
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
            MessageBox(NULL, _T("�̹��� �ε� ����4"), _T("����"), MB_OK);
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

    { // : ����
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

        //SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    { //>> : �ð�
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
    TCHAR strTest[] = _T("�̹��� ���");
    TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
}

void Update()
{
    // : wm_keydown, wm_keyup       AŰ 0x8000(������ ��) 0x8001(���ȴ� ������..?)
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

    { // : ����
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    { //>> : �ð�
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
    // << : hMemDC�� �׷��ֱ�
        
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
    
    Gdi_Draw(hMemDC); // << : gdi+
    
    // >> : hdc�� �׷��ֱ�
    //BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);
    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom,
        hMemDC, 0, 0, rectView.right, rectView.bottom,
        RGB(255, 0, 255));

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}

void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_Draw(HDC hdc)
{
    Graphics graphics(hdc);

    // : text
    SolidBrush brush(Color(255, 255, 0, 0));
    FontFamily fontFamily(L"Times New Roman");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 20.0f);
    graphics.DrawString(L"Hello GDI+!!", -1, &font, pointF, &brush);

    // : line
    Pen pen(Color(128, 255, 0, 0));
    graphics.DrawLine(&pen, 0, 0, 200, 100);

    // : image
    Image img((WCHAR*)L"image/sigong.png");
    int w = img.GetWidth();
    int h = img.GetHeight();
    graphics.DrawImage(&img, 300, 100, w, h);

    // : ani
    Image img2((WCHAR*)L"image/zero_run.png");
    w = img2.GetWidth() / SPRITE_FRAME_COUNT_X;
    h = img2.GetHeight() / SPRITE_FRAME_COUNT_Y;
    int xStart = curframe * w;
    int yStart = 0;

    ImageAttributes imgAttr0;
    imgAttr0.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
    graphics.DrawImage(&img2, Rect(450, 100, w, h), xStart, yStart, w, h, UnitPixel, &imgAttr0);

    // >> : alpha rect
    brush.SetColor(Color(128, 255, 0, 0));
    graphics.FillRectangle(&brush, 100, 100, 200, 300);

    // >> : ratation
    Image* pImg = nullptr;
    pImg = Image::FromFile((WCHAR*)L"image/sigong.png");
    int xPos = 400;
    int yPos = 200;
    if (pImg)
    {
        w = pImg->GetWidth();
        h = pImg->GetHeight();

        Gdiplus::Matrix mat;
        static int rot = 0;
        mat.RotateAt((rot % 360), Gdiplus::PointF(xPos + (float)(w / 2), yPos + (float)(h / 2)));
        graphics.SetTransform(&mat);
        graphics.DrawImage(pImg, xPos, yPos, w, h);
        rot += 10;

        mat.Reset();
        graphics.SetTransform(&mat);
    }
    
    ImageAttributes imgAttr;
    imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));
    xPos = 500;
    graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPoint, &imgAttr);
    graphics.DrawImage(pImg, xPos + 50, yPos, w, h);

    if (pImg)
    {
        REAL transparency = 0.5f;
        ColorMatrix colorMatrix =
        {
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, transparency, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        imgAttr.SetColorMatrix(&colorMatrix);
        xPos = 600;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPoint, &imgAttr);

        ColorMatrix grayMatrix =
        {
            0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
            0.6f, 0.6f, 0.6f, 0.0f, 0.0f,
            0.1f, 0.1f, 0.1f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        };
        imgAttr.SetColorMatrix(&grayMatrix);
        xPos = 700;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPoint, &imgAttr);

        xPos = 800;
        pImg->RotateFlip(RotateNoneFlipX);
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPoint, &imgAttr);

        delete pImg;
    }
}

void Gdi_End()
{
    GdiplusShutdown(g_GdiToken);
}

#define IDC_CHILD1_BTN 2000

LRESULT CALLBACK ChildWndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hBtn;
    static bool bToggle = false;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 11, 30, AniProc);

        hBtn = CreateWindow(_T("button"), _T("OK"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200, 10, 100, 30, hWnd, (HMENU)IDC_CHILD1_BTN, hInst, NULL);

        break;
    case WM_PAINT:
    {
        HDC hdc;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hWnd, &ps);

        DrawBitmapDoubleBuffering(hWnd, hdc);
        if (bToggle)
            TextOut(hdc, 200, 50, _T("Button Clicked"), 14);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHILD1_BTN:
            bToggle = !bToggle;
            break;
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 11);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptMouse;

    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&ptMouse);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        TCHAR str[128];
        wsprintf(str, TEXT("WORLD POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 30, str, lstrlen(str));

        ScreenToClient(hWnd, &ptMouse);

        wsprintf(str, TEXT("LOCAL POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 50, str, lstrlen(str));

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}