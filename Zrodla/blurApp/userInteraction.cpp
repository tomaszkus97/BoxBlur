#include "stdafx.h"
#include "userInteraction.h"

int getNumberOfThreads(int systemCores)

{
	int cores = 0;
	cout << "enter number of threads you want to use (between or equall 1 and 64)" << endl;
	cout << systemCores << " is recommended value for your system" << endl;
	cin >> cores;
	while (cores < 1 || cores>64) {
		cout << "wrong input value!" << endl;
		cin >> cores;
	}
	return cores;
}

string getInputPath()
{
	string path;
	cout << "enter path to bmp image you want to process" << endl;
	cin >> path;
	return path;
}

void printResults(double asmTime, double cTime)
{
	cout << "bmp was successfully processed and saved in files:" << endl;
	cout << "outputASM.bmp for assembly algorithm -> processed in " << asmTime << " seconds" << endl;
	cout << "outputC.bmp for c++ algorithm -> processed in " << cTime << " seconds" << endl;
}
