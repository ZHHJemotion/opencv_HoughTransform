//
//  main.cpp
//  HoughTransformCircles 霍夫变换for圆 --> 霍夫梯度法
//
//  Created by ZHHJemotion on 2016/10/28.
//  Copyright © 2016年 Lukas_Zhang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#define PATH string("/Users/zhangxingjian/Desktop/Programming/C++/OpenCV/opencv_HoughTransform/HoughTransformCircles/HoughTransformCircles/")

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Mat srcImage, midImage, dstImage;
    // 载入原始图
    srcImage = imread(PATH+string("warriors_logo.jpg"));
    // Exception Handlung 异常处理
    if (!srcImage.data)
    {
        printf("error: we can not read the source image!!! \n");
        return false;
    }
    
    // 显示原始图
    namedWindow("the original image");
    imshow("the original image", srcImage);
    
    // 边缘检测，后转为灰度图，进行图像滤波
    //Canny(srcImage, midImage, 50, 200, 3);
    cvtColor(srcImage, midImage, CV_BGR2GRAY);// 把边缘检测后的图转化为灰度图
    GaussianBlur(midImage, dstImage, Size(9,9), 2, 2);// 高斯滤波 --> smooth it, otherwise a lot of false circles may be detected
    
    // 进行霍夫圆变换
    vector<Vec3f> circles;// 定义一个矢量结构circles 用于存放得到的圆矢量集合(圆心坐标x,y,半径r)
    HoughCircles(dstImage, circles, CV_HOUGH_GRADIENT, 1.5, 50, 200, 100, 0, 0);
    
    // 依次绘制出圆
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point certer(cvRound(circles[i][0]),cvRound(circles[i][1]));// 圆心位置
        int radius = cvRound(circles[i][2]);
        // 绘制圆心
        circle(dstImage, certer, 3, Scalar(0,255,0),-1, 8, 0);
        // 绘制圆轮廓
        circle(dstImage, certer, radius, Scalar(155,50,255),3 ,8 ,0);
        // void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0
        // Thickness --> Thickness of the circle outline, if positive. Negative thickness means that a filled circle is to be drawn.
    }
    
    // 显示效果图
    namedWindow("the destination image");
    imshow("the destination image", dstImage);
    
    waitKey(0);
    
    return 0;
}
