// blurApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "blurFunction.h"
#include "blurASM.h"
#include "utilityFunctions.h"
#include "bmpProcessing.h"

using namespace std;




int main()
{
	/*
		double cTime = 0;
		for (int j = 0; j < 10; j++) {
			 cTime += processBmpC("test3.bmp", "outputC.bmp", 1);
		}
		cout << cTime/10 << endl;
	*/
	
		
		
	
	
	cout << radius << endl;
		cout << processBmpC("test.bmp", "outputC.bmp",64) << endl;
		cout << processBmpAsm("test.bmp", "outputAsm.bmp", 64) << endl;
	
	
	//double testTime = processBmpCTest("test2.bmp", "outputTest.bmp");
	//cout << testTime << endl;
	
	system("PAUSE");
    return 0;
}

