// LoadShowOutputImage

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <comm/filesystem.h>


using namespace std;
using namespace cv;

const int g_nMaxAphaValue = 100;
int g_nAlphaValueSlider = 0;
double g_dAlphaValue;
double g_dBetaValue;

char* pWindowName = "blend_image";


Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;

void onTrackbar(int, void*)
{
	g_dAlphaValue = (double)g_nAlphaValueSlider / g_nMaxAphaValue; // alpha percent
	g_dBetaValue = (1.0 - g_dAlphaValue);

	addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2, g_dBetaValue, 0.0, g_dstImage);

	imshow(pWindowName, g_dstImage);

	

}


int main()
{
	std::string  dataPath = FileSystem::getPath("qianmo_opencv\\data\\");
	g_srcImage1 = imread(dataPath + "3.1.jpg");
	g_srcImage2 = imread(dataPath + "3.2.jpg");
	
	g_nAlphaValueSlider = 70;
	namedWindow(pWindowName, 1);

	char trackbarName[50];
	sprintf(trackbarName, "alpha %d", g_nMaxAphaValue);
	createTrackbar(trackbarName, pWindowName, &g_nAlphaValueSlider, g_nMaxAphaValue, onTrackbar);

	onTrackbar(g_nMaxAphaValue, 0);

	waitKey(0);
	return 0;
	
	
	
}
