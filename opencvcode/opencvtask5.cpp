#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//导入摄像头
int main(int argc, char** argv) {
	VideoCapture cap(0);
	Mat img;

	while (1) {
		if (cap.isOpened())//判断摄像头是否打开
		{
			cap.read(img);
			//判断图像是否读取
			if (img.empty()) {
				printf("NO Image");
				return -1;
			}
			imshow("img", img);
			
			//计算FPS
			double fps = cap.get(CAP_PROP_FPS);
			cout << "fps= " << fps << endl;

			if (waitKey(1) == 32)
				break;
		}
		else {
			printf("NO Camera");
			return -2;
		}
	}
	return 0;
}
