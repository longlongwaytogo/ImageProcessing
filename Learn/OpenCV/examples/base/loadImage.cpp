#define USE_C 1

#if USER_C
#include <opencv/highgui.h>

#else 
#include <opencv2/core.hpp>
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
	IplImage* img = cvLoadImage("C:\\Users\\Public\\Pictures\\Sample Pictures\\Tulips.jpg");
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
#else
	Mat image;
	String imageStr = argv[1];
	if (imageStr.size() <= 0)
		imageStr = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Tulips.jpg";
	image = imread(imageStr,IMREAD_COLOR); // read the image file
	if(image.empty()) // check for invalid input_iterator
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image);
#endif 

	waitKey(0); 
	return 0;
}