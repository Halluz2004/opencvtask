#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 0, smin = 0, vmin = 59;
int hmax = 179, smax = 255, vmax = 130;


int main(int argc, char** argv) {
    string path = "Resources/final1.jpg";
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
        Vec2d pt = centroids.at<Vec2d>(i, 0);
        int x = stats.at<int>(i, CC_STAT_LEFT);//stats.at<int>(��ͨ�����,״̬):
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > 500 && area < 3500)
            circle(img, Point(pt[0], pt[1]), width/2, Scalar(0, 255, 255), -1);//��ͨ��Բ����ɫ��
    }
  
    imshow("dst", img);
    Rect ROI1(263, 50, 80, 80);
    Mat imgROI1 = img(ROI1);
    imshow("ROI1", imgROI1);

    Rect ROI2(287, 189, 60, 60);
    Mat imgROI2 = img(ROI2);
    imshow("ROI2", imgROI2);

    Rect ROI3(346, 289, 100, 100);
    Mat imgROI3 = img(ROI3);
    imshow("ROI3", imgROI3);

    Rect ROI4(489, 340, 60, 60);
    Mat imgROI4 = img(ROI4);
    imshow("ROI4", imgROI4);


    waitKey(0);
    return 0;
}