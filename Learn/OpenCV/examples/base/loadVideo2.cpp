#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	//cvNamedWindow("WIND0", CV_WINDOW_AUTOSIZE);
	//CvCapture* capture = cvCreateFileCapture(argv[1]);
	//IplImage* frame;

	//while (1)
	//{
	//	frame = cvQueryFrame(capture);
	//	if (!frame) break;
	//	cvShowImage("WIND0", frame);
	//	char c = cvWaitKey(33);
	//	if (c == 'c') break;
	//}

	//cvReleaseCapture(&capture);
	//cvDestroyWindow("WIND0");

	cvNamedWindow("WIND0", CV_WINDOW_AUTOSIZE);
	cv::VideoCapture capture(argv[1]);
	if (!capture.isOpened())
		return -1;
	bool status = true;  // 判断循环是否中止的布尔量
	cv::Mat frame;
	while (status)
	{
		capture >> frame;
		if (frame.empty())
			break;
		cv::imshow("video", frame);
		if (cv::waitKey(27) >= 0)
			status = false;
	}
	 cvDestroyWindow("WIND0");
	return 0;
}


 
//-------------------- -
//作者：SongYuLong的博客
//来源：CSDN
//原文：https ://blog.csdn.net/songyulong8888/article/details/82749018 
//版权声明：本文为博主原创文章，转载请附上博文链接！