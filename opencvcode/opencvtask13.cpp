#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void Canny_demo(Mat img, Mat& result, int lowThres, int highThres);

int main(int argc, char** argv)
{
	Mat img = imread("C:/Users/24816/Desktop/image/xyy.jpg");
	if (img.empty()) {
		printf("could't find image file");
		return -1;
	}
	Mat dst;
	Canny_demo(img, dst, 25, 75);
	imshow("img", img);
	imshow("myCanny", dst);

	while (1)
		if (waitKey(1) == 32)
			break;
	return 0;
}

void Canny_demo(Mat img, Mat& result, int lowThres, int highThres)
{
	// 1. 高斯模糊
	Mat gauImg;
	GaussianBlur(img, gauImg, Size(5, 5), 0);
	//imshow("gaussian", gauImg);

	Mat grayImg;
	if (gauImg.channels() == 3)
		cvtColor(gauImg, grayImg, COLOR_BGR2GRAY);
	else
		grayImg = gauImg.clone();
	//imshow("gray", grayImg);



	//2.计算梯度角和幅度
	Mat sobxImg, sobyImg, angleImg_orig, angleImg, magImg;
	Sobel(grayImg, sobxImg, CV_32FC1, 1, 0);//x方向一阶微分
	Sobel(grayImg, sobyImg, CV_32FC1, 0, 1);//y方向一阶微分

	//imshow("sobel x", sobxImg);
	//imshow("sobel y", sobyImg);

	cartToPolar(sobxImg, sobyImg, magImg, angleImg_orig, true);
	//函数实现梯度幅值和角度的计算，magImg梯度幅值图像,angleImg_orig角度图像,true为角度表示，false为弧度表示


   //对角度图像angleImg_orig中每一角度值从0到360角度值归一化到[0, 45, 90, 135]这四个值于 angleImg中
	angleImg = Mat(angleImg_orig.size(), CV_8UC1);
	for (int row = 0; row < angleImg_orig.rows; row++)
	{
		for (int col = 0; col < angleImg_orig.cols; col++)
		{		//337.5度到360度 0度到22.5度 157.5度到202.5 归一化成0度
			if ((angleImg_orig.at<float>(row, col) >= 0 && angleImg_orig.at<float>(row, col) < 22.5) ||
				(angleImg_orig.at<float>(row, col) >= 337.5 && angleImg_orig.at<float>(row, col) < 360) ||
				angleImg_orig.at<float>(row, col) >= 157.5 && angleImg_orig.at<float>(row, col) < 202.5)
			{
				angleImg.at<uchar>(row, col) = 0;
			}
			else if ((angleImg_orig.at<float>(row, col) >= 22.5 && angleImg_orig.at<float>(row, col) < 67.5) ||
				(angleImg_orig.at<float>(row, col) >= 202.5 && angleImg_orig.at<float>(row, col) < 247.5))
			{
				angleImg.at<uchar>(row, col) = 45;
			}
			else if ((angleImg_orig.at<float>(row, col) >= 67.5 && angleImg_orig.at<float>(row, col) < 112.5) ||
				(angleImg_orig.at<float>(row, col) >= 247.5 && angleImg_orig.at<float>(row, col) < 292.5))
			{
				angleImg.at<uchar>(row, col) = 90;
			}
			else
			{
				angleImg.at<uchar>(row, col) = 135;
			}
		}
	}


	//3. 非（梯度幅值的）极大值抑制 -- nms
	Mat nmsImg = Mat::zeros(magImg.size(), CV_8UC1);//储存nms后的像素值图像
	Mat nmsMagImg = Mat::zeros(magImg.size(), CV_32FC1);//储存nms后的梯度幅值图像
	for (int row = 1; row < nmsImg.rows - 1; row++) //邻域中心从正数第二行到倒数第二行
	{
		for (int col = 1; col < nmsImg.cols - 1; col++) //邻域中心从正数第二列到倒数第二列
		{

			if (angleImg.at<uchar>(row, col) == 0 &&//角度为0 检查中心点是否是同一行三点中（梯度幅值的）最大值
				(magImg.at<float>(row, col) > magImg.at<float>(row, col - 1) &&
					magImg.at<float>(row, col) > magImg.at<float>(row, col + 1)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);//是的话幅度梯度不变否则为0
				nmsImg.at<uchar>(row, col) = 255;//是的话像素值为255否则为0
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 45 &&//角度为45 检查中心点是否是45°直线中的三点中（梯度幅值的）最大值
				(magImg.at<float>(row, col) > magImg.at<float>(row - 1, col + 1) &&
					magImg.at<float>(row, col) > magImg.at<float>(row + 1, col - 1)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);
				nmsImg.at<uchar>(row, col) = 255;
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 90 &&//角度为90 检查中心点是否是同一列三点中（梯度幅值的）最大值
				(magImg.at<float>(row, col) > magImg.at<float>(row - 1, col) &&
					magImg.at<float>(row, col) > magImg.at<float>(row + 1, col)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);
				nmsImg.at<uchar>(row, col) = 255;
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 135 &&//角度为135 检查中心点是否是135°直线上的三点中（梯度幅值的）最大值
				(magImg.at<float>(row, col) > magImg.at<float>(row - 1, col - 1) &&
					magImg.at<float>(row, col) > magImg.at<float>(row + 1, col + 1)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);
				nmsImg.at<uchar>(row, col) = 255;
				continue;
			}

		}
	}
	imshow("nms", nmsImg);
	imshow("nms mag", nmsMagImg);


	//4. 利用高、低阈值（双阈值）判断是否为边缘像素点
	//先判断强边缘点
	result = nmsImg.clone();
	for (int row = 1; row < result.rows - 1; row++)//邻域中心横坐标从正数第二行到倒数第二行
	{
		for (int col = 1; col < result.cols - 1; col++)//邻域中心纵坐标从正数第二列到倒数第二列
		{
			if (nmsMagImg.at<float>(row, col) >= highThres)//（nmu后）梯度幅值比高阈值大，一定是边缘点
			{
				result.at<uchar>(row, col) = 255;
			}
			if (nmsMagImg.at<float>(row, col) < lowThres)//梯度幅值比高阈值小，一定不是边缘点
			{
				result.at<uchar>(row, col) = 0;
			}
		}
	}
	//imshow("strong edge", result);

	// 再判断中等边缘点（梯度幅值在双阈值之间）
	for (int row = 1; row < result.rows - 1; row++)
	{
		for (int col = 1; col < result.cols - 1; col++)
		{
			if (nmsMagImg.at<float>(row, col) >= lowThres && nmsMagImg.at<float>(row, col) < highThres)//如果梯度幅值在双阈值之间
			{   //如果在8邻域内有边缘点，则认为中心点为边缘点
				if (result.at<uchar>(row - 1, col - 1) == 255 || result.at<uchar>(row - 1, col) == 255 ||
					result.at<uchar>(row - 1, col + 1) == 255 || result.at<uchar>(row, col + 1) == 255 ||
					result.at<uchar>(row + 1, col + 1) == 255 || result.at<uchar>(row + 1, col) == 255 ||
					result.at<uchar>(row + 1, col - 1) == 255 || result.at<uchar>(row, col - 1) == 255)
				{
					result.at<uchar>(row, col) = 255;
				}
				else
				{
					result.at<uchar>(row, col) = 0;
				}
			}
		}
	}
	//imshow("myCanny", result);

}