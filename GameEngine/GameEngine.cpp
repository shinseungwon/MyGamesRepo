﻿#include "stdafx.h"
#include "Tetris.h"

#pragma comment(lib,"winmm.lib")

using namespace std;

#define WIDTH 1024
#define HEIGHT 512

#pragma warning(disable:4996)

static TCHAR windowClass[] = _T("DesktopApp");
static TCHAR title[] = _T("GameTestBoard");

static HINSTANCE hInst;
static HINSTANCE hPrevInst;
static LPWSTR lpCmdLn;
static int nCmdS;

HWND hWnd;
Game* myGame;

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int main(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine)
{
	hInst = hInstance;
	hPrevInst = hPrevInstance;
	lpCmdLn = lpCmdLine;
	nCmdS = 1;

	srand(static_cast<unsigned int>(time(NULL)));
	wWinMain(hInst, hPrevInst, lpCmdLn, nCmdS);

	return 0;
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
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	hWnd = CreateWindow(windowClass, title, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH + 64, HEIGHT + 64, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdS);
	UpdateWindow(hWnd);

	//Start game
	myGame = new Tetris(hWnd, WIDTH, HEIGHT);
	myGame->Prepare();
	GameStart(myGame, WIDTH, HEIGHT);
	//~Start game

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

		//Game event
	case WM_KEYDOWN:
		myGame->KeyDown(wParam);
		break;
	case WM_KEYUP:
		myGame->KeyUp(wParam);
		break;
		//~Game event

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}