#include "stdafx.h"
#include "Tetris.h"

BYTE** tMap = nullptr;
BYTE tmw = 10;
BYTE tmh = 20;

Tetris::Tetris(HWND hWnd, WORD width, WORD height) : Game(hWnd, width, height) {
	DWORD i, j, t, bSize = width * height;
	BitmapPack* bp;
	COLORREF* arr;

	AddSound(L"tetris\\down.mp3");
	AddSound(L"tetris\\erase.mp3");

	this->bgLayer = new GLayer(width, height);
	AddLayer(bgLayer);
	this->gLayer = new GLayer(width, height);
	AddLayer(gLayer);
	this->cLayer = new GLayer(width, height);
	AddLayer(cLayer);

	tMap = new BYTE * [tmh];
	for (i = 0; i < tmh; i++) {
		tMap[i] = new BYTE[tmw];
		memset(tMap[i], 0, tmw * sizeof(BYTE));
	}

	arr = new COLORREF[16];
	for (i = 0; i < 28; i++) {
		if (blocks[i] != nullptr) {

			t = i % 7;
			for (j = 0; j < 16; j++) {
				if (blocks[i][j] == 0) {
					arr[j] = TRANSPARENT;
				}
				else {
					arr[j] = (COLORREF)colors[t];
				}
			}
			AddShape(CreateBitmap(arr, 16, 4, 4, 16));
		}
		else {
			AddShape(nullptr);
		}
	}
	delete[] arr;

	this->next = new Block();
	this->next->object = new GObject();
	this->next->object->width = 4 * 16;
	this->next->object->height = 4 * 16;
	this->gLayer->AddObject(this->next->object);
	this->next->x = -1;
	this->next->y = -1;
	this->next->object->x = 512 + 256 - 32;
	this->next->object->y = 128;

	this->current = new Block();
	this->current->object = new GObject();
	this->current->object->width = 4 * 16;
	this->current->object->height = 4 * 16;
	this->gLayer->AddObject(this->current->object);

	this->downs = new GObject();
	this->gLayer->AddObject(downs);
	this->downs->width = 16 * tmw;
	this->downs->height = 16 * tmh;
	this->downs->x = 512 - tmw * 16 / 2;
	int dSize = this->downs->width * this->downs->height;
	arr = new COLORREF[dSize];
	memset(arr, 0xff, dSize * sizeof(COLORREF));
	bp = CreateBitmap(arr, dSize, this->downs->width, this->downs->height);
	AddShape(bp);
	this->downs->AddShape(bp);
	delete[] arr;

	for (i = 0; i < 28; i++) {
		this->current->object->AddShape(i);
		this->next->object->AddShape(i);
	}

	background = new GObject();
	background->width = width;
	background->height = height;
	bgLayer->AddObject(background);

	//Background
	arr = new COLORREF[bSize];
	memset(arr, 0x00, bSize * sizeof(COLORREF));
	int ps = 512 - 16 * 5, pe = 512 + 16 * 5, pb = 16 * 20
		, pvsy = this->next->object->y, pvsx = this->next->object->x;
	for (i = 0; i < pb; i++) {
		for (j = ps; j < pe; j++) {
			if ((i / 16) % 2 + (j / 16) % 2 == 1) {
				arr[i * width + j] = 0x00333333;
			}
		}
	}

	for (i = pvsy; i < pvsy + 16 * 4; i++) {
		for (j = pvsx; j < pvsx + 16 * 4; j++) {
			if ((i / 16) % 2 + (j / 16) % 2 == 1) {
				arr[i * width + j] = 0x00333333;
			}
		}
	}

	for (i = 0; i < 16 * 21; i++) {
		for (j = ps - 16; j < ps; j++) {
			arr[i * width + j] = PINK;
		}
		for (j = pe; j < pe + 16; j++) {
			arr[i * width + j] = PINK;
		}
	}

	for (i = pb; i < pb + 16; i++) {
		for (j = ps; j < pe; j++) {
			arr[i * width + j] = PINK;
		}
	}

	bp = CreateBitmap(arr, bSize, width, height);
	AddShape(bp);
	background->AddShape(bp);
	delete[] arr;
	//~Background

	//score
	BYTE mass = 4;
	scoreBoard = new GObject * [8];
	GObject* letter;
	for (i = 0; i < 8; i++) {
		letter = new GObject();
		bgLayer->AddObject(letter);
		letter->y = 256 + 64 + 32;
		letter->x = 256 + 128 + 64 + i * 5 * mass;
		letter->width = 4 * mass;
		letter->height = 8 * mass;
		for (j = 0; j < 10; j++) {
			letter->AddShape(asciis[48 + j]);
		}
		letter->AddShape(asciis[32]);
		scoreBoard[i] = letter;
	}

	UINT num = 0;
	string numStr = to_string(num);

	for (i = 0; i < 8; i++) {
		scoreBoard[i]->SetShape(10);
	}
	scoreBoard[7]->SetShape(0);
	//score

	//Cover	
	cover = new GObject();
	cover->width = width;
	cover->height = height;
	cLayer->AddObject(cover);

	arr = new COLORREF[bSize];
	memset(arr, 0x00, bSize * sizeof(COLORREF));
	bp = CreateBitmap(arr, bSize, width, height);
	AddShape(bp);
	cover->AddShape(bp);
	delete[] arr;

	title = new GObject();
	title->y = 128;
	title->x = 256;
	title->width = 64 * 8;
	title->height = 8 * 8;

	int tCount = 8 * 64, r;
	arr = new COLORREF[tCount];
	for (i = 0; i < tCount; i++) {
		if (gameTitle[i] == 0) {
			arr[i] = TRANSPARENT;
		}
		else {
			r = (i % 64) / 8;
			arr[i] = r == 7 ? WHITE : colors[r];
		}
	}

	bp = CreateBitmap(arr, 64 * 8, 64, 8, 8);
	AddShape(bp);
	title->AddShape(bp);
	cLayer->AddObject(title);
	//~Cover

	//level
	levelText = new GObject * [4];
	for (i = 0; i < 4; i++) {
		letter = new GObject();
		gLayer->AddObject(letter);
		letter->y = 256 - 32;
		letter->x = 512 + 256 - 32 + i * 5 * mass;
		letter->width = 4 * mass;
		letter->height = 8 * mass;
		levelText[i] = letter;
	}
	levelText[0]->AddShape(asciis[108]);
	levelText[1]->AddShape(asciis[118]);
	levelText[2]->AddShape(asciis[32]);

	for (i = 0; i < 10; i++) {
		levelText[3]->AddShape(asciis[48 + i]);
	}
	//~level

	//sub
	sub = new GObject * [16];
	for (i = 0; i < 16; i++) {
		letter = new GObject();
		cLayer->AddObject(letter);
		letter->y = 512 - 128;
		letter->x = 256 + 64 + 32 + i * 5 * mass;
		letter->width = 4 * mass;
		letter->height = 8 * mass;
		for (j = 0; j < 128; j++) {
			letter->AddShape(asciis[j]);
		}
		sub[i] = letter;
	}
	//~sub

	CoverTitle();
}

