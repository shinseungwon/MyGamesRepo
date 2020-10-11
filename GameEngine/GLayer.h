#pragma once
#include "GObject.h"
#include "Tools.h"
using namespace std;

#ifndef GLAYER
#define GLAYER

class Game;

class GLayer {
public:
	Game* game = nullptr;
	int priority = -1;
	int width = 0;
	int height = 0;	
	vector<GObject*>* objects;
	GLayer(int width, int height);
	~GLayer();
	void AddObject(GObject* gObject);
	void Draw(COLORREF* ca);
};

#endif