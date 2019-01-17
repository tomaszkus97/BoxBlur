#pragma once
#include "stdafx.h"

using namespace std;

int getNumberOfThreads(int systemCores);
string getInputPath();
void printResults(double asmTime, double cTime);
