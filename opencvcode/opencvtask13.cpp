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
	// 1. ��˹ģ��
	Mat gauImg;
	GaussianBlur(img, gauImg, Size(5, 5), 0);
	//imshow("gaussian", gauImg);

	Mat grayImg;
	if (gauImg.channels() == 3)
		cvtColor(gauImg, grayImg, COLOR_BGR2GRAY);
	else
		grayImg = gauImg.clone();
	//imshow("gray", grayImg);



	//2.�����ݶȽǺͷ���
	Mat sobxImg, sobyImg, angleImg_orig, angleImg, magImg;
	Sobel(grayImg, sobxImg, CV_32FC1, 1, 0);//x����һ��΢��
	Sobel(grayImg, sobyImg, CV_32FC1, 0, 1);//y����һ��΢��

	//imshow("sobel x", sobxImg);
	//imshow("sobel y", sobyImg);

	cartToPolar(sobxImg, sobyImg, magImg, angleImg_orig, true);
	//����ʵ���ݶȷ�ֵ�ͽǶȵļ��㣬magImg�ݶȷ�ֵͼ��,angleImg_orig�Ƕ�ͼ��,trueΪ�Ƕȱ�ʾ��falseΪ���ȱ�ʾ


   //�ԽǶ�ͼ��angleImg_orig��ÿһ�Ƕ�ֵ��0��360�Ƕ�ֵ��һ����[0, 45, 90, 135]���ĸ�ֵ�� angleImg��
	angleImg = Mat(angleImg_orig.size(), CV_8UC1);
	for (int row = 0; row < angleImg_orig.rows; row++)
	{
		for (int col = 0; col < angleImg_orig.cols; col++)
		{		//337.5�ȵ�360�� 0�ȵ�22.5�� 157.5�ȵ�202.5 ��һ����0��
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


	//3. �ǣ��ݶȷ�ֵ�ģ�����ֵ���� -- nms
	Mat nmsImg = Mat::zeros(magImg.size(), CV_8UC1);//����nms�������ֵͼ��
	Mat nmsMagImg = Mat::zeros(magImg.size(), CV_32FC1);//����nms����ݶȷ�ֵͼ��
	for (int row = 1; row < nmsImg.rows - 1; row++) //�������Ĵ������ڶ��е������ڶ���
	{
		for (int col = 1; col < nmsImg.cols - 1; col++) //�������Ĵ������ڶ��е������ڶ���
		{

			if (angleImg.at<uchar>(row, col) == 0 &&//�Ƕ�Ϊ0 ������ĵ��Ƿ���ͬһ�������У��ݶȷ�ֵ�ģ����ֵ
				(magImg.at<float>(row, col) > magImg.at<float>(row, col - 1) &&
					magImg.at<float>(row, col) > magImg.at<float>(row, col + 1)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);//�ǵĻ������ݶȲ������Ϊ0
				nmsImg.at<uchar>(row, col) = 255;//�ǵĻ�����ֵΪ255����Ϊ0
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 45 &&//�Ƕ�Ϊ45 ������ĵ��Ƿ���45��ֱ���е������У��ݶȷ�ֵ�ģ����ֵ
				(magImg.at<float>(row, col) > magImg.at<float>(row - 1, col + 1) &&
					magImg.at<float>(row, col) > magImg.at<float>(row + 1, col - 1)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);
				nmsImg.at<uchar>(row, col) = 255;
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 90 &&//�Ƕ�Ϊ90 ������ĵ��Ƿ���ͬһ�������У��ݶȷ�ֵ�ģ����ֵ
				(magImg.at<float>(row, col) > magImg.at<float>(row - 1, col) &&
					magImg.at<float>(row, col) > magImg.at<float>(row + 1, col)))
			{
				nmsMagImg.at<float>(row, col) = magImg.at<float>(row, col);
				nmsImg.at<uchar>(row, col) = 255;
				continue;
			}

			if (angleImg.at<uchar>(row, col) == 135 &&//�Ƕ�Ϊ135 ������ĵ��Ƿ���135��ֱ���ϵ������У��ݶȷ�ֵ�ģ����ֵ
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


	//4. ���øߡ�����ֵ��˫��ֵ���ж��Ƿ�Ϊ��Ե���ص�
	//���ж�ǿ��Ե��
	result = nmsImg.clone();
	for (int row = 1; row < result.rows - 1; row++)//�������ĺ�����������ڶ��е������ڶ���
	{
		for (int col = 1; col < result.cols - 1; col++)//��������������������ڶ��е������ڶ���
		{
			if (nmsMagImg.at<float>(row, col) >= highThres)//��nmu���ݶȷ�ֵ�ȸ���ֵ��һ���Ǳ�Ե��
			{
				result.at<uchar>(row, col) = 255;
			}
			if (nmsMagImg.at<float>(row, col) < lowThres)//�ݶȷ�ֵ�ȸ���ֵС��һ�����Ǳ�Ե��
			{
				result.at<uchar>(row, col) = 0;
			}
		}
	}
	//imshow("strong edge", result);

	// ���ж��еȱ�Ե�㣨�ݶȷ�ֵ��˫��ֵ֮�䣩
	for (int row = 1; row < result.rows - 1; row++)
	{
		for (int col = 1; col < result.cols - 1; col++)
		{
			if (nmsMagImg.at<float>(row, col) >= lowThres && nmsMagImg.at<float>(row, col) < highThres)//����ݶȷ�ֵ��˫��ֵ֮��
			{   //�����8�������б�Ե�㣬����Ϊ���ĵ�Ϊ��Ե��
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