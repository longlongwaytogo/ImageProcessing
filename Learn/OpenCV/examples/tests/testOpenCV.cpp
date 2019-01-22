#include <opencv2/core/core.hpp> // 核心库
#include <opencv2/highgui/highgui.hpp> // 界面库
#include <iostream>

using namespace cv;
using namespace std; 
 
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);	// Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", CV_WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.

	waitKey(0);											 // Wait for a keystroke in the window
	return 0;
}
//--------------------- 
//作者：yang_xian521 
//来源：CSDN 
//原文：https://blog.csdn.net/yang_xian521/article/details/6894228 
//版权声明：本文为博主原创文章，转载请附上博文链接！