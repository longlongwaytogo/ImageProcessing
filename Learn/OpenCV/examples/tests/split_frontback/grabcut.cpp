//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//
//#include <opencv2/opencv.hpp>
//
//#include <stdio.h>
//
//using namespace  std;
//using namespace cv;
//
//
//static void help()
//{
//	printf("\n\n\t此程序展示了视频前景背景分离的方法,采用cvUpdateBGStatModel()方法.\n"
//		"\n\n\t程序首先会“学习背景”，然后进行分割。\n"
//		"\n\n\t可以用过【Space】空格进行功能切换。\n\n");
//
//}
//
//int main(int argc, const char** argv)
//{
//	help();
//	VideoCapture cap;
//	bool update_bg_model = true;
//
//	cap.open(0); // 打开视频
//	if (!cap.isOpened())
//	{
//		printf("can't open camera or vide file!\n");
//		return -1;
//	}
//
//	namedWindow("image", WINDOW_AUTOSIZE);
//	namedWindow("foreground mask", WINDOW_AUTOSIZE);
//	namedWindow("froeground image", WINDOW_AUTOSIZE);
//	namedWindow("mean background image", WINDOW_AUTOSIZE);
//
//	//Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(100, 25, false);
//	Ptr<BackgroundSubtractorMOG2>  bg_model = createBackgroundSubtractorMOG2(100, 25, false); // 高斯混合模型背景建模
//	Mat img, fgmask, fgimg;
//	cap >> img;
//	
//	for (;;)
//	{
//		cap >> img;
//		if (img.empty())
//			break;
//
//		if (fgimg.empty())
//			fgimg.create(img.size(), img.type());
//
//		// 更新模型
//		bg_model(img, fgmask, update_bg_model ? -1 : 0);
//		fgimg = Scalar::all(0);
//		img.copyTo(fgimg, fgmask);
//
//		Mat bgimg;
//		bg_model->getBackgroundImage(bgimg);
//
//		imshow("image", img);
//		imshow("foreground mask", fgmask);
//		imshow("foreground image", fgimg);
//		if (!bgimg.empty())
//			imshow("mean background image", bgimg);
//
//		char k = (char)waitKey(1);
//		if (k == 27)break;
//		if (k == ' ')
//		{
//			update_bg_model = !update_bg_model;
//			if (update_bg_model)
//			{
//				printf("\t>背景更新(Background update)已打开\n");
//			}
//			else
//			{
//				printf("\t>背景更新(Background update)已关闭\n");
//			}
//		}
//	}
//
//	return 0;
//}

/*
但在opencv3.0中，使用方式不一样，而且3.0中还少了BackgroundSubtractorMOG函数，只有BackgroundSubtractorMOG2，我通过opencv官方文档segment_objects.cpp（http://http://docs.opencv.org/master/dd/d9d/segment_objects_8cpp-example.html#a22）弄明白了如何使用。
---------------------
作者：percy_ling
来源：CSDN
原文：https://blog.csdn.net/mumu0627/article/details/52741220
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
#include "opencv2/opencv.hpp"
#include "opencv2/video/background_segm.hpp"

using namespace cv;
using namespace std;
int main()
{
	VideoCapture video("vtest.avi");
	int frameNum = 1;
	Mat frame, mask, thresholdImage, output;
	if (!video.isOpened())
		cout << "fail to open!" << endl;
	//cout<<video.isOpened();
	double totalFrameNumber = video.get(CAP_PROP_FRAME_COUNT);
	video >> frame;
	Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
	bgsubtractor->setVarThreshold(20);

	while (true) {
		if (totalFrameNumber == frameNum)
			break;
		video >> frame;
		++frameNum;
		//bgSubtractor(frame, mask, 0.001);
		bgsubtractor->apply(frame, mask, 0.01);
		imshow("mask", mask);
		waitKey(100);
	}

	return 0;
}
