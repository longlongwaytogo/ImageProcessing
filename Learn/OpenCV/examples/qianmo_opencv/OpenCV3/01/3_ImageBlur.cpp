// 图像模糊

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <comm/filesystem.h>
#include <string>

using namespace cv;
using namespace std;

int main()
{
	std::string  filePath = FileSystem::getPath("qianmo_opencv\\data\\1_2.jpg");
	Mat srcImage = imread(filePath.c_str());
	imshow("load iamge", srcImage);

	// 进行均值滤波 
	Mat dstImage;
	blur(srcImage, dstImage, Size(7, 7));

	// 显示腐蚀后效果
	imshow("blur", dstImage);

	waitKey(0);
	return 0;
}
