#include "stdafx.h"
#include "utilityFunctions.h"
#include "blurFunction.h"
#include "blurASM.h"
using namespace std;

/*
function returns number of cores
*/
int getNumberOfCores() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;

}



/*
function creating threads and divides input data
runs algorithm function in threads
*/

void createThreadsC(int threadNumber, float* data, float* target, float* hblur, int width, int height, int sizeOfData) {
	sizeOfData = width * height * 4;
	int partialHeight = height / threadNumber;
	int restHeight = height % threadNumber;


	std::thread *threadTab = new std::thread[threadNumber];
	/*processing data -> starting time measuring*/
	for (int i = 0; i < threadNumber; i++) {
		if (restHeight != 0 && i == (threadNumber - 1)) {
			float* currentData = data + (i*partialHeight*width*4);
			float* currentBlur = hblur + (i*partialHeight*width*4);
			
			threadTab[i] = std::thread(horizontal, currentData, currentBlur, width, partialHeight + restHeight);
			
		}
		else {
			float* currentData = data + (i*partialHeight*width*4);
			float* currentBlur = hblur + (i*partialHeight*width*4);
			
			threadTab[i] = std::thread(horizontal, currentData, currentBlur, width, partialHeight);
			
		}
	}
	for (int i = 0; i < threadNumber; i++)
	{
		threadTab[i].join();
	}

	
	for (int i = 0; i < threadNumber; i++) {
		float* currentTarget = target + (i*partialHeight*width * 4);
		
		float* currentBlur = hblur + (i*partialHeight*width * 4) + 7 * width * 4;
		if (threadNumber == 1) {
			threadTab[i] = std::thread(verticalC, currentBlur, currentTarget, width, partialHeight - 14);
		}
		
		else if (i == (threadNumber - 1)) {
			threadTab[i] = std::thread(verticalC, currentBlur, currentTarget, width, partialHeight - 14 + restHeight);
		}
		else {
			threadTab[i] = std::thread(verticalC, currentBlur, currentTarget, width, partialHeight);
		}


	}
	for (int i = 0; i < threadNumber; i++)
	{
		threadTab[i].join();
	}
	

}
void createThreadsAsm(int threadNumber,float* data, float* target, float* hblur, int width, int height, int sizeOfData){
	sizeOfData = width * height * 4;
	

	int partialHeight = height / threadNumber;
	int restHeight = height % threadNumber;


	std::thread *threadTab = new std::thread[threadNumber];
	/*processing data -> starting time measuring*/
	for (int i = 0; i < threadNumber; i++) {

		float* currentData = data + (i*partialHeight*width * 4);
		float* currentBlur = hblur + (i*partialHeight*width * 4);
		//float* currentTarget = target + (i*partialSize);
		if (restHeight != 0 && i == (threadNumber - 1)) {
			threadTab[i] = std::thread(blur, currentData, currentBlur, width, partialHeight+restHeight);
			threadTab[i].join();
			
		}
		else {
			threadTab[i] = std::thread(blur, currentData, currentBlur, width, partialHeight);
			threadTab[i].join();
		}
		//blur(currentData, currentBlur, width, partialHeight);
	}


	for (int i = 0; i < threadNumber; i++) {
		float* currentTarget = target + (i*partialHeight*width * 4);
		float* currentBlur = hblur + (i*partialHeight*width * 4) + 7 * width * 4;
		if (threadNumber == 1) {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight - 14);
			threadTab[i].join();
		}
		else if (i == 0) {
			//currentBlur = hblur + (i*partialSize) ;
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight);
			threadTab[i].join();
		}
		else if (i == (threadNumber - 1)) {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight - 14 + restHeight);
			threadTab[i].join();
		}
		else {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight);
			threadTab[i].join();
		}


	}
	

}
void createThreadsTest(int threadNumber, float* data, float* target, float* hblur, int width, int height, int sizeOfData) {
	sizeOfData = width * height * 4;
	int partialSize = sizeOfData / threadNumber;
	
	int partialHeight = height / threadNumber;
	

	std::thread *threadTab = new std::thread[threadNumber];
	/*processing data -> starting time measuring*/
	for (int i = 0; i < threadNumber; i++) {
		float* currentData = data + (i*partialSize);
		float* currentBlur = hblur + (i*partialSize);
		//float* currentTarget = target + (i*partialSize);
		threadTab[i] = std::thread(blur, currentData, currentBlur, width, partialHeight);
		threadTab[i].join();
		//blur(currentData, currentBlur, width, partialHeight);
	}
	
	
	for (int i = 0; i < threadNumber; i++) {
		float* currentTarget = target + (i*partialSize);
		float* currentBlur = hblur + (i*partialSize) + 7 * width * 4;
		if (threadNumber == 1) {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight - 14);
			threadTab[i].join();
		}
		else if (i == 0) {
			//currentBlur = hblur + (i*partialSize) ;
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight);
			threadTab[i].join();
		}
	 else if(i == (threadNumber - 1)) {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight-14);
			threadTab[i].join();
		}
		else {
			threadTab[i] = std::thread(vertical, currentBlur, currentTarget, width, partialHeight);
			threadTab[i].join();
		}
		
		
	}
	
	
}

/*
function loading Bitmap File into byte array and Headers
*/
byte *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER& fileHeader)
{
	FILE *filePtr; //file pointer
	BITMAPFILEHEADER bitmapFileHeader; //bitmap file header
	byte *bitmapImage;  //image data array
	int imageIdx = 0;  //image index counter


					   //open filename in read binary mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	//read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	//verify that this is a bmp file by check bitmap id
	
	if (bitmapFileHeader.bfType != 0x4D42)
	{
		fclose(filePtr);
		return NULL;
	}
	
	//read the bitmap info header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	//move file point to the begging of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	int padding = 4 - ((bitmapInfoHeader->biWidth * 3) % 4);
	
	//allocate enough memory for the bitmap image data
	bitmapImage = (byte*)malloc(bitmapInfoHeader->biSizeImage+padding*bitmapInfoHeader->biHeight);

	//verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader->biSizeImage + padding*bitmapInfoHeader->biHeight, 1, filePtr);

	//make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}
	fileHeader = bitmapFileHeader;

	//close file and return bitmap iamge data
	fclose(filePtr);
	return bitmapImage;
}

/*
function saving processed array into bitmap file
*/
void saveBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER fileHeader, byte * data)
{
	FILE *filePtr; //our file pointer

	int imageIdx = 0;  //image index counter


					   //open filename in read binary mode
	filePtr = fopen(filename, "wb");
	if (filePtr == NULL) {
		std::cout << "cannot open file" << std::endl;
	}




	//write the bitmap info header
	//fwrite(bitmapInfoHeader,sizeof(unsigned char), sizeof(BITMAPINFOHEADER), filePtr); // small edit. forgot to add the closing bracket at sizeof
	//move file point to the begging of bitmap data
	//fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	fwrite(&fileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);
	fwrite(bitmapInfoHeader, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), filePtr);


	//write in the bitmap image data
	fwrite(data, 1, bitmapInfoHeader->biSizeImage, filePtr);





	//close file and return bitmap iamge data
	fclose(filePtr);

}


