
// camera capture
// 使用虚拟相机时，出现x64下崩溃，x86下正常
#include <opencv2\opencv.hpp>  
using namespace cv;

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{
	//【1】从摄像头读入视频
	VideoCapture capture(0);

	//【2】循环显示每一帧
	while (1)
	{
		Mat frame;  //定义一个Mat变量，用于存储每一帧的图像
		capture >> frame;  //读取当前帧
		imshow("读取视频", frame);  //显示当前帧
		waitKey(30);  //延时30ms
	}
	return 0;
}