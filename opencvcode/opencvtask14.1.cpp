#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void remove_acne(Mat src)
{
	// 转化为灰度图像
	Mat imgGray;
	cvtColor(src, imgGray, COLOR_BGR2GRAY);

	//中值滤波-去除噪声
	Mat imgBlur(src.size(), src.type());
	GaussianBlur(imgGray, imgBlur, Size(), 2.0);

	// OTSU threshold查找痘
	Mat imgThreshold;
	threshold(imgBlur, imgThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// acne mask应用形态学开（细化，分离轮廓）
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat acneMask;
	morphologyEx(imgThreshold, acneMask, MORPH_OPEN, kernel);
	imshow("Acne Mask", acneMask);

	// inpaint复原
	inpaint(src, acneMask, src, 10, INPAINT_TELEA);

}
int main(int argc, char** argv) {
	Mat img = imread("Resources/14-1.jpg");
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img_before", img);
	Rect ROI(100, 350, 200, 100);
	Mat imgROI = img(ROI);
	imshow("ROI", imgROI);

	remove_acne(imgROI);
	imshow("img_after", img);

	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}