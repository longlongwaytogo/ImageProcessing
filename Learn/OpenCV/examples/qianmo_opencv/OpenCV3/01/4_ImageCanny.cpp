
// 边缘检测 canny

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <comm/filesystem.h>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	std::string  filePath = FileSystem::getPath("qianmo_opencv\\data\\1_2.jpg");
	Mat srcImage = imread(filePath.c_str());
	imshow("orign image",srcImage);
	

	Mat dstImage,edge,grayImage; 
	// create matrix of core 
	dstImage.create(srcImage.size(),srcImage.type());
	
	// 将原图像转为灰度图像
	cvtColor(srcImage,grayImage, COLOR_BGR2GRAY);
	
	// 使用3x3内核降噪
	blur(grayImage,edge,Size(3,3));
	
	// 运行Canny算子
	Canny(edge,edge,3,9,3);
	
	// show image
	imshow("Canny",edge);
	
	waitKey(0);
	
	return 0;
}