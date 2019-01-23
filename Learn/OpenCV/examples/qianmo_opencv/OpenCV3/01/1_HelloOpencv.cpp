#include <opencv2/opencv.hpp>
#include <comm/filesystem.h>
#include <string>

using namespace cv;
using namespace std;

int main()
{
	std::string  filePath = FileSystem::getPath("qianmo_opencv\\data\\1_1.jpg");
	Mat img = imread(filePath.c_str(), IMREAD_GRAYSCALE);
	imshow("load iamge", img);
	waitKey(0);
}