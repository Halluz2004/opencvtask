#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 0, smin = 0 , vmin = 172;
int hmax = 179, smax = 255, vmax = 255;


int main(int argc, char** argv) {
	string path = "Resources/final2.jpg";
	Mat img = imread(path);
	Mat imgHSV, mask;
	//imshow("img", img);
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);

	inRange(imgHSV, lower, upper, mask);
	//imshow("mask", mask);


    Mat labels = Mat::zeros(img.size(), CV_32S);// labelsΪ���ͼ�񣨱���index=0�����Ǳ����������ͨ�����Ϊ�±�ֵ�����һ
    Mat stats, centroids;//stats״̬����size=��ͨ����*5����centroids��ͨ�����ĵľ���size=��ͨ����*2��
    // stats ��ÿһ����ǵ�ͳ����Ϣ����һ��5�еľ���ÿһ�ж�Ӧÿ����ͨ�������Ӿ��ε�x��y��width��height�����
    int num_labels = connectedComponentsWithStats(mask, labels, stats, centroids, 8);
    //printf("total labels : %d\n", (num_labels - 1));

    for (int i = 1; i < num_labels; i++) {
        int x = stats.at<int>(i, CC_STAT_LEFT);//stats.at<int>(��ͨ�����,״̬):
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > 1000)
            rectangle(img, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);//��Ӿ��Σ���ɫ��
    }

    imshow("dst", img);

    Rect ROI1(168, 163, 100, 100);
    Mat imgROI1 = img(ROI1);
    imshow("ROI1", imgROI1);

	waitKey(0);
	return 0;
}