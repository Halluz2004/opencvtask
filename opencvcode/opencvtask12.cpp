#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture cap(0);
	Mat img, imgBlur, imgGaussianBlur, imgMedianBlur;
	while (1) {
		cap.read(img);
		imshow("img",img);

		blur(img, imgBlur, Size(3, 3));//均值滤波
		GaussianBlur(img, imgGaussianBlur, Size(3, 3), 3, 0);//高斯滤波
		medianBlur(img, imgMedianBlur, 5);//中值滤波

		imshow("Blur",imgBlur);
		imshow("medianBlur", imgMedianBlur);
		if (waitKey(1) == 32)
			break;
	}
	return 0;
}