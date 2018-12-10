// BoxBlurApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BoxBlurApp.h"


/*
function returns number of cores
*/
int getNumberOfCores() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;

}
int average(std::vector<int> vec) {
	int number = vec.capacity();
	int sum=0;
	for (auto i: vec) {
		sum += i;
	}
	return sum / number;
}
/*image processing function
will be run in threads
*/
void algorithm(byte* data,byte* target,byte* hblur,int size,int width, int height) {
	
	//horizontal blur
	const int blurSize = 3;
	std::vector <int> rPixelRow, gPixelRow, bPixelRow;
	int x, y, pixelCounter = 0;
	std::cout << "width:" << width << "height:" << height << "size:"<<size<<std::endl;
	
	for (int i = 3; i < size-3; i+=3) {
	//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel
		x = pixelCounter%width;
		
		rPixelRow.clear();
		gPixelRow.clear();
		bPixelRow.clear();
		//BGR
		//if on the border of image
		
		if (x != 0 && x != width - 1) {




			for (int j = -(blurSize / 2); j <= blurSize / 2; j++) {
				bPixelRow.push_back(data[i + (j * 3)]);
				gPixelRow.push_back(data[i + 1 + (j * 3)]);
				rPixelRow.push_back(data[i + 2 + (j * 3)]);
			}
			//std::cout << "B:" << bPixelRow[0] << "    G:" << gPixelRow[0] << "     R:" << rPixelRow[0] << std::endl;
			hblur[i] = average(bPixelRow);
			hblur[i + 1] = average(gPixelRow);
			hblur[i + 2] = average(rPixelRow);
		}
		pixelCounter++;

	}
	
	//vertical blur
	pixelCounter = 0;
	for (int i = 3; i < size - 3; i += 3) {
		//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel
		
		y = pixelCounter / width;
		rPixelRow.clear();
		gPixelRow.clear();
		bPixelRow.clear();
		//BGR
		//if on the border of image

		if (y != 0 && y != height - 1) {




			for (int j = -(blurSize / 2); j <= blurSize / 2; j++) {
				bPixelRow.push_back(hblur[i + (j * 3 * width)]);
				gPixelRow.push_back(hblur[i + 1 + (j * 3 * width)]);
				rPixelRow.push_back(hblur[i + 2 + (j * 3 * width)]);
			}
			//std::cout << "B:" << bPixelRow[0] << "    G:" << gPixelRow[0] << "     R:" << rPixelRow[0] << std::endl;
			target[i] = average(bPixelRow);
			target[i + 1] = average(gPixelRow);
			target[i + 2] = average(rPixelRow);
		}
		pixelCounter++;

	}
}
/*
function creating threads and divides input data 
runs algorithm function in threads
*/
/*
void createThreads(int threadNumber, unsigned char* data, int sizeOfData) {
	int partialSize = sizeOfData / threadNumber;
	std::thread *threadTab = new std::thread[4];
	unsigned char* partialData = new unsigned char[partialSize];
	std::cout << "size of data:" << sizeOfData << std::endl;
	std::cout << "partial size:" << partialSize << std::endl;
	for (int i = 0; i < threadNumber; i++) {
		
		for (int j = i*partialSize,k=0; j < (i+1)* partialSize ; j++,k++) {
			partialData[k] = data[j];
		}
		
		for (int j = 0; j <  partialSize; j++) {
			partialData[j] = data[j];
		}
		

		threadTab[i] =  std::thread(algorithm,partialData,partialSize);
		std::cout << "thread: " << i << "working" << std::endl;
	}

}
*/
/*
function loading Bitmap File into char array and Headers 
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

	//allocate enough memory for the bitmap image data
	bitmapImage = (byte*)malloc(bitmapInfoHeader->biSizeImage);

	//verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

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
 void saveBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,BITMAPFILEHEADER fileHeader, byte * data)
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

	fwrite(&fileHeader,1, sizeof(BITMAPFILEHEADER), filePtr);
	fwrite(bitmapInfoHeader, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), filePtr);
	

	//write in the bitmap image data
	fwrite(data, 1, bitmapInfoHeader->biSizeImage, filePtr);

	

	

	//close file and return bitmap iamge data
	fclose(filePtr);
	
}

int main()
{
	byte * bmpTab;
	
	
	BITMAPINFOHEADER header;
	BITMAPFILEHEADER fileHeader;
	
	if (bmpTab = LoadBitmapFile("test.bmp", &header, fileHeader)) {
		std::cout << "number of cores: " << getNumberOfCores() << std::endl;
		//bmpTarget = LoadBitmapFile("test.bmp", &header, fileHeader);
		byte* bmpTarget = (byte*)malloc(header.biSizeImage);
		byte* bmpHBlur = (byte*)malloc(header.biSizeImage);
		//createThreads(getNumberOfCores(), bmpTab, header.biSizeImage);
		algorithm(bmpTab,bmpTarget,bmpHBlur, header.biSizeImage,header.biWidth,header.biHeight);
		saveBitmapFile("output.bmp", &header, fileHeader, bmpTarget);
		system("PAUSE");
		delete[] bmpTab,bmpHBlur;
		delete[] bmpTarget;
		return 0;
	}
	else {
		std::cout << "error!";
	}
}

