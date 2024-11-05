//#include "stdafx.h"
//#include "rapidxml.hpp"
//#include <windows.h>
//#include <objidl.h>
//#include <gdiplus.h>
//#include <vector>
//#include <fstream>
//using namespace std;
//using namespace rapidxml;
//using namespace Gdiplus;
//#pragma comment (lib,"Gdiplus.lib")
#include "Group.h"
#include "resource.h"

#define MAX_LOADSTRING 100
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

float scale = 1.0f;
float rotateSVG = 0.0f;
float scrollX = 0.0f;
float scrollY = 0.0f;
float maxWidth = 0, maxHeight = 0;
string path = "sample.svg";
vector<Shape*> shapes;
List lst;
View vb;

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

/*VOID OnPaint(HDC hdc)
{
   // Ref: https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-getting-started-use
   Graphics graphics(hdc);
   Pen      pen(Color(255, 0, 0, 255));
   graphics.DrawLine(&pen, 0, 0, 200, 100);
}*/

VOID OnPaint(HDC hdc)
{
    Graphics grap(hdc);
    grap.ScaleTransform(scale, scale);
    grap.TranslateTransform(scrollX, scrollY);
    grap.RotateTransform(rotateSVG);

    for (auto* shape : shapes) {
        shape->draw(grap);
    }
}

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

ATOM MyRegisterClass(HINSTANCE hInstance) {
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
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ZOOM_IN:

        }

    }
    }
}

/*INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;

   // Read XML
   xml_document<> doc;
   xml_node<> *rootNode;
   // Read the xml file into a vector
   ifstream file("sample.svg");
   vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
   buffer.push_back('\0');
   // Parse the buffer using the xml file parsing library into doc 
   doc.parse<0>(&buffer[0]);

   rootNode = doc.first_node();
   xml_node<> *node = rootNode->first_node();

   while (node != NULL) {
	   char *nodeName = node->name();
	   xml_attribute<> *firstAttribute = node->first_attribute();
	   char *attributeName = firstAttribute->name();
	   char *attributeValue = firstAttribute->value();
	   xml_attribute<> *secondAttribute = firstAttribute->next_attribute();
	   // Set breakpoint here to view value
	   // Ref: http://rapidxml.sourceforge.net/manual.html
	   node = node->next_sibling();
   }

   
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("SVG Demo"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
      
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, INT iCmdShow) {
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SVGDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    shapes = readFile(path, maxWidth, maxHeight, lst, vb);

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

    for (Shape* shape : shapes)
        delete shape;

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_KEYDOWN:
       switch (wParam) {
            case 'I': zoomIn(hWnd); break;
            case 'O': zoomOut(hWnd); break;
            case 'R': ScrollRight(hWnd); break;
            case 'L': ScrollLeft(hWnd); break;
            case VK_UP: ScrollUp(hWnd); break;
            case VK_DOWN: ScrollDown(hWnd); break;
            case VK_LEFT: ScrollLeft(hWnd); break;
            case VK_RIGHT: ScrollRight(hWnd); break;
            case 'D': defaultScreen(hWnd); break;
       }
       return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
} // WndProc