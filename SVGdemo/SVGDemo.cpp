#include "Group.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define _WIN32_WINNT 0x0600

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

float scale = 1.0f;
float rotateSVG = 0.0f;
float scrollX = 0.0f;
float scrollY = 0.0f;
float maxWidth = 0, maxHeight = 0;
string path = "sample.svg";

void zoomIn(HWND hWnd);
void zoomOut(HWND hWnd);
void RotateClockWise(HWND hWnd);
void RotateCounterClockWise(HWND hWnd);
void ScrollUp(HWND hWnd);
void ScrollDown(HWND hWnd);
void ScrollLeft(HWND hWnd);
void ScrollRight(HWND hWnd);
void defaultScreen(HWND hWnd);

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void zoomIn(HWND hWnd) {
    scale *= 1.1f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void zoomOut(HWND hWnd) {
    scale *= 0.9f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void RotateClockWise(HWND hWnd) {
    rotateSVG += 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void RotateCounterClockWise(HWND hWnd) {
    rotateSVG -= 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void ScrollUp(HWND hWnd) {
    scrollY -= 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void ScrollDown(HWND hWnd) {
    scrollY += 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void ScrollLeft(HWND hWnd) {
    scrollX -= 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void ScrollRight(HWND hWnd) {
    scrollX += 10.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

void defaultScreen(HWND hWnd) {
    scale = 1.0f;
    rotateSVG = 0.0f;
    scrollX = 0.0f;
    scrollY = 0.0f;
    InvalidateRect(hWnd, NULL, TRUE);
}

ATOM MyRegisterClass(HINSTANCE hInstance) { //
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SVGDEMO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SVGDEMO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR token;
    GdiplusStartup(&token, &gdiplusStartupInput, nullptr);
    bool is_dragging = false;
    POINT last_mouse_position;
    List lst;
    View v;
    vector<shape*> shapes = readFile(path, maxWidth, maxHeight, lst, v);
    switch (message)
    {
    case WM_COMMAND: 
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ZOOM_IN:
            zoomIn(hWnd);
            break;
        case IDM_ZOOM_OUT:
            zoomOut(hWnd);
            break;
        case IDM_DEFAULT:
            defaultScreen(hWnd);
            break;
        case IDM_ROTATE_LEFT:
            RotateCounterClockWise(hWnd);
            break;
        case IDM_ROTATE_RIGHT:
            RotateClockWise(hWnd);
            break;
        case IDM_UP:
            ScrollUp(hWnd);
            break;
        case IDM_DOWN:
            ScrollDown(hWnd);
            break;
        case IDM_RIGHT:
            ScrollRight(hWnd);
            break;
        case IDM_LEFT:
            ScrollLeft(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_KEYDOWN:
    {
        switch (lParam)
        {
        case VK_UP:
            ScrollUp(hWnd);
            break;
        case VK_DOWN:
            ScrollDown(hWnd);
            break;
        case VK_LEFT:
            ScrollLeft(hWnd);
            break;
        case VK_RIGHT:
            ScrollRight(hWnd);
            break;
        case 'i': case 'I':
            zoomIn(hWnd);
            break;
        case 'o': case 'O':
            zoomOut(hWnd);
            break;
        case 'r': case 'R':
            RotateClockWise(hWnd);
            break;
        case 'l': case 'L':
            RotateCounterClockWise(hWnd);
            break;
        case 'd': case 'D':
            defaultScreen(hWnd);
            break;
        }
    }

    case WM_MOUSEWHEEL:
    {
        short del = GET_WHEEL_DELTA_WPARAM(wParam);
        if (del > 0)
            zoomIn(hWnd);
        else
            zoomOut(hWnd);
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics gra(hdc);
        gra.ScaleTransform(static_cast<float>(ps.rcPaint.bottom) / v.widthView, static_cast<float>(ps.rcPaint.bottom) / v.heightView);
        gra.TranslateTransform(v.minX, v.minY);
        image(gra, rotateSVG, maxWidth + scrollX, maxHeight + scrollY, scrollX, scrollY, scale);
        for (int i = 0; i < shapes.size(); i++) {
            shapes[i]->draw(gra, lst);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    for (int i = 0; i < shapes.size(); i++)
        delete shapes[i];

    GdiplusShutdown(token);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, INT iCmdShow) {
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SVGDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    int argumentCnt;
    LPWSTR* szAgrumentList = CommandLineToArgvW(GetCommandLineW(), &argumentCnt);
    if (argumentCnt > 1)
    {
        // cmd args are UTF-16, convert to UTF-8
        wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
        path = converter.to_bytes(szAgrumentList[1]);
    }
    LocalFree(szAgrumentList);

    if (!InitInstance(hInstance, iCmdShow))
        return FALSE;

    HACCEL  hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SVGDEMO));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { //
    UNREFERENCED_PARAMETER(lParam);
    switch (message) 
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

