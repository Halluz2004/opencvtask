#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img = imread("Resources/8.png");
Mat imgHSV,dst;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

void on_Trackbar(int, void*) {

	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);

	inRange(imgHSV, lower, upper,dst);
	imshow("dst", dst);
}
int main(int argc, char** argv) {
	
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img", img);

	cvtColor(img, imgHSV, COLOR_BGR2HSV);//转化为HSV空间
	
	namedWindow("trackbar", 0);
	createTrackbar("hmin", "trackbar",  &hmin, 179, on_Trackbar);
	createTrackbar("hmax", "trackbar",  &hmax, 179, on_Trackbar);
	createTrackbar("smin", "trackbar",  &smin, 255, on_Trackbar);
	createTrackbar("smax", "trackbar",  &smax, 255, on_Trackbar);
	createTrackbar("vmin", "trackbar",  &vmin, 255, on_Trackbar);
	createTrackbar("vmax", "trackbar",  &vmax, 255, on_Trackbar);
	
	on_Trackbar(hmin, 0);//滑动条初始化
	//滑动滑动条 提取红色像素
	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}