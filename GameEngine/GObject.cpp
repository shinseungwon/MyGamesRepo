#include "stdafx.h"
#include "GObject.h"
#include "Game.h"
#include "GLayer.h"

GObject::GObject() {	
	shapes = new vector<BitmapPack*>();
	actions = new vector<vector<pair<int, int>>*>();
}

void GObject::AddShape(int id) {
	shapes->push_back(game->shapes->at(id));
}

void GObject::AddShape(BitmapPack* pack) {
	shapes->push_back(pack);
}

void GObject::SetAction(int x) {
	if (x < actions->size() && x >= 0) {
		cAction = x;
	}
	else {
		cAction = 0;
	}
}

void GObject::SetShape(int id) {
	currentShape = id;
}

COLORREF* GObject::GetShape() {

	if (cAction == -1 || actions->size() == 0 ) {		
		return shapes->at(currentShape)->data;
	}
	else {		
		vector<pair<int, int>>* action = actions->at(cAction);

		int mSequence = action->size();
		pair<int, int> sequence = action->at(cSequence);

		int mFrame = sequence.second;
		COLORREF* shape = shapes->at(sequence.first)->data;

		if (cFrame == mFrame - 1) {
			cFrame = 0;
			if (cSequence == mSequence - 1) {
				cSequence = 0;
				cAction = -1;
			}
			else {
				cSequence++;
			}
		}
		else {
			cFrame++;
		}

		return shape;
	}	
}

GObject::~GObject() {
	delete shapes;
}