Tetris::~Tetris() {

}

void Tetris::UpdateDowns() {
	int i, j, s = tmw * tmh;
	BitmapPack* downs = shapes->at(28);
	COLORREF* mapRef = new COLORREF[s];

	for (i = 0; i < tmh; i++) {
		for (j = 0; j < tmw; j++) {
			if (tMap[i][j] > 1) {
				mapRef[i * tmw + j] = colors[tMap[i][j] - 2];
			}
			else {
				mapRef[i * tmw + j] = TRANSPARENT;
			}
		}
	}
	WriteBitmap(mapRef, downs->data, s, tmw, tmh, 16);
	delete[] mapRef;
}

void Tetris::UpdateScores(BYTE c) {
	int i;
	score += ls[c] * (level + 1);
	string numStr = to_string(score);

	for (i = 0; i < 8 - numStr.size(); i++) {
		scoreBoard[i]->SetShape(10);
	}

	for (i = 8 - numStr.size(); i < 8; i++) {
		scoreBoard[i]->SetShape(numStr[i - (8 - numStr.size())] - 0x30);
	}
}

BYTE Tetris::Erase() {
	int i, j, k, lp, sp;
	BYTE erased[4];
	memset(erased, 0x00, 4 * sizeof(BYTE));
	BYTE x = tmh - 1, c = 0;
	BYTE** t = new BYTE * [tmh];
	for (i = 0; i < tmh; i++) {
		t[i] = new BYTE[tmw];
		memset(t[i], 0x00, tmw * sizeof(BYTE));
	}

	for (i = tmh - 1; i >= 0; i--) {
		for (j = 0; j < tmw; j++) {
			if (tMap[i][j] < 2) {
				break;
			}
		}
		if (j < tmw) { //Áß°£¿¡ ³ª¿È -> ºóÄ­ ÀÖÀ½
			memcpy(t[x--], tMap[i], tmw * sizeof(BYTE));
		}
		else { //Áß°£¿¡ ¾È³ª¿È ->ºóÄ­ ¾øÀ½ -> ¶óÀÎ ¼ö
			erased[c] = i;
			c++;
		}
	}

	if (c > 0) {
		lockKeys = 1;
		RunSound(1);
		COLORREF* data = downs->shapes->at(0)->data;
		lp = 16 * 16 * tmw;
		for (i = 0; i < 10; i++) {
			for (j = 0; j < c; j++) {
				sp = 16 * tmw * erased[j] * 16;
				for (k = sp; k < sp + lp; k++) {
					if (i % 2 == 0) {
						data[k] = WHITE;
					}
					else {
						data[k] = BLACK;
					}
				}
			}
			Sleep(100);
		}
		lockKeys = 0;
	}

	for (i = 0; i < tmh; i++) {
		memcpy(tMap[i], t[i], tmw * sizeof(BYTE));
		delete[] t[i];
	}
	delete[] t;

	return c;
}

