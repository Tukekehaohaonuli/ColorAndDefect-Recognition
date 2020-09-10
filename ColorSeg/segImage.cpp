#include "opencv2/opencv.hpp"   
#include <iostream>   
#include <string>   
   
using namespace cv;   
using namespace std;   

Point p1, p2;

bool isSelect = false;
bool flag = false;

void onMouse(int event, int x, int y, int, void *param)
{   
    Mat& tmp = *(Mat *)param;
    if(event == CV_EVENT_LBUTTONDOWN)
    {   
        if(!isSelect && !flag)
        {
            p1 = Point(x,y);
            circle(tmp, p1, 10, Scalar(255, 0, 0), -1);
            isSelect = true;

            imshow("Image", tmp);
        }
        else
        {   
            p2 = Point(x,y);
            circle(tmp, p2, 10, Scalar(255, 0, 0), -1);
            rectangle(tmp, p1, p2, Scalar(255, 0, 0), 3);

            imshow("Image", tmp);

            flag = true;
        }
    }
}


int main(int argc, char* argv[])   
{   
    Mat img, imgShow;

	//读取图像
    img = imread("/home/usr/ColorSeg/ColorImg/1.jpg"); 
    imgShow = img.clone();

    //选取ROI区域
    namedWindow("Image", CV_WINDOW_NORMAL);
	setMouseCallback("Image", onMouse, &imgShow);
    
    imshow("Image", imgShow);
    waitKey(0);

    Mat roiImage, roiImageGray;

    if(flag)
    {
        roiImage = img(cv::Rect(p1, p2));

        namedWindow("Select ROI", CV_WINDOW_NORMAL);
        imshow("Select ROI", roiImage);
        waitKey(0);
    }
    else
    {
        cout << "No Roi Image!" << endl;
    }

    imwrite("r1.png", roiImage);

    // destroyWindow("Image");
    // destroyWindow("Select ROI");

    // //灰度图像
    // cvtColor(roiImage, roiImageGray, CV_BGR2GRAY);
    
    // //直方图均衡化
    // equalizeHist(roiImageGray, roiImageGray); 

    // //Canny提取
    // Mat CannyEdge;
    // Canny(roiImageGray, CannyEdge, 200, 250);

    
    
    
    // namedWindow("Test", CV_WINDOW_NORMAL);
    // imshow("Test", roiImageGray);
    
    // namedWindow("Image", CV_WINDOW_NORMAL);
    // imshow("Image", CannyEdge);
    // waitKey(0);

    return 0;   
}  
