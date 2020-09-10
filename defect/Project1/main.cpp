#include <opencv2/opencv.hpp>  
#include <iostream>   
#include <string>   

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat roiImage, roiImageGray;

	//��ȡͼ��
	roiImage = imread("D:/project data/ColorSeg/roi/2.jpg");
	namedWindow("Original", CV_WINDOW_NORMAL);
	imshow("Original", roiImage);
	//�Ҷ�ͼ��
	cvtColor(roiImage, roiImageGray, CV_BGR2GRAY);

	// //ֱ��ͼ���⻯
	// equalizeHist(roiImageGray, roiImageGray); 

	//Canny��ȡ
	Mat CannyEdge;
	Canny(roiImageGray, CannyEdge, 30, 150);
	//Canny(roiImageGray, CannyEdge, 100, 150);

	//���� ��ʴ
	Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));
	//Mat element = getStructuringElement(MORPH_RECT,Size(15, 15));
	morphologyEx(CannyEdge, CannyEdge, MORPH_CLOSE, element);

	Mat element1 = getStructuringElement(MORPH_RECT, Size(3, 1));
	morphologyEx(CannyEdge, CannyEdge, MORPH_ERODE, element1);

	//��������
	vector<vector<Point> > contours;
	findContours(CannyEdge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	//drawContours(roiImage, contours, -1, Scalar(0, 255, 0), 1, 8);

	vector<vector<Point> > contours1;
	for (int i = 0; i <= contours.size() - 1; i++)
	{   
		//�����������
		float area = contourArea(contours[i]);
		//������������300,����
		if (area > 300)
		{
			contours1.push_back(contours[i]);
		}
	}
	//��������������ɫ��
	drawContours(roiImage, contours1, -1, Scalar(0, 255, 0), 5, 8);


	for (int i = 0; i <= contours1.size() - 1; i++)
	{   
		//������Ӿ���
		Rect rect = boundingRect(contours1[i]);
		int area = int(contourArea(contours1[i]));

		rectangle(roiImage, rect, Scalar(0, 0, 255), 5, 8);
		putText(roiImage, to_string(area), Point(rect.x - 20 + rect.width / 2, rect.y - 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
	}


	namedWindow("Test", CV_WINDOW_NORMAL);
	imshow("Test", roiImage);

	namedWindow("Image", CV_WINDOW_NORMAL);
	imshow("Image", CannyEdge);
	waitKey(0);

	return 0;
}
