// blurApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "blurFunction.h"
#include "blurASM.h"
#include "utilityFunctions.h"

using namespace std;

float  *sourceTab;
float *targetTab;


int main()
{
	byte * bmpTab;


	BITMAPINFOHEADER header;
	BITMAPFILEHEADER fileHeader;
	int bgrCounter = 0;
	int targetCounter = 0;
	if (bmpTab = LoadBitmapFile("test.bmp", &header, fileHeader)) {
		std::cout << "number of cores: " << getNumberOfCores() << std::endl;
		//bmpTarget = LoadBitmapFile("test.bmp", &header, fileHeader);
		byte* bmpTarget = (byte*)malloc(header.biSizeImage);
		byte* bmpTargetASM = (byte*)malloc(header.biSizeImage);
		byte* bmpHBlur = (byte*)malloc(header.biSizeImage);
		//createThreads(getNumberOfCores(), bmpTab, header.biSizeImage);
		algorithm(bmpTab, bmpTarget, bmpHBlur, header.biSizeImage, header.biWidth, header.biHeight);
		sourceTab = new float[header.biWidth*header.biHeight*4];
		targetTab = new float[header.biWidth*header.biHeight * 4];
		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {
				sourceTab[i - 1] = 0;
			}
			else {
				sourceTab[i - 1] = (float)bmpTab[bgrCounter];
				bgrCounter++;
			}
			targetTab[i - 1] = -1;
		}
		blur(sourceTab, targetTab, header.biWidth, header.biHeight);
		saveBitmapFile("output.bmp", &header, fileHeader, bmpTarget);
		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {
				
			}
			else {
				bmpTargetASM[targetCounter] = (BYTE)targetTab[i-1];
				targetCounter++;
			}
			
		}
		saveBitmapFile("outputASM.bmp", &header, fileHeader, bmpTargetASM);
		
		system("PAUSE");
		delete[] bmpTab, bmpHBlur;
		delete[] bmpTarget;
		return 0;
	}
	else {
		std::cout << "error!";
	}
    return 0;
}

