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
	Mat srcMat = imread("C:/Users/24816/Desktop/image/xyy.jpg");//读取图片
	imshow("img_before", srcMat);
	
	Mat deepMat, shallowMat;
	shallowMat = srcMat;//浅复制
	srcMat.copyTo(deepMat);//深复制

	//按任务二修改
	srcMat = threshold(srcMat, 100);
	
	imshow("deepMat", deepMat);
	imshow("shallowMat", shallowMat);//观察得浅复制受影响，深复制不受影响
	//按下空格停止
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}