#include "stdafx.h"
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
Tetris* tetris;

char keys[128];

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Draw(promise<string>* p);
void Run(promise<string>* p);

void KeyDown(WPARAM wParam);
void KeyUp(WPARAM wParam);

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

	tetris = new Tetris(WIDTH, HEIGHT);
	tetris->Prepare();

	promise<string> p1;
	future<string> data1 = p1.get_future();
	thread t1(Draw, &p1);

	promise<string> p2;
	future<string> data2 = p2.get_future();
	thread t2(Run, &p2);

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

	case WM_KEYDOWN:		
		KeyDown(wParam);
		break;
	case WM_KEYUP:		
		KeyUp(wParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void Draw(promise<string>* p) {	
	HDC hdc = GetDC(hWnd);
	HDC src = CreateCompatibleDC(hdc);

	UINT f = 0;
	while (1) {
		tetris->game->Draw();
		COLORREF* board = tetris->game->board;

		if (board != nullptr) {
			SelectObject(src, CreateBitmap(WIDTH, HEIGHT, 1, 8 * 4, board));
			BitBlt(hdc, 0, 0, WIDTH, HEIGHT, src, 0, 0, SRCCOPY);
		}
		Sleep(100);
	}
	DeleteDC(src);
	ReleaseDC(hWnd, hdc);
	p->set_value("End");
}

void Run(promise<string>* p) {
	memset(keys, 0x00, sizeof(char) * 128);	

	UINT f = 0;
	while (1) {
		tetris->Run(f++);
		Sleep(1);		
	}
	cout << "Game End ..." << endl;
	p->set_value("End");
}

void KeyDown(WPARAM wParam) {
	if (keys[wParam] == 0) {
		keys[wParam] = 1;
		//printf("Key down 0x%x\n", (UINT)wParam);
		tetris->KeyDown(wParam);
	}
	else {
		//printf("Key pressing 0x%x\n", (UINT)wParam);
		tetris->KeyPressing(wParam);
	}
}

void KeyUp(WPARAM wParam) {
	keys[wParam] = 0;
	//printf("Key up 0x%x\n", (UINT)wParam);
	tetris->KeyUp(wParam);
}