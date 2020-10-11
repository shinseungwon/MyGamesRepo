#include "stdafx.h"
#include "Game.h"

Game::Game(int bWidth, int bHeight) {
	shapes = new vector<BitmapPack*>();
	shapes = new vector<BitmapPack*>();

	layers = new vector<GLayer*>();
	board = new COLORREF[bWidth * bHeight];
	memset(board, 0, bWidth * bHeight * sizeof(COLORREF));
	this->bWidth = bWidth;
	this->bHeight = bHeight;
}

void Game::AddShape(string path) {
	BitmapPack* pack = GetBitmap(path);
	shapes->push_back(pack);
}

void Game::AddShape(BitmapPack* pack) {
	shapes->push_back(pack);
}

void Game::AddLayer(GLayer* gLayer) {
	gLayer->game = this;
	layers->push_back(gLayer);
}

void Game::Draw() {
	memset(board, 0, bWidth * bHeight * sizeof(COLORREF));
	for (int i = 0; i < layers->size(); i++) {
		GLayer* l = layers->at(i);
		l->Draw(board);
	}
}

Game::~Game() {
	int i;
	for (i = 0; i < shapes->size(); i++) {
		delete[] shapes->at(i)->data;
		delete shapes->at(i);
	}
	delete shapes;

	for (i = 0; i < layers->size(); i++) {
		delete layers->at(i);
	}
	delete layers;

	delete[] board;
}