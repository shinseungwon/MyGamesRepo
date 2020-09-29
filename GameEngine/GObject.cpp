#include "stdafx.h"
#include "GObject.h"

GObject::GObject() {
	shapes = new vector<COLORREF*>();
	actions = new vector<vector<pair<int, int>>*>();
}

void GObject::AddShape(COLORREF* shape, int width, int height) {
	if (this->width == 0) {
		this->width = width;
	}
	if (this->height == 0) {
		this->height = height;
	}
	shapes->push_back(shape);
}

void GObject::AddShape(string path) {
	BitmapPack* pack = GetBitmap(path);
	AddShape(pack->data, pack->width, pack->height);
}

void GObject::SetAction(int x) {
	if (x < actions->size() && x >= 0) {
		cAction = x;
	}
	else {
		cAction = 0;
	}
}

COLORREF* GObject::GetShape() {

	if (actions->size() == 0) {
		return shapes->at(0);
	}

	vector<pair<int, int>>* action = actions->at(cAction);

	int mSequence = action->size();
	pair<int, int> sequence = action->at(cSequence);

	int mFrame = sequence.second;
	COLORREF* shape = shapes->at(sequence.first);

	if (cFrame == mFrame - 1) {
		cFrame = 0;
		if (cSequence == mSequence - 1) {
			cSequence = 0;
		}
		else {
			cSequence++;
		}
	}
	else {
		cFrame++;
	}

	return shape;
}

BitmapPack* GObject::GetBitmap(string path) {
	BitmapPack* res = new BitmapPack();
	int i = 0, j = 0;
	char* arr = nullptr;

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
			arr = new char[resultSize];

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

GObject::~GObject() {

}