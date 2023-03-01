#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv; 
using namespace std;

int main(int argc, char** argv)
{
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));//初始化一张白色图像
	rectangle(img, Point(180, 200), Point(332, 300), Scalar(0, 0, 0), FILLED); 
	line(img, Point(180, 320), Point(332, 320), Scalar(67, 255, 0), 6);
	circle(img, Point(256, 120), 50, Scalar(0, 0, 0), 4);

	imshow("img", img);
	waitKey(0);

	return 0;
}