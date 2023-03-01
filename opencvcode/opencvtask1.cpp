#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img = imread("C:/Users/24816/Desktop/image/xyy.jpg");//读取图片
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img_before", img);
	
	//遍历每个像素 
	for (int i = 0; i < img.rows; i++) {
		uchar* p_row = img.ptr<uchar>(i);//p_row指向每行的首元素的地址
		for (int j = 0; j < img.cols ; j++) {
			//取平均值并赋值
			uchar average = (p_row[3 * j] + p_row[3 * j + 1] + p_row[3 * j + 2]) / 3;
			p_row[3 * j] = average;
			p_row[3 * j + 1] = average;
			p_row[3 * j + 2] = average;
		}
	}
	imshow("img_after", img);//观察到图像变成灰度图像（三个通道值相同）
	
	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;
	
	return 0;
}