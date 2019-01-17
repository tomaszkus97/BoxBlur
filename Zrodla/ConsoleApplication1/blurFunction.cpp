// ConsoleApplication1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "blurFunction.h"

int average(std::vector<int> vec) {
	int number = vec.capacity();
	int sum = 0;
	for (auto i : vec) {
		sum += i;
	}
	return sum / number;
}
void horizontal(float * source, float * target, int width, int height) {
	//std::cout << "radius="<<blurRadiusC << std::endl;
	const int blurSize = 7;
	int size = width*height*4;
	std::vector <float> average(4);
	int x, y, pixelCounter = 0;
	//std::cout << "width:" << width << "height:" << height << "size:" << size << std::endl;

	for (int i = 0; i <  size; i += 4) {
		//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel
		x = pixelCounter%width;

		for (int i = 0; i < 4; i++) {
			average[i] = 0;
		}
		
		//BGR
		//if on the border of image

		if ((x - (blurSize / 2)) > 0 && x < (width - (blurSize / 2))) {




			for (int j = -(blurSize / 2); j <= blurSize / 2; j++) {
				average[0]+=(source[i + (j * 4)]);
				average[1]+=(source[i + 1 + (j * 4)]);
				average[2]+=(source[i + 2 + (j * 4)]);
			}
			for (int i = 0; i < 3; i++) {
				average[i] = average[i] / blurSize;
			}
			//std::cout << "B:" << bPixelRow[0] << "    G:" << gPixelRow[0] << "     R:" << rPixelRow[0] << std::endl;
			target[i] = average[0];
			target[i + 1] = average[1];
			target[i + 2] = average[2];
		}
		else {
			target[i] = source[i];
			target[i + 1] = source[i + 1];
			target[i + 2] = source[i + 2];
		}
		pixelCounter++;

	}

}
void verticalC(float * source, float * target, int width, int height) {
	const int blurSize = 7;
	int size = width*height * 4;
	std::vector <float> average(4);
	int x, y, pixelCounter = 0;
	//std::cout << "width:" << width << "height:" << height << "size:" << size << std::endl;

	for (int i = 0; i < size; i += 4) {
		//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel
		y = pixelCounter / width;

		for (int i = 0; i < 4; i++) {
			average[i] = 0;
		}

		//BGR
		//if on the border of image

		//if ((y - (blurSize / 2))> 0 && y < (height - (blurSize / 2))) {




			for (int j = -(blurSize / 2); j <= blurSize / 2; j++) {
				average[0] += (source[i + (j * 4 * width)]);
				average[1] += (source[i + 1 + (j * 4 * width)]);
				average[2] += (source[i + 2 + (j * 4* width)]);
			}
			for (int i = 0; i < 3; i++) {
				average[i] = average[i] / blurSize;
			}
			//std::cout << "B:" << bPixelRow[0] << "    G:" << gPixelRow[0] << "     R:" << rPixelRow[0] << std::endl;
			target[i] = average[0];
			target[i + 1] = average[1];
			target[i + 2] = average[2];
		/*}
		else {
			target[i] = source[i];
			target[i + 1] = source[i + 1];
			target[i + 2] = source[i + 2];
		}
		*/
		pixelCounter++;

	}

}
/*image processing function
will be run in threads
*/
void algorithm(BYTE* data, BYTE* target, BYTE* hblur, int size, int width, int height, int startingPoint) {

	//horizontal blur
	const int blurSize =7;
	std::vector <int> rPixelRow, gPixelRow, bPixelRow;
	int x, y, pixelCounter = 0;
	//std::cout << "width:" << width << "height:" << height << "size:" << size << std::endl;

	for (int i = startingPoint; i < startingPoint+size; i += 3) {
		//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel
		x = pixelCounter%width;

		rPixelRow.clear();
		gPixelRow.clear();
		bPixelRow.clear();
		//BGR
		//if on the border of image

		if ((x- (blurSize / 2)) > 0 && x < (width - (blurSize / 2))) {




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
		else {
			hblur[i] = data[i];
			hblur[i + 1] = data[i+1];
			hblur[i + 2] = data[i+2];
		}
		pixelCounter++;

	}

	//vertical blur
	pixelCounter = 0;
	for (int i = startingPoint; i < startingPoint+size; i += 3) {
		//std::cout<<"B:"<<(int)data[i]<<"    G:"<<(int)data[i+1]<<"     R:"<<(int)data[i + 2]<<std::endl;
		//computing coordinates of current pixel

		y = pixelCounter / width;
		rPixelRow.clear();
		gPixelRow.clear();
		bPixelRow.clear();
		//BGR
		//if on the border of image

		//if ((y - (blurSize/2))> 0 && y < (height - (blurSize / 2))) {




			for (int j = -(blurSize / 2); j <= blurSize / 2; j++) {
				bPixelRow.push_back(hblur[i + (j * 3 * width)]);
				gPixelRow.push_back(hblur[i + 1 + (j * 3 * width)]);
				rPixelRow.push_back(hblur[i + 2 + (j * 3 * width)]);
			}
			//std::cout << "B:" << bPixelRow[0] << "    G:" << gPixelRow[0] << "     R:" << rPixelRow[0] << std::endl;
			target[i] = average(bPixelRow);
			target[i + 1] = average(gPixelRow);
			target[i + 2] = average(rPixelRow);
	//	}
			/*
		else {
			target[i] = hblur[i];
			target[i + 1] = hblur[i+1];
			target[i + 2] = hblur[i+2];
		}
		*/
		pixelCounter++;

	}
}