#include "CenterOfFocus.h"

int main(int argc, char **argv) {
	CenterOfFocus cof;
	cof.getCOF();
	pair<Mat, Point>* array = cof.getArray();
	for (int i = 0; i < 5; i++) {
		cout << array[i].second.x << " " << array[i].second.x << endl;
	}
	return 0;
}
