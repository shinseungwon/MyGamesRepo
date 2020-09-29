#pragma once
#include "GLayer.h"
using namespace std;

#ifndef GAME
#define GAME

class Game {
public:
	vector<GLayer*>* layers;
	COLORREF* board;
	int bWidth = 0;
	int bHeight = 0;
	Game(int bWidth, int bHeight);
	void AddLayer(GLayer* gLayer);
	void Draw();	
};

#endif