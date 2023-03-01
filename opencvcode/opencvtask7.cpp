#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat gammaTransform(Mat img, float gamma)
{
	float k = 1.0f / gamma;

	//制表提高效率
	Mat lut(1, 256, CV_8UC1);//制作一个Mat型的表（单通道） 储存gamma矫正后对应的像素值
	uchar* p = lut.data;//lut.data为首地址
	for (int i = 0; i < 256; i++) {
		float f = (i + 0.5f) / 256;//归一化
		f = (float)(pow(f, k));//预补偿
		p[i] = saturate_cast<uchar>(f * 256.0f - 0.5f);//反归一化
	}//得到每一像素值“归一化、预补偿、反归一化”后对应的像素值

	Mat dst;
	LUT(img, lut, dst);
	return dst;
}

int main(int argc, char** argv)
{ 
	Mat img = imread("Resources/7-1.png");
	Mat imgGamma = gammaTransform(img, 2.2);//设定gamma值为2.2

	imshow("img", img);
	imshow("imgGamma", imgGamma);//gamma大于1增加图片像素低的部分的对比度，减小图片像素高的部分的对比度 
	//观察到图片黑的地方可以看得更清晰，图片亮的地方会变得更模糊，可以使得 较暗看不清的图片 清晰
	
	while (1){
		if (waitKey(1) == 32)
			break;
	}

	return 0;

}