#include "stdafx.h"
#include "Game.h"

Game::Game(HWND hWnd, WORD bWidth, WORD bHeight) {
	this->hWnd = hWnd;
	shapes = new vector<BitmapPack*>();
	layers = new vector<GLayer*>();
	soundPlaying = new vector<BYTE>();
	soundLocs = new vector<LPCTSTR>();
	board = new COLORREF[(DWORD64)bWidth * bHeight];
	memset(board, 0, (DWORD64)bWidth * bHeight * sizeof(COLORREF));
	memset(keys, 0x00, 128 * sizeof(BYTE));
	this->bWidth = bWidth;
	this->bHeight = bHeight;

	int i, j;
	BitmapPack* b;
	COLORREF* a = new COLORREF[32];
	for (i = 0; i < 128; i++) {
		if (!empty(asciiChar[i])) {
			for (j = 0; j < 32; j++) {
				if (asciiChar[i][j] == 0) {
					a[j] = TRANSPARENT;
				}
				else {
					a[j] = WHITE;
				}
			}
			asciis[i] = CreateBitmap(a, 32, 4, 8, 4);
		}
		else {
			asciis[i] = nullptr;
		}
	}
}

void Game::AddSound(LPCTSTR loc) {
	soundCount++;
	soundPlaying->push_back(0);	
	soundLocs->push_back(loc);
}

void Game::RunSound(UINT x) {
	soundPlaying->at(x) = 1;
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
		if (!l->hide) {
			l->Draw(board);
		}
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

	promise<string> p3;
	future<string> data3 = p3.get_future();
	thread* t3 = new thread(GameSound, game, &p3);
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
		Sleep(game->waiting);
	}
	DeleteDC(src);
	ReleaseDC(game->hWnd, hdc);
	p->set_value("End");
}

void GameSound(Game* game, promise<string>* p) {
	UINT f = 0, i;
	for (i = 0; i < game->soundCount; i++) {
		LoadMp3(game->hWnd, game->soundLocs->at(i));
	}

	while (1) {
		for (i = 0; i < game->soundCount; i++) {
			if (game->soundPlaying->at(i) == 1) {
				PlayMp3(game->hWnd, i + 1);
				game->soundPlaying->at(i) = 0;
			}
		}
	}
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