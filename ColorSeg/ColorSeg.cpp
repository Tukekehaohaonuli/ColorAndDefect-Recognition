#include "opencv2/opencv.hpp"   
#include <iostream>   
#include <string>   
   
using namespace cv;   
using namespace std;   

//需要聚类的个数
const int nClusters = 4;  

enum Mode { RGB, HSV, YCRCB, LAB };
   
int main(int argc, char* argv[])   
{   
    Mat img;
    
	//读取图像
    img = imread("D:\project data\ColorSeg\ColorImg\8.jpg"); 
    imshow("original", img);

    // //直方图增强
    // Mat rgb[3];
    // split(img, rgb);  
    // for (int i = 0; i < 3; i++)  
    // {  
    //     equalizeHist(rgb[i], rgb[i]);  
    // }  
    // merge(rgb, 3, img);
    
    Mat src;

    //选择图像变换方式
	Mode mode;
    mode = Mode::HSV;

	//图像变换
	switch (mode) 
	{
	case Mode::RGB:
        src = img.clone();
		break;
	case Mode::HSV:
		cv::cvtColor(img, src, cv::COLOR_BGR2HSV );
		break;
	case Mode::YCRCB:
		cv::cvtColor(img, src, cv::COLOR_BGR2YCrCb );
		break;
	case Mode::LAB:
		cv::cvtColor(img, src, cv::COLOR_BGR2Lab );
		break;
	}

    blur(src, src, Size(20,20));
    
	//特征向量  像素点个数 × 特征维度
    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);   
    
    vector<Mat> bgr;   
    split(src, bgr);

    // for(int i = 0; i < 3; i++)
    // {
    //     equalizeHist(bgr[i], bgr[i]);
    // }

	//生成特征向量
    for(int i=0; i<src.cols*src.rows; i++)    
    {   
        p.at<float>(i,0) = (i/src.cols) / src.rows;   
        p.at<float>(i,1) = (i%src.cols) / src.cols; 
        p.at<float>(i,2) = bgr[0].data[i] / 255.0;         //b
        p.at<float>(i,3) = bgr[1].data[i] / 255.0;         //g
        p.at<float>(i,4) = bgr[2].data[i] / 255.0;         //r
    }   
    
    Mat bestLabels, centers, clustered; 

    //聚类
    kmeans(p, nClusters, bestLabels, TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),3, KMEANS_PP_CENTERS, centers); 

    vector<Mat> segImg;
    segImg.resize(nClusters);

    //初始化
    for(int i = 0; i < segImg.size(); i++)
    {
        segImg[i] = Mat::zeros(src.rows, src.cols, CV_8UC3);
    }
    
    for(int i = 0; i < src.cols * src.rows; i++)
    {
        int index = bestLabels.at<int>(i, 0);

        int y = i / src.cols;
        int x = i % src.cols;

        segImg[index].at<Vec3b>(y,x) = img.at<Vec3b>(y,x);
    }

    for(int i = 0; i < segImg.size(); i++)
    {   
        namedWindow("Img", CV_WINDOW_NORMAL);
        imshow("Img", segImg[i]);
        waitKey(0);
    }

    return 0;   
}  

 