//
//  main.cpp
//  HoughTansform 综合霍夫变换---霍夫线变换&霍夫圆变换
//
//  Created by ZHHJemotion on 2016/10/25.
//  Copyright © 2016年 Lukas_Zhang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#define PATH string("/Users/zhangxingjian/Desktop/Programming/C++/OpenCV/opencv_HoughTransform/HoughTansform/HoughTansform/")

using namespace cv;
using namespace std;

//------------------------------- the global variables delaration section ----------------------------
//   Describle: 全局变量声明
//----------------------------------------------------------------------------------------------------
Mat srcImage, midImage, dstImage;// 原始图，中间临时图，效果图
vector<Vec4i> lines;//定义一个矢量结构 lines 用于存放得到的线段矢量集合。Vec4i为4通道int型数据，正好符合HoughLines返回的4个参数，即两个Point的坐标。
vector<Vec3f> circes;//定义一个矢量结构 circles 用于存放得到的圆参数集合
int onThreshold1 = 100;// 变量接收的 TrackBar 的位置参数 霍夫线变换
int onThreshold2 = 150;// 霍夫圆变换

//------------------------------- the global functions delaration section ---------------------------
//    Desscribe: 全局函数声明
//---------------------------------------------------------------------------------------------------
static void onHoughLines(int, void *);// 回调函数--霍夫线变换
static void onHoughCircles(int, void *);// 回调函数--霍夫圆变换
static void showHelpText();


//------------------------------- main() function ---------------------------------

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    showHelpText();
    
    // 载入原始图
    srcImage = imread(PATH+string("warriors_logo.jpg"),1);
    // 异常处理 Exception Handlung
    if (!srcImage.data)
    {
        printf("error: we cannot read the source image 1.jpg!!! \n");
        return false;
    }
    
    // 显示原始图
    namedWindow("the original image");
    imshow("the original image", srcImage);
    
    // 创建滚动条
    namedWindow("the destination image");
    createTrackbar("parameter", "the destination image", &onThreshold1, 200, onHoughLines);
    createTrackbar("parameter", "the destination image", &onThreshold2, 300, onHoughCircles);
    
    // 进行边缘检测和转化为灰度值
    Canny(srcImage, midImage, 50, 200, 3);// Canny 边缘检测
    cvtColor(midImage, dstImage, CV_GRAY2BGR);// 把边缘检测后的图转化为灰度图
    //cvtColor(srcImage, midImage, CV_BGR2GRAY);
    //GaussianBlur(midImage, dstImage, Size(7,7), 2, 2);// 高斯平滑滤波
    
    // 调用回调函数 onHoughLines()
    onHoughLines(onThreshold1, 0);
    onHoughCircles(onThreshold2, 0);
    //HoughLinesP(midImage, lines, 1, CV_PI/180, 80, 50, 10);
    
    // 显示效果图
    imshow("the destination image", dstImage);
    
    waitKey(0);
    
    return 0;
}


//---------------------------- onHoughLines() function ------------------------------
//    Describe: 回调函数
//-----------------------------------------------------------------------------------

static void onHoughLines(int, void *)
{
    // 定义局部变量
    Mat midImageLocal = midImage.clone();
    Mat dstImageLocal = dstImage.clone();
    
    // 调用 HoughLinesP()
    vector<Vec4i> myLines;
    HoughLinesP(midImageLocal, myLines, 1, CV_PI/180, onThreshold1+1, 50, 10); //防止 onThresHold 不为零
    
    // 循环遍历绘制每一条线段
    for (size_t i = 0; i < myLines.size(); i++)
    {
        Vec4i l = myLines[i];
        line(dstImageLocal, Point(l[0],l[1]), Point(l[2],l[3]), Scalar(23,180,55), 1, CV_AA);
    }
    
    // 显示效果图
    imshow("the destination image", dstImageLocal);
}


//--------------------------- onHoughCircles() function ----------------------------
//    Describe: 回调函数
//-----------------------------------------------------------------------------------

static void onHoughCircles(int, void *)
{
    // 定义局部变量
    Mat midImageLocal = midImage.clone();
    Mat dstImageLocal = dstImage.clone();
    
    // 调用 HoughCircles()
    vector<Vec3f> myCircles;
    HoughCircles(midImageLocal, myCircles, CV_HOUGH_GRADIENT, 1, onThreshold2+1);
    
    // 循环遍历绘制出圆
    for (size_t i = 0; i < myCircles.size(); i++)
    {
        Point certer(cvRound(myCircles[i][0]),cvRound(myCircles[i][1]));// 圆心坐标
        int radius = cvRound(myCircles[i][2]);// 半径
        // 绘制圆心
        circle(dstImageLocal, certer, 3, Scalar(0,255,0), -1, 8, 0);
        // 绘制圆轮廓
        circle(dstImageLocal, certer, radius, Scalar(255,0,0), 3, 8, 0);
    }
    
    // 显示效果图
    imshow("the destination image", dstImageLocal);
}

//--------------------------- showHelpText() function ------------------------------

static void showHelpText()
{
    //输出一些帮助信息
    printf("\n\n\n\t请调整滚动条观察图像效果~\n\n");
    printf("\n\n\t\t\t\t\t\t\t\t byZHHJemotion");
}
