#pragma once
#pragma once


#define BLURFUNCTION_API __declspec(dllexport)

#include "stdafx.h"
extern   BLURFUNCTION_API int radius;
extern "C"  int __declspec(dllexport)  average(std::vector<int> vec);


extern "C"  void __declspec(dllexport) horizontal(
	float * source, float * target, int width, int height);
extern "C"  void __declspec(dllexport) verticalC(
	float * source, float * target, int width, int height);
extern "C"  void __declspec(dllexport) algorithm(BYTE* data, BYTE* target, BYTE* hblur, int size, int width, int height, int startingPoint);

