/*
Q1. p93 문제 7
    - 키 down, up 상태 체크
    - 화면 갱신 이벤트 발생
    - 상태에 따라 사각형 4개 그려주기

Q2. 1. 클라이언트 영역에 마우스 클릭, 그 위치에 기본으로 원 생성
        1.1 랜덤으로 사각형, 원, 별이 생성되도록 한다.

    2. 임의의 방향으로 이동 / 클라이언트 외곽에서 반사
    3. 클래스로 구현
        사각형/ 별도 같은 식으로 작동하도록 한다.

    4. 다른 오브젝트와의 관계 설정
        모드 설정 1번 반사, 2번 합체, 3번 분열로 키 설정
        4.1 반사 - 다른 오브젝트와 부딪히면 튕기기
        4.2 합체 - 상성 관계에 따라 다른 오브젝트와 만나면 합체해서 커지게 하기
        4.3 분열 - 상성 관계에 따라 다른 오브젝트와 만나면 분열해서 작아지게 하기
            별 > 원 > 사각형 > 별
    5. 일정 크기 이상, 또는 이하는 사라지도록 한다.
    6. 각 오브젝트를 회전 시킨다.

    CObject
    {
    protected:
    좌표
    이동 스피드
    이동 방향(앵글 또는 벡터)
    타입( 원, 별, 사각형)
    
    public:
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual BOOL Collision(..) = 0;
        // : 그 외 함수 .. setposition
    };

    CCircle : public CObject
    {
    private:
        반지름
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

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
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
    DrawText(hdc, _T("아래쪽"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawUp(HDC hdc)
{
    RECT rect;
    rect.left = 300; rect.top = 100; rect.right = 400; rect.bottom = 200;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("위쪽"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawLeft(HDC hdc)
{
    RECT rect;
    rect.left = 200; rect.top = 200; rect.right = 300; rect.bottom = 300;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("왼쪽"), 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void DrawRight(HDC hdc)
{
    RECT rect;
    rect.left = 400; rect.top = 200; rect.right = 500; rect.bottom = 300;
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DrawText(hdc, _T("오른쪽"), 3, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

