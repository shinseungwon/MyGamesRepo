#pragma once
#include "Tools.h"
#include "Game.h"
using namespace std;

#ifndef TETRIS
#define TETRIS

const COLORREF colors[7] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, ORANGE };

const BYTE blocks[28][16] = {
							 { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

							,{ 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 }
							,{ 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 }

							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{}//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
							,{ 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
							,{ 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
};

const BYTE scs[7] = { 2, 2, 4, 1, 2, 4, 4 };

const BYTE bws[28] = { 3, 3, 3, 2, 4, 3, 3, 2, 2, 2, 0, 1, 2, 2, 0, 0, 3, 0, 0, 3, 3, 0, 0, 2, 0, 0, 2, 2 };
const BYTE bhs[28] = { 2, 2, 2, 2, 1, 2, 2, 3, 3, 3, 0, 4, 3, 3, 0, 0, 2, 0, 0, 2, 2, 0, 0, 3, 0, 0, 3, 3 };

const BYTE lfr[10] = { 50, 45, 40, 35, 30, 25, 20, 15, 10, 5 };
const USHORT ls[5] = { 0, 50, 100, 300, 1200 };

class Block {
public:
	BYTE id = 0;
	GObject* object = nullptr;
	
	BYTE cs = 0; //current shape
	BYTE x = 0;
	BYTE y = 0;

	Block();

	void Set(BYTE id = 7);
	void Update();
	void Mark();

	inline BYTE Wt() { return bws[cs * 7 + id]; }
	inline BYTE Ht() { return bhs[cs * 7 + id]; }
	inline BYTE Sc() { return scs[id]; }

	BYTE Top();
	BYTE Rotate();

	BYTE CLeft();
	BYTE CRight();

	BYTE Down();	
	BYTE Left();
	BYTE Right();

	BYTE Drop();

	~Block();
};

class Tetris : public Game {
public:
	UINT cf = 0;
	BYTE fr = 50;
	BYTE level = 0;
	UINT score = 0;
	GObject** scoreBoard;
	
	GLayer* bgLayer;
	GLayer* gLayer;
	GObject* downs;

	BYTE state = 0;

	Block* current = nullptr;
	Block* next = nullptr;

	Tetris(HWND hWnd, int width, int height);

	void Update(BYTE c);
	BYTE Erase();

	void Prepare();	
	void Run(UINT f);

	void KeyDown(WPARAM wParam);
	void KeyUp(WPARAM wParam);
	void KeyPressing(WPARAM wParam);

	~Tetris();
};

#endif