#pragma once
#include "GLayer.h"
#include "Tools.h"
using namespace std;

#ifndef GAME
#define GAME

class Game {
public:
	vector<BitmapPack*>* shapes;

	vector<GLayer*>* layers;
	COLORREF* board;

	int bWidth = 0;
	int bHeight = 0;
	Game(int bWidth, int bHeight);

	void AddShape(string path);
	void AddShape(BitmapPack* pack);

	void AddLayer(GLayer* gLayer);
	void Draw();	

	~Game();
};

#endif