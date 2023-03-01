#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

RNG rng(12345);//用于随机颜色 种子为12345
void connected_component_stats(Mat& image);

int main(int argc, char** argv) {
    Mat src = imread("Resources/11.png");
    if (src.empty()) {
        printf("could not load image...\n");
    }
    imshow("src", src);

    connected_component_stats(src);

    while (1)
        if (waitKey(1) == 32)
            break;
    return 0;
}

void connected_component_stats(Mat& image) {
    // 二值化
    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);////THRESH_OTSU自适应取阈值（省去了自己调阈值的麻烦），而第三个参数手动取的阈值不生效
    //imshow("binary", binary);

    Mat labels = Mat::zeros(image.size(), CV_32S);// labels为标记图像（背景index=0），非背景区域的连通域序号为下标值逐个加一
    Mat stats, centroids;//stats状态矩阵（size=连通域数*5），centroids连通域中心的矩阵（size=连通域数*2）
    // stats ：每一个标记的统计信息，是一个5列的矩阵，每一行对应每个连通区域的外接矩形的x、y、width、height和面积
    int num_labels = connectedComponentsWithStats(binary, labels, stats, centroids, 8);
    

    //得到每个连通域的颜色
    vector<Vec3b> colors(num_labels);
    // background color黑色
    colors[0] = Vec3b(0, 0, 0);
    // object color绿色
    for (int i = 1; i < num_labels; i++) {
        colors[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    }

    // 得到结果图像
    Mat dst = Mat::zeros(image.size(), image.type());
    int w = image.cols;
    int h = image.rows;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            int label = labels.at<int>(row, col);//将各个连通域序号赋给label
            int area = stats.at<int>(label, CC_STAT_AREA);
            if (label == 0 || area > 10000 || area < 1500) continue;//保留回形针尾
            dst.at<Vec3b>(row, col) = colors[label];//将该连通域所属颜色赋给dst图像
        }
    }

    int count = 0;//计回形针数
    for (int i = 1; i < num_labels; i++) {
        Vec2d pt = centroids.at<Vec2d>(i, 0);
        int x = stats.at<int>(i, CC_STAT_LEFT);//stats.at<int>(连通域序号,状态):
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area < 50000 && area>1500) {
           // printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
            rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);//外接矩形（紫色）
            count++;
        }
    }
    printf("total labels : %d\n",count);
    imshow("dst", dst);
}

