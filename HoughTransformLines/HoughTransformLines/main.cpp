//
//  main.cpp
//  HoughTransformLines 霍夫线变换
//
//  霍夫变换要求源图像为8-bit的单通道二进制图像，一般为灰度图
//
//  Created by ZHHjemotion on 2016/10/28.
//  Copyright © 2016年 Lukas_Zhang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

#define PATH string("/Users/zhangxingjian/Desktop/Programming/C++/OpenCV/opencv_HoughTransform/HoughTransformLines/HoughTransformLines/")

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Mat srcImage, midImage, dstImage1, dstImage2; // 原始图，临时图（边缘图），目标图1(HoughLines()),2(HoughLinesP())
    
    // 载入原始图 1.jpg
    srcImage = imread(PATH+string("1.jpg"));
    
    // Exception Handlung
    if (!srcImage.data)
    {
        printf("error: we can not read the source image!!!\n");
        return false;
    }
    
    // 进行边缘检测和转化为灰度图
    Canny(srcImage, midImage, 50, 200, 3); // Canny边缘检测
    cvtColor(midImage, dstImage1, CV_GRAY2BGR);
    cvtColor(midImage, dstImage2, CV_GRAY2BGR);// 将边缘检测后的图转化为灰度图
    
    //进行霍夫变换
    vector<Vec2f> lines1;// 定义一个矢量结构lines用于存放得到的线段矢量集合
    vector<Vec4i> lines2;
    HoughLines(midImage, lines1, 1, CV_PI/180, 150, 0, 0);
    HoughLinesP(midImage, lines2, 1, CV_PI/180, 80, 50, 10);
    
    // 依次在图中绘制出每条线段
    // HoughLines()
    for (size_t i = 0; i < lines1.size(); i++)
    {
        float rho = lines1[i][0], theta = lines1[i][1];
        Point p1, p2;
        double a = cos(theta), b = sin(theta);
        double x0 =a*rho, y0 = b*rho;
        p1.x = cvRound(x0 + 1000*(-b));
        p1.y = cvRound(y0 + 1000*(a));
        p2.x = cvRound(x0 - 1000*(-b));
        p2.y = cvRound(y0 - 1000*(a));
        line(dstImage1, p1, p2, Scalar(55,100,195), 1, CV_AA);
        // void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
        // Point pt1, pt2: the points of the line segment
        // Scalar& color --> cv_RGB(r, g ,b);
        // lineType = 8(8-connected line) / 4(4-connected line) / CV_AA(antialiased line)
    }
    // HoughLinesP()
    for (size_t i = 0; i < lines2.size(); i++)
    {
        Vec4i l = lines2[i];
        line(dstImage2, Point(l[0],l[1]), Point(l[2],l[3]), Scalar(186,88,255), 1, CV_AA);
    }
    
    // 显示原始图
    namedWindow("the original image");
    imshow("the original image", srcImage);
    
    // 边缘检测后的图
    namedWindow("the image after the edge detection");
    imshow("the image after the edge detection", midImage);
    
    // 显示效果图
    //Mat g;
    //g = Mat::zeros(Size(midImage.rows, midImage.cols), CV_8UC1);
    //lines.push_back(g);
    //lines.push_back(g);
    //lines.push_back(midImage);
    
    //merge(lines, dstImage);
    
    namedWindow("the destination image of HoughLines()");
    imshow("the destination image of HoughLines()",  dstImage1);
    namedWindow("the destination image of HoughLinesP()");
    imshow("the destination image of HoughLinesP()", dstImage2);
    
    waitKey(0);
    
    return 0;
}
