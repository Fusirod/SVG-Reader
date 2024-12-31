#include "Group.h"
#include "resource.h"

using namespace std;

constexpr int MAXSTRING = 100;

HINSTANCE hInst;
WCHAR szTitle[MAXSTRING];
WCHAR szWindowClass[MAXSTRING];
float scale = 1;
float Rotate = 0;
float scrollX = 0;
float scrollY = 0;
float maxWidth = 0, maxHeight = 0;
string path = "23127094_23127134_23127200_23127246_3.svg";
//string path;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static void HandleMenuCommand(HWND hWnd, WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    case IDM_ZOOM_IN:
        scale *= 1.1f;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_ZOOM_OUT:
        scale *= 0.9f;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_DEFAULT:
        scale = 1;
        Rotate = 0;
        scrollX = 0;
        scrollY = 0;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_ROTATE_LEFT:
        Rotate -= 30;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_ROTATE_RIGHT:
        Rotate += 30;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_UP:
        scrollY -= 20;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_DOWN:
        scrollY += 20;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_LEFT:
        scrollX -= 20;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDM_RIGHT:
        scrollX += 20;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    default:
        break;
    }
}


static void HandleKeyDown(HWND hWnd, WPARAM wParam)
{
    switch (wParam)
    {
    case VK_UP:
        scrollY -= 20;
        break;
    case VK_DOWN:
        scrollY += 20;
        break;
    case VK_LEFT:
        scrollX -= 20;
        break;
    case VK_RIGHT:
        scrollX += 20;
        break;
    case 'I': case 'i':
        scale *= 1.1f;
        break;
    case 'O': case 'o':
        scale *= 0.9f;
        break;
    case 'R': case 'r':
        Rotate += 30;
        break;
    case 'L': case 'l':
        Rotate -= 30;
        break;
    case 'D': case 'd':
        scale = 1;
        Rotate = 0;
        scrollX = 0;
        scrollY = 0;
        break;
    default:
        return;
    }
    InvalidateRect(hWnd, NULL, TRUE);
}

static void HandleMouseWheel(HWND hWnd, WPARAM wParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    if (delta > 0)
        scale *= 1.1f;
    else
        scale *= 0.9f;

    InvalidateRect(hWnd, NULL, TRUE);
}

static void Redraw(HWND hWnd, vector<shapes*>& shapes, defs& def, viewBox& vb, float Rotate, float scrollX, float scrollY, float scale)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    Graphics graphics(hdc);

    graphics.ScaleTransform(static_cast<float>(ps.rcPaint.bottom) / vb.boxWidth,
        static_cast<float>(ps.rcPaint.bottom) / vb.boxHeight);
    graphics.TranslateTransform(vb.minX, vb.minY);
    transformImage(graphics, Rotate, maxWidth + scrollX, maxHeight + scrollY, scrollX, scrollY, scale);

    for (auto shape : shapes)
        shape->draw(graphics, def);

    EndPaint(hWnd, &ps);
}

static void CleanResources(vector<shapes*>& shapes, ULONG_PTR token)
{
    for (auto shape : shapes)
        delete shape;
    shapes.clear();
    GdiplusShutdown(token);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    bool isDragging = false;
    defs def;
    viewBox vb;
    vector<shapes*> shapes = readFile(path, maxWidth, maxHeight, def, vb);
    switch (message)
    {
    case WM_COMMAND:
        HandleMenuCommand(hWnd, wParam);
        break;
    case WM_KEYDOWN:
        HandleKeyDown(hWnd, wParam);
        break;
    case WM_MOUSEWHEEL:
        HandleMouseWheel(hWnd, wParam);
        break;
    case WM_PAINT:
        Redraw(hWnd, shapes, def, vb, Rotate, scrollX, scrollY, scale);
        break;
    case WM_DESTROY:
        CleanResources(shapes, 0); // 
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, INT iCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SVGDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Process command line arguments
    int argCount;
    LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
    if (argCount > 1)
    {
        // cmd args are UTF-16, convert to UTF-8
        wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
        path = converter.to_bytes(szArgList[1]);
    }

    /*if (argCount < 2) {
    MessageBox(nullptr, L"Please provide the SVG file path as a command-line argument.", L"Error", MB_OK | MB_ICONERROR);
    return 1;
    }
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    path = converter.to_bytes(szArgList[1]);*/
    
    LocalFree(szArgList);

    GdiplusStartupInput gdiplusStartupInput; //
    ULONG_PTR gdiplusToken; //
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr); // 

    // Perform application initialization:
    if (!InitInstance(hInstance, iCmdShow))
    {
        GdiplusShutdown(gdiplusToken);
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SVGDEMO));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken); //
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SVGDEMO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SVGDEMO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

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
