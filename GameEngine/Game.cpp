#include "stdafx.h"
#include "Game.h"

Game::Game(HWND hWnd, int bWidth, int bHeight) {
	this->hWnd = hWnd;

	shapes = new vector<BitmapPack*>();
	shapes = new vector<BitmapPack*>();

	layers = new vector<GLayer*>();
	board = new COLORREF[bWidth * bHeight];
	memset(board, 0, bWidth * bHeight * sizeof(COLORREF));
	memset(keys, 0x00, 128 * sizeof(BYTE));
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

void Game::Run(UINT f) {
	cout << "Game::Run" << endl;
}

void Game::Prepare() {
	cout << "Game::Prepare" << endl;
}

void Game::KeyDown(WPARAM wParam) {
	cout << "Game::KeyDown" << endl;
}

void Game::KeyUp(WPARAM wParam) {
	cout << "Game::KeyUp" << endl;
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

void GameStart(Game* game, int w, int h) {
	promise<string> p1;
	future<string> data1 = p1.get_future();	
	thread* t1 = new thread(GameRun, game, &p1);

	promise<string> p2;
	future<string> data2 = p2.get_future();	
	thread* t2 = new thread(GameDraw, game, &p2);
}

void GameDraw(Game* game, promise<string>* p) {
	HDC hdc = GetDC(game->hWnd);
	HDC src = CreateCompatibleDC(hdc);

	UINT f = 0;
	while (1) {
		game->Draw();
		COLORREF* board = game->board;

		if (board != nullptr) {
			SelectObject(src, CreateBitmap(game->bWidth, game->bHeight, 1, 8 * 4, board));
			BitBlt(hdc, 0, 0, game->bWidth, game->bHeight, src, 0, 0, SRCCOPY);
		}
		Sleep(100);
	}
	DeleteDC(src);
	ReleaseDC(game->hWnd, hdc);
	p->set_value("End");
}

void GameRun(Game* game, promise<string>* p) {
	UINT f = 0;
	while (1) {
		game->Run(f++);
		Sleep(1);
	}
	cout << "Game End ..." << endl;
	p->set_value("End");
}