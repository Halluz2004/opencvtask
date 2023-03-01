#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img = imread("C:/Users/24816/Desktop/image/xyy.jpg");//��ȡͼƬ
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img_before", img);
	
	//����ÿ������ 
	for (int i = 0; i < img.rows; i++) {
		uchar* p_row = img.ptr<uchar>(i);//p_rowָ��ÿ�е���Ԫ�صĵ�ַ
		for (int j = 0; j < img.cols ; j++) {
			//ȡƽ��ֵ����ֵ
			uchar average = (p_row[3 * j] + p_row[3 * j + 1] + p_row[3 * j + 2]) / 3;
			p_row[3 * j] = average;
			p_row[3 * j + 1] = average;
			p_row[3 * j + 2] = average;
		}
	}
	imshow("img_after", img);//�۲쵽ͼ���ɻҶ�ͼ������ͨ��ֵ��ͬ��
	
	//���¿ո�ֹͣ
	while (1)
		if (waitKey(1) == 32)
			break;
	
	return 0;
}