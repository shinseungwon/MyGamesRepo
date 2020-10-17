#include "stdafx.h"
#include "Tools.h"

BitmapPack::BitmapPack() {
	this->size = 0;
	this->width = 0;
	this->height = 0;
	this->data = nullptr;
}

BitmapPack::BitmapPack(int s, int w, int h, COLORREF* d) {
	this->size = s;
	this->width = w;
	this->height = h;
	this->data = d;
}

BitmapPack* GetBitmap(string path) {
	BitmapPack* res = new BitmapPack();
	int i = 0, j = 0;
	BYTE* arr = nullptr;

	ifstream ifstr(path, ios::binary);

	if (ifstr.is_open()) {
		vector<char> vc(istreambuf_iterator<char>(ifstr), (istreambuf_iterator<char>()));
		if (vc[0] == 0x42 && vc[1] == 0x4D) {

			int headerSize;
			int width;
			int height;
			short bitCount;

			memcpy(&headerSize, &vc[10], sizeof(int));
			memcpy(&width, &vc[18], sizeof(int));
			memcpy(&height, &vc[22], sizeof(int));
			memcpy(&bitCount, &vc[28], sizeof(short));

			int resultSize = (vc.size() - headerSize) / 3 * 4;
			arr = new BYTE[resultSize];

			if (bitCount == 24) {
				for (i = headerSize; i < vc.size(); i += 3) {
					arr[j++] = vc[i];
					arr[j++] = vc[i + 1];
					arr[j++] = vc[i + 2];
					arr[j++] = char(0);
				}

				res->size = resultSize / 4;
				res->width = width;
				res->height = height;
				res->data = (COLORREF*)arr;
			}
			else {
				cout << "file is not 24bit bitmap" << endl;
			}
		}
		else {
			cout << "file is not bitmap" << endl;
		}

		ifstr.close();
	}
	else {
		cout << "file not exist" << endl;
	}

	return res;
}

DWORD LoadMp3(HWND& hWnd, LPCWSTR path)
{
	MCI_OPEN_PARMS mciOpen;
	mciOpen.lpstrDeviceType = L"MPEGVideo";
	mciOpen.lpstrElementName = path;	
	DWORD res = mciSendCommand(1, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD64)(LPVOID)&mciOpen);
	char* errorStr = new char[128];
	mciGetErrorStringA(res, errorStr, 128);
	cout << "Load error : " << res << "(" << errorStr << ")" << endl;
	return res;
}

void PlayMp3(HWND& hWnd, int id) {
	MCI_PLAY_PARMS mciPlay;
	//mciPlay.dwCallback = (DWORD)hWnd;
	mciSendCommand(id, MCI_SEEK, MCI_SEEK_TO_START, (DWORD64)(LPVOID)NULL);
	mciSendCommand(id, MCI_PLAY, MCI_NOTIFY, (DWORD64)(LPVOID)&mciPlay);
}

void WriteBitmap(COLORREF* a, COLORREF* b, int size, int width, int height, int m) {
	memset(b, 0x00, (size_t)size * m * m * sizeof(COLORREF));
	int i, j, k, l;
	COLORREF** aa = new COLORREF * [height];
	aa[0] = &a[0];
	for (i = 1; i < height; i++) {
		aa[i] = aa[i - 1] + width;
	}

	int w = width * m, h = height * m;
	COLORREF** bb = new COLORREF * [h];
	bb[0] = &b[0];
	for (i = 1; i < h; i++) {
		bb[i] = bb[i - 1] + w;
	}

	int im, jm;
	for (i = 0; i < height; i++) {
		im = i * m;
		for (j = 0; j < width; j++) {
			jm = j * m;
			for (k = 0; k < m; k++) {
				for (l = 0; l < m; l++) {
					bb[im + k][jm + l] = aa[i][j];
				}
			}
		}
	}
	delete[] aa;
	delete[] bb;
}

BitmapPack* CreateBitmap(COLORREF* a, int size, int width, int height, int m) {
	COLORREF* b = new COLORREF[(size_t)size * m * m];
	memset(b, 0x00, (size_t)size * m * m * sizeof(COLORREF));
	BitmapPack* res = new BitmapPack(size * m * m, width * m, height * m, b);

	int i, j, k, l;
	COLORREF** aa = new COLORREF * [height];
	aa[0] = &a[0];
	for (i = 1; i < height; i++) {
		aa[i] = aa[i - 1] + width;
	}

	int w = width * m, h = height * m;
	COLORREF** bb = new COLORREF * [h];
	bb[0] = &b[0];
	for (i = 1; i < h; i++) {
		bb[i] = bb[i - 1] + w;
	}

	int im, jm;
	for (i = 0; i < height; i++) {
		im = i * m;
		for (j = 0; j < width; j++) {
			jm = j * m;
			for (k = 0; k < m; k++) {
				for (l = 0; l < m; l++) {
					bb[im + k][jm + l] = aa[i][j];
				}
			}
		}
	}
	delete[] aa;
	delete[] bb;

	return res;
}

void Trans(BitmapPack* pack) {
	int w = pack->width, h = pack->height, s = pack->size;
	int i, j, k, l;
	COLORREF* a = pack->data;
	COLORREF** aa = Get2DArray(pack->data, pack->size, pack->width, pack->height);

	COLORREF* b = new COLORREF[s];
	COLORREF** bb = Get2DArray(b, pack->size, pack->height, pack->width);

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			bb[i][j] = aa[j][i];
		}
	}
	pack->data = b;
	delete[] a;
	delete[] aa;
	delete[] bb;
}

void RevX(BitmapPack* pack) {
	int i, j, t, w = pack->width, h = pack->height, s = pack->size;
	COLORREF* a = pack->data;
	COLORREF** aa = Get2DArray(pack->data, pack->size, pack->width, pack->height);
	int hh = h / 2;
	for (i = 0; i < hh; i++) {
		for (j = 0; j < w; j++) {
			t = aa[i][j];
			aa[i][j] = aa[h - i - 1][j];
			aa[h - i - 1][j] = t;
		}
	}
}

void RevY(BitmapPack* pack) {
	int i, j, t, w = pack->width, h = pack->height, s = pack->size;
	COLORREF* a = pack->data;
	COLORREF** aa = Get2DArray(pack->data, pack->size, pack->width, pack->height);
	int ww = w / 2;
	for (i = 0; i < ww; i++) {
		for (j = 0; j < h; j++) {
			t = aa[j][i];
			aa[j][i] = aa[j][w - i - 1];
			aa[j][w - i - 1] = t;
		}
	}
}

void RotateCW(BitmapPack* pack) {
	Trans(pack);
	RevY(pack);
}

void RotateCCW(BitmapPack* pack) {
	Trans(pack);
	RevX(pack);
}

BitmapPack* BpClone(BitmapPack* pack) {
	COLORREF* c = new COLORREF[pack->size];
	memcpy(c, pack->data, pack->size * sizeof(COLORREF*));
	return new BitmapPack(pack->size, pack->width, pack->height, c);
}