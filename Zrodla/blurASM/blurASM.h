#pragma once
#define BLURASM_API __declspec(dllexport)

extern "C"  void __declspec(dllexport) blur(
	float * source, float * target, int width, int height);