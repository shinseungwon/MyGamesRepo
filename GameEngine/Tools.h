#pragma once
using namespace std;

class BitmapPack {
public:
	COLORREF* data;
	int size;
	int width;
	int height;
	BitmapPack();
	BitmapPack(int s, int w, int h, COLORREF* d);
};

BitmapPack* GetBitmap(string path);
DWORD LoadMp3(HWND& hWnd, LPCWSTR path);
void PlayMp3(HWND& hWnd, int id);
void WriteBitmap(COLORREF* a, COLORREF* b, int size, int width, int height, int m = 1);
BitmapPack* CreateBitmap(COLORREF* a, int size, int width, int height, int m = 1);

template <typename T> T** Get2DArray(T* a, int s, int w, int h) {
	int i, j;
	T** aa = new T*[h];
	aa[0] = &a[0];
	for (i = 1; i < h; i++) {
		aa[i] = aa[i - 1] + w;
	}

	return aa;
}

template <typename T> void Trans(T* a, int s, int w, int h) {
	int i, j;
	T** aa = Get2DArray(a, s, w, h);

	T* b = new T[s];
	memset(b, 0x00, s * sizeof(T));

	T** bb = Get2DArray(b, s, h, w);
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			bb[i][j] = aa[j][i];
		}
	}

	delete[] aa;
	delete[] bb;

	memcpy(a, b, s * sizeof(T));
	delete[] b;
}

template <typename T> void RevX(T* a, int s, int w, int h) {
	T** aa = Get2DArray(a, s, w, h);
	int i, j, hh = h / 2;
	T t;
	for (i = 0; i < hh; i++) {
		for (j = 0; j < w; j++) {
			t = aa[i][j];
			aa[i][j] = aa[h - i - 1][j];
			aa[h - i - 1][j] = t;
		}
	}
}

template <typename T> void RevY(T* a, int s, int w, int h) {
	T** aa = Get2DArray(a, s, w, h);
	int i, j, ww = w / 2;
	T t;
	for (i = 0; i < ww; i++) {
		for (j = 0; j < h; j++) {
			t = aa[j][i];
			aa[j][i] = aa[j][w - i - 1];
			aa[j][w - i - 1] = t;
		}
	}
}

template <typename T> void RotateCW(T* a, int s, int w, int h) {
	Trans(a, s, w, h);
	RevY(a, s, h, w);
}

template <typename T> void RotateCCW(T* a, int s, int w, int h) {
	Trans(a, s, w, h);
	RevX(a, s, h, w);
}

void Trans(BitmapPack* pack);
void RevX(BitmapPack* pack);
void RevY(BitmapPack* pack);
void RotateCW(BitmapPack* pack);
void RotateCCW(BitmapPack* pack);

BitmapPack* BpClone(BitmapPack* pack);