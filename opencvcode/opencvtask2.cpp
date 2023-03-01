#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat threshold(Mat img, uchar threshold) {
	//遍历每个像素 
	for (int i = 0; i < img.rows; i++) {
		uchar* p_row = img.ptr<uchar>(i);//p_row指向每行的首元素的地址
		for (int j = 0; j < img.cols; j++) {
			uchar average = (p_row[3 * j] + p_row[3 * j + 1] + p_row[3 * j + 2]) / 3;//取平均值
			//判断平均数和阈值大小关系
			if (average > threshold) {
				p_row[3 * j] = 255;
				p_row[3 * j + 1] = 255;
				p_row[3 * j + 2] = 255;
			}
			else {
				p_row[3 * j] = 0;
				p_row[3 * j + 1] = 0;
				p_row[3 * j + 2] = 0;
			}
		}
	}
	return img;
}

int main(int argc, char** argv) {
	Mat img = imread("C:/Users/24816/Desktop/image/xyy.jpg");//读取图片
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img_before", img);

	Mat imgThreshold = threshold(img, 100);
	//Mat imgThreshold = threshold(img, 200);
	imshow("img_after", imgThreshold);//观察到图片二值化

	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}