#pragma once
using namespace std;

#ifndef GOBJECT
#define GOBJECT

class BitmapPack {
public:
	COLORREF* data;
	int size;
	int width;
	int height;
};

class GObject {
private:
	BitmapPack* GetBitmap(string path);
public:
	COLORREF* defaultShape = nullptr;
	vector<COLORREF*>* shapes;
	vector<vector<pair<int, int>>*>* actions;
	int cAction = 0;
	int cSequence = 0;
	int cFrame = 0;
	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;
	GObject();
	~GObject();
	void AddShape(COLORREF* shape, int width, int height);
	void AddShape(string path);
	COLORREF* GetShape();	
	void SetAction(int x);	
};

#endif