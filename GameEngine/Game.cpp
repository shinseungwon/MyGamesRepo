#include "stdafx.h"
#include "Game.h"

Game::Game(int bWidth, int bHeight) {
	layers = new vector<GLayer*>();
	board = new COLORREF[bWidth * bHeight];
	memset(board, 0, bWidth * bHeight * sizeof(COLORREF));
	this->bWidth = bWidth;
	this->bHeight = bHeight;
}

void Game::AddLayer(GLayer* gLayer) {
	layers->push_back(gLayer);
}

void Game::Draw() {	
	for (int i = 0; i < layers->size(); i++) {
		GLayer* l = layers->at(i);
		l->Draw(board);
	}
}