#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void remove_acne(Mat src)
{
	// ת��Ϊ�Ҷ�ͼ��
	Mat imgGray;
	cvtColor(src, imgGray, COLOR_BGR2GRAY);

	//��ֵ�˲�-ȥ������
	Mat imgBlur(src.size(), src.type());
	GaussianBlur(imgGray, imgBlur, Size(), 2.0);

	// OTSU threshold���Ҷ�
	Mat imgThreshold;
	threshold(imgBlur, imgThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// acne maskӦ����̬ѧ����ϸ��������������
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat acneMask;
	morphologyEx(imgThreshold, acneMask, MORPH_OPEN, kernel);
	imshow("Acne Mask", acneMask);

	// inpaint��ԭ
	inpaint(src, acneMask, src, 10, INPAINT_TELEA);

}
int main(int argc, char** argv) {
	Mat img = imread("Resources/14-1.jpg");
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	imshow("img_before", img);
	Rect ROI(100, 350, 200, 100);
	Mat imgROI = img(ROI);
	imshow("ROI", imgROI);

	remove_acne(imgROI);
	imshow("img_after", img);

	//���¿ո�ֹͣ
	while (1)
		if (waitKey(1) == 32)
			break;

	return 0;
}