// WindowsProject2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsProject2.h"

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

// >> :
void FileRead(HWND hWnd, LPTSTR filename);
void FileSave(HWND hWnd, LPTSTR filename);

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
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

#define IDC_EDIT_NOTEPAD 123 // << :

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit; // << :
    RECT rectView;

    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rectView);
        hEdit = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 100,
            rectView.right, rectView.bottom, hWnd, (HMENU)IDC_EDIT_NOTEPAD, hInst, NULL);
    }
    break;
    case WM_SIZE:
    {
        GetClientRect(hWnd, &rectView);
        MoveWindow(hEdit, 0, 0, rectView.right, rectView.bottom, TRUE);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_FILEOPEN:
            FileRead(hEdit, (LPTSTR)_T("test2.txt"));
            break;
        case ID_FILESAVE:
            FileSave(hEdit, (LPTSTR)_T("test2.txt"));
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
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
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

void FileRead(HWND hWnd, LPTSTR filename)
{
    HANDLE hFile;
    LPTSTR buffer;
    DWORD size, charNum;
    int fileSize;
    hFile = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    fileSize = GetFileSize(hFile, &size);
    buffer = new TCHAR[(int)fileSize / sizeof(TCHAR) + 1];
    memset(buffer, 0, fileSize);
    ReadFile(hFile, buffer, fileSize, &charNum, NULL);
    buffer[(int)fileSize / sizeof(TCHAR)] = NULL;
    SetWindowText(hWnd, buffer);
    delete[] buffer;
    CloseHandle(hFile);
}

void FileSave(HWND hWnd, LPTSTR filename)
{
    HANDLE hFile;
    LPTSTR buffer;
    int size;
#ifdef _UNICODE
    WORD uni = 0xfeff;
    DWORD nSize;
#endif
    hFile = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

#ifdef _UNICODE
    WriteFile(hFile, &uni, 2, &nSize, NULL);
#endif
    size = GetWindowTextLength(hWnd);
    buffer = new TCHAR[size + 1];
    memset(buffer, 0, (size + 1) * sizeof(TCHAR));
    size = GetWindowText(hWnd, (LPTSTR)buffer, size + 1);
    buffer[size] = NULL;
    WriteFile(hFile, buffer, size * sizeof(TCHAR), (LPDWORD)&size, NULL);
    delete[] buffer;
    CloseHandle(hFile);
}