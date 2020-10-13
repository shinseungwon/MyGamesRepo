#pragma once
#include "GLayer.h"
#include "Tools.h"
using namespace std;

#ifndef GAME
#define GAME

#define BLACK 0x00000000
#define WHITE 0x00ffffff

#define RED 0x00ff0000
#define GREEN 0x0000ff00
#define BLUE 0x000000ff
#define YELLOW 0x00ffff00
#define CYAN 0x0000ffff
#define MAGENTA 0x00ff00ff

#define ORANGE 0x00ff7f00
#define PINK 0x00ff007f
#define ERIN 0x0000ff7f
#define LIME 0x007fff00
#define PURPLE 0x007f00ff
#define AZURE 0x00007fff

class Game {
public:
	HWND hWnd;

	vector<BitmapPack*>* shapes;
	vector<GLayer*>* layers;

	COLORREF* board;
	int bWidth = 0;
	int bHeight = 0;

	BYTE keys[128];	

	Game(HWND hWnd, int bWidth, int bHeight);

	void AddShape(string path);
	void AddShape(BitmapPack* pack);

	void AddLayer(GLayer* gLayer);
	void Draw();

	virtual void Run(UINT f);
	virtual void Prepare();
	virtual void KeyDown(WPARAM wParam);
	virtual void KeyUp(WPARAM wParam);	

	~Game();
};

void GameStart(Game* game, int w, int h);
void GameDraw(Game* game, promise<string>* p);
void GameRun(Game* game, promise<string>* p);
#endif