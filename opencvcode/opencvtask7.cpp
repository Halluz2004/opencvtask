#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat gammaTransform(Mat img, float gamma)
{
	float k = 1.0f / gamma;

	//�Ʊ����Ч��
	Mat lut(1, 256, CV_8UC1);//����һ��Mat�͵ı���ͨ���� ����gamma�������Ӧ������ֵ
	uchar* p = lut.data;//lut.dataΪ�׵�ַ
	for (int i = 0; i < 256; i++) {
		float f = (i + 0.5f) / 256;//��һ��
		f = (float)(pow(f, k));//Ԥ����
		p[i] = saturate_cast<uchar>(f * 256.0f - 0.5f);//����һ��
	}//�õ�ÿһ����ֵ����һ����Ԥ����������һ�������Ӧ������ֵ

	Mat dst;
	LUT(img, lut, dst);
	return dst;
}

int main(int argc, char** argv)
{ 
	Mat img = imread("Resources/7-1.png");
	Mat imgGamma = gammaTransform(img, 2.2);//�趨gammaֵΪ2.2

	imshow("img", img);
	imshow("imgGamma", imgGamma);//gamma����1����ͼƬ���ص͵Ĳ��ֵĶԱȶȣ���СͼƬ���ظߵĲ��ֵĶԱȶ� 
	//�۲쵽ͼƬ�ڵĵط����Կ��ø�������ͼƬ���ĵط����ø�ģ��������ʹ�� �ϰ��������ͼƬ ����
	
	while (1){
		if (waitKey(1) == 32)
			break;
	}

	return 0;

}