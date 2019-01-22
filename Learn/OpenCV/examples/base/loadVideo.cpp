#define USE_C 0

#if USER_C
#include <opencv/highgui.h>

#else 
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#endif 

using namespace cv;
using namespace std;


int main( int argc, char** argv)
{
	/*if(argc != 2)
	{
		cout <<" Usage display ImageLoadAndDisplay" << endl;
		return -1;
	}*/
	
#if USE_C
	//CvCapture* capture = cvCreateFileCapture("C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv");
	CvCapture* capture = cvCreateFileCapture("F:\\project_2\\OpenCV4\\opencv\\sources\\samples\\data\\vtest.avi");
	IplImage* frame;
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	while(1) {
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Example2",frame);
		char c = cvWaitKey(33);
		if(c == 27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Example2");
#else

		VideoCapture capture("F:\\project_2\\OpenCV4\\opencv\\sources\\samples\\data\\vtest.avi");
		while (true) {
			Mat frame;
			Mat edge;
			capture >> frame;

			if (frame.empty())
				break;

			cvtColor(frame, edge, COLOR_BGR2GRAY);

			blur(edge, edge, Size(7, 7));

			Canny(edge, edge, 10, 30);

			imshow("Video", frame);
			imshow("After canny", edge);

			//等待50ms，如果从键盘输入的是q、Q、或者是Esc键，则退出
			int key = waitKey(50);
			if (key == 'q' || key == 'Q' || key == 27)
				break;
		}


#endif 

	waitKey(0); 
	return 0;
}