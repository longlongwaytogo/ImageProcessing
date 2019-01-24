// https://www.cnblogs.com/riddick/p/9000281.html
// ÕºœÒ»⁄∫œ÷Æ≤¥À…»⁄∫œ£®Possion Matting£©
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <comm/filesystem.h>

using namespace std;
using namespace cv;

void main()
{
	std::string  filePath = FileSystem::getPath("qianmo_opencv\\data\\");
	Mat imgL = imread(filePath + "apple.jpg");
	Mat imgR = imread(filePath + "orange.jpg");

	int imgH = imgR.rows;
	int imgW = imgR.cols;

	Mat mask = Mat::zeros(imgL.size(), CV_8UC1);
	mask(Rect(0, 0, imgW*0.5, imgH)).setTo(255);
	imshow("mask", mask);

	Point center(imgW*0.25, imgH*0.5);

	Mat blendImg;
	seamlessClone(imgL, imgR, mask, center, blendImg, NORMAL_CLONE);

	imshow("blendimg", blendImg);

	waitKey(0);
}
