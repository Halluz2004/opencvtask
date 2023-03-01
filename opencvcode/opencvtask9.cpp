#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std; 

int main(int argc, char** argv) {
	Mat img = imread("Resources/9.png");
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img", img);

	Mat imgGray, imgDil, imgErode, imgOpen,imgClose;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//矩形，大小3*3的算子
	dilate(imgGray, imgDil, kernel);
	erode(imgGray, imgErode, kernel);
	morphologyEx(imgGray, imgOpen, MORPH_OPEN, kernel);
	morphologyEx(imgGray, imgClose, MORPH_CLOSE, kernel);

	imshow("imgDil", imgDil);
	imshow("imgErode", imgErode);
	imshow("imgOpen", imgOpen);
	imshow("imgClose", imgClose);
	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}