// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

float  *sourceTab;
float *targetTab;
int width=4, height=4;

extern "C" void MyProc(float * source,  float * target, int width, int height);

int main()
{
	sourceTab = new float[width*height*4];
	targetTab = new float[width*height * 4];
	for (int i = 1; i <= width*height * 4; i++) {
		if ((i % 4 == 0)&&(i!=0)) {
			sourceTab[i-1] = 0;
		}
		else {
			sourceTab[i-1] = i;
		}
		targetTab[i-1] = -1;
	}
	MyProc(sourceTab, targetTab,width,height);
	
	for (int i = 0; i < width*height*4; i++) {
		if (i % (width * 4) == 0) {
			cout << endl;
		}
		cout << targetTab[i] << " ";

	}
	system("PAUSE");
    return 0;
}

