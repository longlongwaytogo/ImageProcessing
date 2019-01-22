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
	bool status = true;  // �ж�ѭ���Ƿ���ֹ�Ĳ�����
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
//���ߣ�SongYuLong�Ĳ���
//��Դ��CSDN
//ԭ�ģ�https ://blog.csdn.net/songyulong8888/article/details/82749018 
//��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