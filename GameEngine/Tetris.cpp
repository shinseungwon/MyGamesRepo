#include "stdafx.h"
#include "Tetris.h"

BYTE** tMap = nullptr;
BYTE tmw = 10;
BYTE tmh = 20;

Tetris::Tetris(HWND hWnd, int width, int height) : Game(hWnd, width, height) {
	int i, j, t, bSize = width * height;
	COLORREF* arr;
	this->bgLayer = new GLayer(width, height);
	AddLayer(bgLayer);
	this->gLayer = new GLayer(width, height);
	AddLayer(gLayer);

	tMap = new BYTE * [tmh];
	for (i = 0; i < tmh; i++) {
		tMap[i] = new BYTE[tmw];
		memset(tMap[i], 0, tmw * sizeof(BYTE));
	}

	COLORREF* temp = new COLORREF[16];
	for (i = 0; i < 28; i++) {
		if (blocks[i] != nullptr) {
			t = i % 7;
			for (j = 0; j < 16; j++) {
				if (blocks[i][j] == 0) {
					temp[j] = WHITE;
				}
				else {
					temp[j] = (COLORREF)colors[t];
				}
			}
			AddShape(CreateBitmap(temp, 16, 4, 4, 16));
		}
		else {
			AddShape(nullptr);
		}
	}
	delete[] temp;

	this->downs = new GObject();
	this->gLayer->AddObject(downs);
	this->downs->width = 16 * tmw;
	this->downs->height = 16 * tmh;
	this->downs->x = 512 - tmw * 16 / 2;
	int dSize = this->downs->width * this->downs->height;
	arr = new COLORREF[dSize];
	//memset(arr, 0x00, size * sizeof(COLORREF));
	for (i = 0; i < dSize; i++) {
		arr[i] = WHITE;
	}
	AddShape(CreateBitmap(arr, dSize, this->downs->width, this->downs->height));
	this->downs->AddShape(28);
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

	for (i = 0; i < 28; i++) {
		this->current->object->AddShape(i);
		this->next->object->AddShape(i);
	}

	GObject* background = new GObject();
	background->width = width;
	background->height = height;
	bgLayer->AddObject(background);

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

	AddShape(CreateBitmap(arr, bSize, width, height));
	background->AddShape(29);
}

Tetris::~Tetris() {

}

void Tetris::Update() {
	int i, j, s = tmw * tmh;

	BitmapPack* downs = shapes->at(28);
	COLORREF* mapRef = new COLORREF[s];
	for (i = 0; i < tmh; i++) {
		for (j = 0; j < tmw; j++) {
			if (tMap[i][j] > 1) {
				mapRef[i * tmw + j] = colors[tMap[i][j] - 2];
			}
			else {
				mapRef[i * tmw + j] = WHITE;
			}
		}
	}
	WriteBitmap(mapRef, downs->data, s, tmw, tmh, 16);
	delete[] mapRef;
}

void Tetris::Erase() {
	int i, j;
	BYTE c = 0, k = tmh - 1;
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
		if (j < tmw) {
			memcpy(t[k--], tMap[i], tmw * sizeof(BYTE));
		}
		else {
			c++;
		}
	}

	for (i = 0; i < tmh; i++) {
		memcpy(tMap[i], t[i], tmw * sizeof(BYTE));
		delete[] t[i];
	}
	delete[] t;
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
				current->Mark();
				Erase();
				Update();
				current->Set(next->id);
				next->Set();
				if (current->Top() == 1) {
					state = 1;
				}
			}
		}
		else if (state == 1) {
			cout << "End ..." << endl;
		}
	}
}

void Tetris::KeyDown(WPARAM wParam) {
	if (keys[wParam] == 0) {
		keys[wParam] = 1;
		switch (wParam) {
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
				current->Mark();
				Erase();
				Update();
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
			current->Mark();
			Erase();
			Update();
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

Block::Block() {

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

Block::~Block() {
	object = nullptr;
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
	cout << mDist << endl;

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