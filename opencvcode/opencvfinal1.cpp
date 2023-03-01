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


    Mat labels = Mat::zeros(img.size(), CV_32S);// labels为标记图像（背景index=0），非背景区域的连通域序号为下标值逐个加一
    Mat stats, centroids;//stats状态矩阵（size=连通域数*5），centroids连通域中心的矩阵（size=连通域数*2）
    // stats ：每一个标记的统计信息，是一个5列的矩阵，每一行对应每个连通区域的外接矩形的x、y、width、height和面积
    int num_labels = connectedComponentsWithStats(mask, labels, stats, centroids, 8);
    //printf("total labels : %d\n", (num_labels - 1));

    for (int i = 1; i < num_labels; i++) {
        Vec2d pt = centroids.at<Vec2d>(i, 0);
        int x = stats.at<int>(i, CC_STAT_LEFT);//stats.at<int>(连通域序号,状态):
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > 500 && area < 3500)
            circle(img, Point(pt[0], pt[1]), width/2, Scalar(0, 255, 255), -1);//连通域圆（黄色）
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