

// 图像模糊效果

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

	// 创建一个腐蚀核矩阵
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat dstImage;
	// 进行腐蚀处理
	erode(srcImage, dstImage, element);
	// 显示腐蚀后效果
	imshow("ercode", dstImage);

	waitKey(0);
	return 0;
}