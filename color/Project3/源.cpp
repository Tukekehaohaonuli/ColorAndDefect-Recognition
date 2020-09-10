#include <opencv2/opencv.hpp>
#include <iostream>   
#include <string>   

using namespace cv;
using namespace std;

//��Ҫ����ĸ���
const int nClusters = 4;

enum Mode { RGB, HSV, YCRCB, LAB };

void nameColor(cv::Mat matHsv)
{
	vector<int> colorVec;
	colorVec.push_back(matHsv.at<uchar>(0, 0));
	colorVec.push_back(matHsv.at<uchar>(0, 1));
	colorVec.push_back(matHsv.at<uchar>(0, 2));

	if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 255)
		&& (colorVec[2] >= 0 && colorVec[2] <= 46))
	{

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 43)
		&& (colorVec[2] >= 46 && colorVec[2] <= 220)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 0 && colorVec[0] <= 180)
		&& (colorVec[1] >= 0 && colorVec[1] <= 30)
		&& (colorVec[2] >= 221 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if (((colorVec[0] >= 0 && colorVec[0] <= 10) || (colorVec[0] >= 156 && colorVec[0] <= 180))
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;

	}
	else if ((colorVec[0] >= 11 && colorVec[0] <= 25)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 26 && colorVec[0] <= 34)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 35 && colorVec[0] <= 77)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 78 && colorVec[0] <= 99)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 100 && colorVec[0] <= 124)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else if ((colorVec[0] >= 125 && colorVec[0] <= 155)
		&& (colorVec[1] >= 43 && colorVec[1] <= 255)
		&& (colorVec[2] >= 46 && colorVec[2] <= 255)) {

		cout << "��" << endl;
	}
	else {

		cout << "δ֪" << endl;
	}
}

int main(int argc, char* argv[])
{
	Mat img;

	//��ȡͼ��
	img = imread("D:/project data/ColorSeg/ColorImg/7.jpg");
	namedWindow("original", CV_WINDOW_NORMAL);
	imshow("original", img);

	// //ֱ��ͼ��ǿ
	// Mat rgb[3];
	// split(img, rgb);  
	// for (int i = 0; i < 3; i++)  
	// {  
	//     equalizeHist(rgb[i], rgb[i]);  
	// }  
	// merge(rgb, 3, img);

	Mat src;

	//ѡ��ͼ��任��ʽ
	Mode mode;
	mode = Mode::HSV;

	//ͼ��任
	switch (mode)
	{
	case Mode::RGB:
		src = img.clone();
		break;
	case Mode::HSV:
		cv::cvtColor(img, src, cv::COLOR_BGR2HSV);
		break;
	case Mode::YCRCB:
		cv::cvtColor(img, src, cv::COLOR_BGR2YCrCb);
		break;
	case Mode::LAB:
		cv::cvtColor(img, src, cv::COLOR_BGR2Lab);
		break;
	}

	blur(src, src, Size(20, 20));

	//��������  ���ص���� �� ����ά��
	Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);

	vector<Mat> bgr;
	split(src, bgr);

	// for(int i = 0; i < 3; i++)
	// {
	//     equalizeHist(bgr[i], bgr[i]);
	// }

	//������������
	for (int i = 0; i < src.cols*src.rows; i++)
	{
		p.at<float>(i, 0) = (i / src.cols) / src.rows;
		p.at<float>(i, 1) = (i%src.cols) / src.cols;
		p.at<float>(i, 2) = bgr[0].data[i] / 255.0;         //b
		p.at<float>(i, 3) = bgr[1].data[i] / 255.0;         //g
		p.at<float>(i, 4) = bgr[2].data[i] / 255.0;         //r
	}

	Mat bestLabels, centers, clustered;

	//����
	kmeans(p, nClusters, bestLabels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);

	vector<Mat> segImg;
	segImg.resize(nClusters);

	vector<Mat> colorImg;
	colorImg.resize(nClusters);

	//��ʼ��
	for (int i = 0; i < segImg.size(); i++)
	{
		segImg[i] = Mat::zeros(src.rows, src.cols, CV_8UC3);
		//colorImg[i] = Mat::zeros(src.rows, src.cols, CV_8UC3);
		colorImg[i] = Mat::zeros(90000, 1, CV_8UC3);
	}
	//�������Ժ�Ľ�����洢��ͼƬsegImg��
	for (int i = 0; i < src.cols * src.rows; i++)
	{
		int index = bestLabels.at<int>(i, 0);

		int y = i / src.cols;
		int x = i % src.cols;

		segImg[index].at<Vec3b>(y, x) = img.at<Vec3b>(y, x);
	}

	//
	int rr = 0;
	cv::Vec3b a(0, 0, 0);
	for (int i = 0; i < segImg.size(); i++)
	{
		cv::Vec3b color;

		//ÿ��ͼƬ
		for (int j = 0; j < segImg[i].rows; j++)
		{

			for (int k = 0; k < segImg[i].cols; k++)
			{
				if (segImg[i].at<Vec3b>(j, k) != a)
				{
					//color = segImg[i].at<Vec3b>(j,k);
					if (rr < 90000)
					{
						colorImg[i].at<Vec3b>(rr, 0) = segImg[i].at<Vec3b>(j, k);
						rr++;
					}
				}
			}
		}

		// for(int cc = 0; cc < colorImg[i].rows; cc++)
		// {
		//     for(int rr = 0; rr < colorImg[i].cols; rr++)
		//     {
		//         colorImg[i].at<Vec3b>(cc, rr) = color;
		//     }
		// }

		rr = 0;
	}

	for (int i = 0; i < colorImg.size(); i++)
	{
		Size size = Size(300, 300);
		resize(colorImg[i], colorImg[i], size);
	}

	//�������ɵ�4�ֽ����ͼƬ��waitKey(0)��ʾ�ȵ��ո�������Ժ��ͷ�
	for (int i = 0; i < segImg.size(); i++)
	{
		Mat matHsv;
		cvtColor(colorImg[i], matHsv, COLOR_BGR2HSV);

		nameColor(matHsv);


		namedWindow("Img", CV_WINDOW_NORMAL);
		//namedWindow("Img");
		imshow("Img", colorImg[i]);
		namedWindow("main", CV_WINDOW_NORMAL);
		imshow("main", segImg[i]);
		waitKey(0);
	}

	return 0;
}


