#include "stdafx.h"
#include "Game.h"
#include "GLayer.h"

GLayer::GLayer(int width, int height) {		
	this->width = width;
	this->height = height;
	objects = new vector<GObject*>();
}

void GLayer::AddObject(GObject* gObject) {
	gObject->game = game;
	gObject->layer = this;
	objects->push_back(gObject);
}

void GLayer::Draw(COLORREF* board) {
	int i, j, k;
	for (i = 0; i < objects->size(); i++) {		
		GObject* o = objects->at(i);
		COLORREF* c = o->GetShape();
		int sb = o->y * width + o->x;
		int sc = 0;
		for (j = 0; j < o->height; j++) {
			for (k = 0; k < o->width; k++) {
				if (c[sc + k] != TRANSPARENT) {
					board[sb + k] = c[sc + k];
				}				
			}
			sb += width;
			sc += o->width;
		}
	}
}

GLayer::~GLayer() {
	delete objects;
}