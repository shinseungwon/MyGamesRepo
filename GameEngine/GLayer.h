#pragma once
#include "GObject.h"
using namespace std;

#ifndef GLAYER
#define GLAYER

class GLayer {
public:
	int priority = -1;
	int width = 0;
	int height = 0;
	vector<GObject*>* objects;
	GLayer(int width, int height);
	void AddObject(GObject* gObject);
	void Draw(COLORREF* ca);
};

#endif