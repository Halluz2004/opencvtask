#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img = imread("C:/Users/24816/Desktop/image/xyy.jpg");
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img", img);

	Mat imgChannels[3];
	split(img, imgChannels);//ͨ�����룬������imgChannels������

	imshow("blue",imgChannels[0]);
	imshow("green", imgChannels[1]);
	imshow("red", imgChannels[2]);
	//���¿ո�ֹͣ
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}