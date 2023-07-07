/*
Q1. p93 ���� 7
    - Ű down, up ���� üũ
    - ȭ�� ���� �̺�Ʈ �߻�
    - ���¿� ���� �簢�� 4�� �׷��ֱ�

Q2. 1. Ŭ���̾�Ʈ ������ ���콺 Ŭ��, �� ��ġ�� �⺻���� �� ����
        1.1 �������� �簢��, ��, ���� �����ǵ��� �Ѵ�.

    2. ������ �������� �̵� / Ŭ���̾�Ʈ �ܰ����� �ݻ�
    3. Ŭ������ ����
        �簢��/ ���� ���� ������ �۵��ϵ��� �Ѵ�.

    4. �ٸ� ������Ʈ���� ���� ����
        ��� ���� 1�� �ݻ�, 2�� ��ü, 3�� �п��� Ű ����
        4.1 �ݻ� - �ٸ� ������Ʈ�� �ε����� ƨ���
        4.2 ��ü - �� ���迡 ���� �ٸ� ������Ʈ�� ������ ��ü�ؼ� Ŀ���� �ϱ�
        4.3 �п� - �� ���迡 ���� �ٸ� ������Ʈ�� ������ �п��ؼ� �۾����� �ϱ�
            �� > �� > �簢�� > ��
    5. ���� ũ�� �̻�, �Ǵ� ���ϴ� ��������� �Ѵ�.
    6. �� ������Ʈ�� ȸ�� ��Ų��.

    CObject
    {
    protected:
    ��ǥ
    �̵� ���ǵ�
    �̵� ����(�ޱ� �Ǵ� ����)
    Ÿ��( ��, ��, �簢��)
    
    public:
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual BOOL Collision(..) = 0;
        // : �� �� �Լ� .. setposition
    };

    CCircle : public CObject
    {
    private:
        ������
    public:
        virtual void Update() override;
        virtual void Draw() override;
        virtual BOOL Collision(..) override;
    }

*/

#include "framework.h"
#include "WinAPI_Proj.h"
#include <cmath>

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

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    while (true)
    {   
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //Update();
            //Render();
        }
    }

    return (int)msg.wParam;
}

// 230629
void DrawUp(HDC hdc);
void DrawDown(HDC hdc);
void DrawRight(HDC hdc);
void DrawLeft(HDC hdc);

void DrawCircle(HDC hdc, POINT center, int r, BOOL bFlag);

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

#define timer_ID_1 11
#define timer_ID_2 12
const int circleRadius = 50;

double LengthPts(POINT pt1, POINT pt2)
{
    return (sqrt(
        (float)(pt2.x - pt1.x) * (pt2.x - pt1.x) +
        (float)(pt2.y - pt1.y) * (pt2.y - pt1.y)
    ));
}

BOOL InCircle(POINT pt1, POINT pt2)
{
    if (LengthPts(pt1, pt2) < circleRadius)return TRUE;
    return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
    HBRUSH hBrush, oldBrush;
    static int flag = 0; //  1: left, 2: right, 3: up, 4: down

    static POINT ptCurPos;
    static POINT ptMousePos;
    static RECT rectView;
    static bool bFlag;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        ptCurPos.x = circleRadius; ptCurPos.y = circleRadius;
       /* SetTimer(hWnd, timer_ID_1, 500, NULL);
        SetTimer(hWnd, timer_ID_2, 1000, NULL);*/
        bFlag = FALSE;
    
        break;

    case WM_TIMER:
        if (wParam == timer_ID_1)
        {
            ptCurPos.x -= 20;
            //ptCurPos.y += 20;
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        else if (wParam == timer_ID_2)
        {
            ptCurPos.y += 20;
        }

        break;

    case WM_KEYDOWN:
    {
        hdc = BeginPaint(hWnd, &ps);

        /*if (wParam == VK_LEFT)  flag = 1;
        else if (wParam == VK_RIGHT)  flag = 2;
        else if (wParam == VK_UP)  flag = 3;
        else if (wParam == VK_DOWN)  flag = 4;
        InvalidateRgn(hWnd, NULL, TRUE);*/

        if (wParam == VK_RIGHT)
        {
            ptCurPos.x += circleRadius;
            if (ptCurPos.x + circleRadius > rectView.right)
            {
                ptCurPos.x = rectView.right - circleRadius;
            }
        }
        else if (wParam == VK_LEFT)
        {
            ptCurPos.x -= circleRadius;
            if (ptCurPos.x - circleRadius < rectView.left)
            {
                ptCurPos.x = rectView.left + circleRadius;
            }
        }
        else if (wParam == VK_UP)
        {
            ptCurPos.y -= circleRadius;
            if (ptCurPos.y - circleRadius < rectView.top)
            {
                ptCurPos.y = rectView.top + circleRadius;
            }
        }
        else if (wParam == VK_DOWN)
        {
            ptCurPos.y += circleRadius;
            if (ptCurPos.y + circleRadius > rectView.bottom)
            {
                ptCurPos.y = rectView.bottom - circleRadius;
            }
        }
        bFlag = TRUE;
        InvalidateRgn(hWnd, NULL, TRUE);
        
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_KEYUP:
    {
        flag = 0;
        bFlag = FALSE;
        InvalidateRgn(hWnd, NULL, TRUE);
    }
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

    case WM_LBUTTONDOWN:
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);
        if (InCircle(ptMousePos, ptCurPos)) bFlag = true;
        InvalidateRgn(hWnd, NULL, TRUE);

        break;

    case WM_LBUTTONUP:
        bFlag = false;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_MOUSEMOVE:
        if (bFlag)
        {
            ptCurPos.x = LOWORD(lParam);
            ptCurPos.y = HIWORD(lParam);
            InvalidateRgn(hWnd, NULL, TRUE);
        }

        break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        /*DrawDown(hdc);
        DrawUp(hdc);
        DrawRight(hdc);
        DrawLeft(hdc);

        switch (flag)
        {
        case 1:
        {
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, 200, 200, 300, 300);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }
        break;
        case 2:
        {
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, 400, 200, 500, 300);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }
        break;
        case 3:
        {
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, 300, 100, 400, 200);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }
        break;
        case 4:
        {
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, 300, 400, 400, 300);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }
        break;
        }*/

        DrawCircle(hdc, ptCurPos, circleRadius, bFlag);
        /*if (bFlag)
            Rectangle(hdc, ptCurPos.x - circleRadius, ptCurPos.y - circleRadius, ptCurPos.x + circleRadius, ptCurPos.y + circleRadius);*/
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, timer_ID_1);

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

void DrawCircle(HDC hdc, POINT center, int r, BOOL bFlag)
{
    if (bFlag)
        SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

void DrawDown(HDC hdc)
{
    RECT rect;
    rect.left = 300; rect.top = 400; rect.right = 400; rect.bottom = 300;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("�Ʒ���"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawUp(HDC hdc)
{
    RECT rect;
    rect.left = 300; rect.top = 100; rect.right = 400; rect.bottom = 200;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("����"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawLeft(HDC hdc)
{
    RECT rect;
    rect.left = 200; rect.top = 200; rect.right = 300; rect.bottom = 300;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("����"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawRight(HDC hdc)
{
    RECT rect;
    rect.left = 400; rect.top = 200; rect.right = 500; rect.bottom = 300;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("������"), 3, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

