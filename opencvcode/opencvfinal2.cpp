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


    Mat labels = Mat::zeros(img.size(), CV_32S);// labels为标记图像（背景index=0），非背景区域的连通域序号为下标值逐个加一
    Mat stats, centroids;//stats状态矩阵（size=连通域数*5），centroids连通域中心的矩阵（size=连通域数*2）
    // stats ：每一个标记的统计信息，是一个5列的矩阵，每一行对应每个连通区域的外接矩形的x、y、width、height和面积
    int num_labels = connectedComponentsWithStats(mask, labels, stats, centroids, 8);
    //printf("total labels : %d\n", (num_labels - 1));

    for (int i = 1; i < num_labels; i++) {
        int x = stats.at<int>(i, CC_STAT_LEFT);//stats.at<int>(连通域序号,状态):
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > 1000)
            rectangle(img, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);//外接矩形（紫色）
    }

    imshow("dst", img);

    Rect ROI1(168, 163, 100, 100);
    Mat imgROI1 = img(ROI1);
    imshow("ROI1", imgROI1);

	waitKey(0);
	return 0;
}