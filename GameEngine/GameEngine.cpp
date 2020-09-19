#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <future>

#include "Game.h"

#define WIDTH 512

using namespace std;

#pragma warning(disable:4996)

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("GameTestBoard");

static HINSTANCE hInst;
static HINSTANCE hPrevInst;
static LPWSTR lpCmdLn;
static int nCmdS;

HWND hWnd;
//unsigned char** pixels;

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Run(promise<string>* p);

int main(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	hInst = hInstance;
	hPrevInst = hPrevInstance;
	lpCmdLn = lpCmdLine;
	nCmdS = nCmdShow;

	wWinMain(hInst, hPrevInst, lpCmdLn, nCmdS);
}

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, 512 + 64, 512 + 64, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	promise<string> p;
	future<string> data = p.get_future();
	thread t(Run, &p);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void Run(promise<string>* p) {
	HDC hdc = GetDC(hWnd);

	COLORREF* arr = (COLORREF*)calloc(512 * 512, sizeof(COLORREF));
	HDC src = CreateCompatibleDC(hdc);
	HBITMAP map;
	//LPCWSTR path = TEXT("C:\\Users\\ssw90\\source\\repos\\MyGamesRepo\\GameEngine\\Debug\\test.bmp");
	//LPCWSTR path = TEXT("C:\\Download\\test3.bmp");	
	LPCWSTR path = TEXT("test.bmp");
	
	map = (HBITMAP)LoadImage(GetModuleHandle(NULL), path, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	
	
	//SelectObject(src, map);
	//BitBlt(hdc, 10, 10, 512, 512, src, 0, 0, SRCCOPY);

	//return;
	int r = 0, g = 0, b = 0;
	int x = 0, y = 0, z = 0;
	while (1) {

		//Sleep(1000);
		//for (int i = 0; i < 512; i++) {
		//	for (int j = 0; j < 512; j++) {				
		//		x = (i + j + r) % 256;
		//		y = (i + j + b) % 256;
		//		z = (i + j + b) % 256;
		//		arr[i* 512 + j] = RGB(x, y, z);
		//	}
		//}
		//map = CreateBitmap(512, 512, 1, 8 * 4, (void*)arr);

		SelectObject(src, map);
		BitBlt(hdc, 10 + r, 10 + g, 512, 512, src, 0, 0, SRCCOPY);
		r++;
		g++;
		b++;
		cout << "Drawing ..." << r << ' ' << g << ' ' << b << endl;
		Sleep(100);
	}

	DeleteDC(src);
	ReleaseDC(hWnd, hdc);
	p->set_value("End");
}