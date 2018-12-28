#pragma once

#include "stdafx.h"
int getNumberOfCores();
int average(std::vector<int> vec);
void algorithm(byte* data, byte* target, byte* hblur, int size, int width, int height);
byte *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER& fileHeader);
void saveBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER fileHeader, byte * data);

