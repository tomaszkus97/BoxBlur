#pragma once
#include "stdafx.h"
#include "utilityFunctions.h"
#include "blurASM.h"

using namespace std;

double processBmpC(char* inputName, char* outputName, int threadsNumber) {
	byte * bmpTab;


	BITMAPINFOHEADER header;
	BITMAPFILEHEADER fileHeader;
	int bgrCounter = 0;
	int targetCounter = 0;
	bool isPadding = false;
	int padding = 0;
	if (bmpTab = LoadBitmapFile(inputName, &header, fileHeader)) {
		//std::cout << "number of cores: " << getNumberOfCores() << std::endl;
		float *sourceTab = new float[header.biWidth*header.biHeight * 4];
		BYTE *sourceTabNoPadding = new BYTE[header.biWidth*header.biHeight * 3];
		float *targetTab = new float[header.biWidth*header.biHeight * 4];
		float *hblurTab = new float[header.biWidth*header.biHeight * 4];

		//handling bmp padding

		if ((header.biWidth * 3) % 4 != 0) {
			padding = 4 - ((header.biWidth * 3) % 4);
			isPadding = true;
		}
		else padding = 0;
		
			int counter = 0;
			int sourceCounter = 0;
			for (int i = 1; i <=header.biHeight; i++) {
				for (int j = 1; j <= header.biWidth * 3 ; j++) {
					sourceTabNoPadding[sourceCounter] = bmpTab[counter] ;
					counter++;
					sourceCounter++;
				}
				counter += padding;
			}
			

		
		
			for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
				if ((i % 4 == 0) && (i != 0)) {
					sourceTab[i - 1] = 0;
				}
				else {
					sourceTab[i - 1] = (float)sourceTabNoPadding[bgrCounter];
					bgrCounter++;
				}
				targetTab[i - 1] = -1;
			}
		
		clock_t begin = clock();
		//horizontal(sourceTab, hblurTab, header.biWidth, header.biHeight);
		//verticalC(hblurTab+7*header.biWidth*4, targetTab, header.biWidth, header.biHeight-14);
		createThreadsC(threadsNumber, sourceTab, targetTab, hblurTab, header.biWidth, header.biHeight, header.biSizeImage);
		clock_t end = clock();
		double asmTime = double(end - begin) / CLOCKS_PER_SEC;
		byte * bmpTargetASM = new byte[header.biWidth*header.biHeight * 3];
		byte * bmpTargetWithPadding = new byte[header.biWidth*header.biHeight * 3 + padding*header.biHeight];

		

		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {

			}
			else {
				bmpTargetASM[targetCounter] = (BYTE)targetTab[i - 1];
				//bmpTargetASM[targetCounter] = (BYTE)hblurTab[i - 1];
				targetCounter++;
			}

		}

		//bmp padding back to array

		counter = 0;
		sourceCounter = 0;
		for (int i = 1; i <= header.biHeight; i++) {
			for (int j = 1; j <= header.biWidth * 3; j++) {
				bmpTargetWithPadding[counter] = bmpTargetASM[sourceCounter];
				counter++;
				sourceCounter++;
			}
			for (int k = 0; k < padding; k++) {
				bmpTargetWithPadding[counter] = 0;
				counter++;
			}
			
		}

		saveBitmapFile(outputName, &header, fileHeader, bmpTargetWithPadding);


		delete[] bmpTab, bmpTargetASM, sourceTab, targetTab;

		return asmTime;
	}
	else {
		return -1;
	}
}
double processBmpAsm(char* inputName, char* outputName, int threadsNumber) {
	byte * bmpTab;


	BITMAPINFOHEADER header;
	BITMAPFILEHEADER fileHeader;
	int bgrCounter = 0;
	int targetCounter = 0;
	bool isPadding = false;
	int padding = 0;
	if (bmpTab = LoadBitmapFile(inputName, &header, fileHeader)) {
		//std::cout << "number of cores: " << getNumberOfCores() << std::endl;
		float *sourceTab = new float[header.biWidth*header.biHeight * 4];
		BYTE *sourceTabNoPadding = new BYTE[header.biWidth*header.biHeight * 3];
		float *targetTab = new float[header.biWidth*header.biHeight * 4];
		float *hblurTab = new float[header.biWidth*header.biHeight * 4];

		//handling bmp padding

		if ((header.biWidth * 3) % 4 != 0) {
			padding = 4 - ((header.biWidth * 3) % 4);
			isPadding = true;
		}
		else padding = 0;

		int counter = 0;
		int sourceCounter = 0;
		for (int i = 1; i <= header.biHeight; i++) {
			for (int j = 1; j <= header.biWidth * 3; j++) {
				sourceTabNoPadding[sourceCounter] = bmpTab[counter];
				counter++;
				sourceCounter++;
			}
			counter += padding;
		}




		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {
				sourceTab[i - 1] = 0;
			}
			else {
				sourceTab[i - 1] = (float)sourceTabNoPadding[bgrCounter];
				bgrCounter++;
			}
			targetTab[i - 1] = -1;
		}

		clock_t begin = clock();
		//horizontal(sourceTab, hblurTab, header.biWidth, header.biHeight);
		//verticalC(hblurTab+7*header.biWidth*4, targetTab, header.biWidth, header.biHeight-14);
		createThreadsAsm(threadsNumber, sourceTab, targetTab, hblurTab, header.biWidth, header.biHeight, header.biSizeImage);
		clock_t end = clock();
		double asmTime = double(end - begin) / CLOCKS_PER_SEC;
		byte * bmpTargetASM = new byte[header.biWidth*header.biHeight * 3];
		byte * bmpTargetWithPadding = new byte[header.biWidth*header.biHeight * 3 + padding*header.biHeight];



		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {

			}
			else {
				bmpTargetASM[targetCounter] = (BYTE)targetTab[i - 1];
				//bmpTargetASM[targetCounter] = (BYTE)hblurTab[i - 1];
				targetCounter++;
			}

		}

		//bmp padding back to array

		counter = 0;
		sourceCounter = 0;
		for (int i = 1; i <= header.biHeight; i++) {
			for (int j = 1; j <= header.biWidth * 3; j++) {
				bmpTargetWithPadding[counter] = bmpTargetASM[sourceCounter];
				counter++;
				sourceCounter++;
			}
			for (int k = 0; k < padding; k++) {
				bmpTargetWithPadding[counter] = 0;
				counter++;
			}

		}

		saveBitmapFile(outputName, &header, fileHeader, bmpTargetWithPadding);


		delete[] bmpTab, bmpTargetASM, sourceTab, targetTab;

		return asmTime;
	}
	else {
		return -1;
	}
}
double processBmpCTest(char* inputName, char* outputName) {
	byte * bmpTab;


	BITMAPINFOHEADER header;
	BITMAPFILEHEADER fileHeader;
	int bgrCounter = 0;
	int targetCounter = 0;
	if (bmpTab = LoadBitmapFile(inputName, &header, fileHeader)) {
		//std::cout << "number of cores: " << getNumberOfCores() << std::endl;
		float *sourceTab = new float[header.biWidth*header.biHeight * 4];
		float *targetTab = new float[header.biWidth*header.biHeight * 4];
		float *hblurTab = new float[header.biWidth*header.biHeight * 4];
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

		clock_t begin = clock();
		//horizontal(sourceTab, hblurTab, header.biWidth, header.biHeight);
		//verticalC(hblurTab+7*header.biWidth*4, targetTab, header.biWidth, header.biHeight-14);
		createThreadsTest(2, sourceTab, targetTab, hblurTab, header.biWidth, header.biHeight, header.biSizeImage);
		clock_t end = clock();
		double asmTime = double(end - begin) / CLOCKS_PER_SEC;
		byte * bmpTargetASM = new byte[header.biWidth*header.biHeight * 4];

		for (int i = 1; i <= header.biWidth*header.biHeight * 4; i++) {
			if ((i % 4 == 0) && (i != 0)) {

			}
			else {
				bmpTargetASM[targetCounter] = (BYTE)targetTab[i - 1];
				//bmpTargetASM[targetCounter] = (BYTE)hblurTab[i - 1];
				targetCounter++;
			}

		}


		saveBitmapFile(outputName, &header, fileHeader, bmpTargetASM);


		delete[] bmpTab, bmpTargetASM, sourceTab, targetTab;

		return asmTime;
	}
	else {
		return -1;
	}
}