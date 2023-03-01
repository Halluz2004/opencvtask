#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//��������ͷ
int main(int argc, char** argv) {
	VideoCapture cap(0);
	Mat img;

	while (1) {
		if (cap.isOpened())//�ж�����ͷ�Ƿ��
		{
			cap.read(img);
			//�ж�ͼ���Ƿ��ȡ
			if (img.empty()) {
				printf("NO Image");
				return -1;
			}
			imshow("img", img);
			
			//����FPS
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
