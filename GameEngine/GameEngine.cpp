#include "stdafx.h"
#include "Game.h"

using namespace std;

#define WIDTH 512

#pragma warning(disable:4996)

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("GameTestBoard");

static HINSTANCE hInst;
static HINSTANCE hPrevInst;
static LPWSTR lpCmdLn;
static int nCmdS;

HWND hWnd;

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Run(promise<string>* p);
Game* SetGame();

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
	Game* game = SetGame();

	HDC hdc = GetDC(hWnd);
	HDC src = CreateCompatibleDC(hdc);
	HBITMAP map = nullptr;
	int f = 0;
	while (1) {		
		cout << "frame " << f++ << endl;		
		game->Draw();
		map = CreateBitmap(WIDTH, WIDTH, 1, 8 * 4, (void*)game->board);
		SelectObject(src, map);
		BitBlt(hdc, 0, 0, WIDTH, WIDTH, src, 0, 0, SRCCOPY);
		Sleep(100);		
	}

	DeleteDC(src);
	ReleaseDC(hWnd, hdc);
	p->set_value("End");
}

Game* SetGame() {
	Game* game = new Game(WIDTH, WIDTH);

	GLayer* bgLayer = new GLayer(WIDTH, WIDTH);

	GObject* background = new GObject();
	background->AddShape("bmps\\bg.bmp");
	bgLayer->AddObject(background);

	game->AddLayer(bgLayer);

	GLayer* objectLayer = new GLayer(WIDTH, WIDTH);

	GObject* obj1 = new GObject();
	obj1->AddShape("bmps\\test1.bmp");
	obj1->AddShape("bmps\\test2.bmp");
	obj1->x = 100;
	obj1->y = 100;

	GObject* obj2 = new GObject();
	obj2->AddShape("bmps\\test3.bmp");
	obj2->AddShape("bmps\\test4.bmp");
	obj2->x = 300;
	obj2->y = 300;

	vector<pair<int, int>>* action = new vector<pair<int, int>>();
	action->push_back(make_pair(0, 9));
	action->push_back(make_pair(1, 15));
	obj1->actions->push_back(action);
	obj2->actions->push_back(action);

	objectLayer->AddObject(obj1);
	objectLayer->AddObject(obj2);

	game->AddLayer(objectLayer);

	return game;
}