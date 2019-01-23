
// load video

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <comm/filesystem.h>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	std::string  filePath = "C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv";
	VideoCapture capture(filePath.c_str());


	Mat frame;
	while (1)
	{
		capture >> frame; 
		imshow("read video", frame);
		waitKey(30);
	}
	waitKey(0);
	
	return 0;
}