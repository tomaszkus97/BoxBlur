#pragma once

#include "stdafx.h"
int getNumberOfCores();
void createThreadsC(int threadNumber, float* data, float* target, float* hblur, int width, int height, int sizeOfData);
void createThreadsAsm(int threadNumber, float* data, float* target, float* hblur, int width, int height, int sizeOfData);
byte *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER& fileHeader);
void saveBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER fileHeader, byte * data);
void createThreadsTest(int threadNumber, float* data, float* target, float* hblur, int width, int height, int sizeOfData);
