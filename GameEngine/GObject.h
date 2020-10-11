#pragma once
#include "Tools.h"
using namespace std;

#ifndef GOBJECT
#define GOBJECT

class Game;
class GLayer;

class GObject {
public:	
	Game* game = nullptr;
	GLayer* layer = nullptr;
	int currentShape = 0;
	vector<BitmapPack*>* shapes;
	vector<vector<pair<int, int>>*>* actions;
	int cAction = -1;
	int cSequence = 0;
	int cFrame = 0;
	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;
	GObject();
	~GObject();	
	void AddShape(int id);
	COLORREF* GetShape();
	void SetAction(int x);
	void SetShape(int id);
};

#endif