void Tetris::Prepare() {
	next->Set();
	current->Set();
	current->Top();
}

void Tetris::Run(UINT f) {
	cf = f;
	if (cf % fr == 0) {

		//system("cls");
		//for (int i = 0; i < tmh; i++) {
		//	for (int j = 0; j < tmw; j++) {
		//		printf("%d ", tMap[i][j]);
		//	}
		//	printf("\n");
		//}
		//printf("\n");

		if (state == 0) {
			if (current->Down() == 1) {
				RunSound(0);
				current->Mark();
				UpdateScores(Erase());
				UpdateDowns();
				current->Set(next->id);
				next->Set();
				if (current->Top() == 1) {
					state = 1;
				}
			}
		}
		else if (state == 1) {
			CoverGameOver();
			cLayer->hide = 0;
		}
		else if (state == 2) {

		}
		else if (state == 3) {

		}
		else {

		}
	}
}

void Tetris::KeyDown(WPARAM wParam) {
	if (!lockKeys && keys[wParam] == 0) {
		keys[wParam] = 1;
		switch (wParam) {
		case 0x10://shift
			if (state == 0 && level < 9) {
				level++;
				fr = lfr[level];
				RefreshLevel();
			}
			break;
		case 0x11://ctrl
			if (state == 0 && level > 0) {
				level--;
				fr = lfr[level];
				RefreshLevel();
			}
			break;
		case 0x0d://enter
			if (state == 1) {
				Reset();
				cLayer->hide = 1;
				state = 0;
			}
			else if (state == 3) {
				cLayer->hide = 1;
				state = 0;
			}
			break;
		case 0x1b://esc			
			if (state == 0) {
				CoverPause();
				state = 2;
				cLayer->hide = 0;
			}
			else if (state == 2) {
				state = 0;
				cLayer->hide = 1;
			}
			break;
		case 0x20://space						
			current->Drop();
			break;
		case 0x25://l
			current->Left();
			break;
		case 0x26://u	
			current->Rotate();
			break;
		case 0x27://r
			current->Right();
			break;
		case 0x28://d			
			if (state == 0 && current->Down() == 1) {
				RunSound(0);
				current->Mark();
				UpdateScores(Erase());
				UpdateDowns();
				current->Set(next->id);
				next->Set();
				if (current->Top() == 1) {
					state = 1;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
		KeyPressing(wParam);
	}
}

void Tetris::KeyUp(WPARAM wParam) {
	if (!lockKeys) {
		keys[wParam] = 0;
		switch (wParam) {
		case 0x20://space
			break;
		case 0x25://l
			break;
		case 0x26://u
			break;
		case 0x27://r
			break;
		case 0x28://d
			break;
		default:
			break;
		}
	}	
}

void Tetris::KeyPressing(WPARAM wParam) {
	switch (wParam) {
	case 0x20://space
		break;
	case 0x25://l
		current->Left();
		break;
	case 0x26://u
		break;
	case 0x27://r
		current->Right();
		break;
	case 0x28://d
		if (state == 0 && current->Down() == 1) {
			RunSound(0);
			current->Mark();
			UpdateScores(Erase());
			UpdateDowns();
			current->Set(next->id);
			next->Set();

			if (current->Top() == 1) {
				state = 1;
			}
		}
		break;
	default:
		break;
	}
}

void Tetris::CoverTitle() {
	//  Press  Enter  
	const char* arr = "  Press  Enter  ";
	for (int i = 0; i < 16; i++) {
		sub[i]->SetShape(arr[i]);
	}
}

void Tetris::CoverPause() {
	//     Paused     
	const char* arr = "     Paused     ";
	for (int i = 0; i < 16; i++) {
		sub[i]->SetShape(arr[i]);
	}
}

void Tetris::CoverGameOver() {
	//   Game  Over   
	const char* arr = "   Game  Over   ";
	for (int i = 0; i < 16; i++) {
		sub[i]->SetShape(arr[i]);
	}
}

void Tetris::RefreshLevel() {
	levelText[3]->SetShape(level);
}

void Tetris::Reset() {
	level = 0;
	fr = 50;
	score = 0;
	for (int i = 0; i < tmh; i++) {
		memset(tMap[i], 0x00, tmw * sizeof(BYTE));
	}
	UpdateDowns();
	Prepare();
}

Block::Block() {

}

Block::~Block() {
	object = nullptr;
}

void Block::Set(BYTE id) {

	if (id == 7) {
		this->id = rand() % 7;
	}
	else {
		this->id = id;
	}

	this->cs = 0;
	this->object->SetShape(this->id);
}

void Block::Update() {
	BYTE i, j, k, a, b;
	const BYTE* block = blocks[cs * 7 + id];

	for (i = 0; i < tmh; i++) {
		for (j = 0; j < tmw; j++) {
			if (tMap[i][j] == 1) {
				tMap[i][j] = 0;
			}
		}
	}

	for (i = 0; i < 4; i++) {
		a = y + i;
		for (j = 0; j < 4; j++) {
			b = x + j;
			k = i * 4 + j;
			if (block[k] == 1) {
				tMap[a][b] = 1;
			}
		}
	}
}

void Block::Mark() {
	int i, j, a, b;
	const BYTE* block = blocks[cs * 7 + id];
	for (i = 0; i < 4; i++) {
		a = y + i;
		for (j = 0; j < 4; j++) {
			b = x + j;
			if (block[i * 4 + j] == 1) {
				tMap[a][b] = id + 2;
			}
		}
	}
}

BYTE Block::Top() {
	x = 3;
	y = 0;

	int i, j, k, a, b;
	const BYTE* block = blocks[cs * 7 + id];
	for (i = 0; i < 4; i++) {
		a = y + i;
		for (j = 0; j < 4; j++) {
			b = x + j;
			k = i * 4 + j;
			if (block[k] == 1) {
				if (tMap[a][b] > 1) {
					return 1;
				}
			}
		}
	}

	Update();
	object->x = 512 - 32;
	object->y = 0;
	return 0;
}

BYTE Block::Rotate() {
	int i, j, a, b, cl, cr, d;
	BYTE ncs = (cs + 1) % Sc(), csw = bws[cs * 7 + id], ncsw = bws[ncs * 7 + id];

	d = ncsw - csw;
	if (d > 0) {
		cl = CLeft();
		cr = CRight();
		if (cl + cr >= d) {
			for (i = 0; i < d - cr; i++) {
				Left();
			}
		}
		else {
			return 1;
		}
	}

	cs = ncs;
	Update();
	object->SetShape(cs * 7 + id);
	return 0;
}

BYTE Block::CLeft() {
	if (x <= 0) {
		return 0;
	}
	int i, j, k, a, b, m = tmw;
	const BYTE* block = blocks[cs * 7 + id];
	for (i = 0; i < Ht(); i++) {
		a = y + i;
		for (j = 0; j < Wt(); j++) {
			if (block[i * 4 + j] == 1) {
				b = x + j;

				for (k = 1; k <= b; k++) {
					if (tMap[a][b - k] > 1) {
						break;
					}
				}

				if (k - 1 < m) {
					m = k - 1;
				}
			}
		}
	}
	return m;
}

BYTE Block::CRight() {
	if (x + Wt() >= tmw) {
		return 0;
	}
	int i, j, k, a, b, m = tmw;
	const BYTE* block = blocks[cs * 7 + id];
	for (i = 0; i < Ht(); i++) {
		a = y + i;
		for (j = Wt() - 1; j >= 0; j--) {
			if (block[i * 4 + j] == 1) {
				b = x + j;

				for (k = 1; k < tmw - b; k++) {
					if (tMap[a][b + k] > 1) {
						break;
					}
				}

				if (k - 1 < m) {
					m = k - 1;
				}
			}
		}
	}
	return m;
}

BYTE Block::Down() {
	int i, j, k, a, b;
	const BYTE* block = blocks[cs * 7 + id];
	for (i = 0; i < 4; i++) {
		if (y + Ht() >= tmh) {
			return 1;
		}
		a = y + i;
		for (j = 0; j < 4; j++) {
			b = x + j;
			if (block[i * 4 + j] == 1 && tMap[a + 1][b] > 1) {
				return 1;
			}
		}
	}

	y++;
	Update();
	object->y += 16;
	return 0;
}

BYTE Block::Drop() {
	int i, j, k, a, b, dist = 0, mDist = tmh;
	const BYTE* block = blocks[cs * 7 + id];

	for (i = x; i < x + Wt(); i++) {
		a = i - x;
		for (j = y + Ht() - 1; j >= y; j--) {
			b = j - y;
			if (block[b * 4 + a] == 1) {
				for (k = y + Ht(); k < tmh; k++) {
					if (tMap[k][i] > 1) {
						break;
					}
				}
				dist = k - j - 1;
				if (dist < mDist) {
					mDist = dist;
				}
				break;
			}
		}
	}

	if (mDist > 0) {
		y += mDist;
		Update();
		object->y += 16 * mDist;
		return 0;
	}
	else {
		return 1;
	}
}

BYTE Block::Left() {
	if (CLeft() == 0) {
		return 1;
	}
	x--;
	Update();
	object->x -= 16;
	return 0;
}

BYTE Block::Right() {
	if (CRight() == 0) {
		return 1;
	}
	x++;
	Update();
	object->x += 16;
	return 0;
}