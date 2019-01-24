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
//	printf("\n\n\t�˳���չʾ����Ƶǰ����������ķ���,����cvUpdateBGStatModel()����.\n"
//		"\n\n\t�������Ȼᡰѧϰ��������Ȼ����зָ\n"
//		"\n\n\t�����ù���Space���ո���й����л���\n\n");
//
//}
//
//int main(int argc, const char** argv)
//{
//	help();
//	VideoCapture cap;
//	bool update_bg_model = true;
//
//	cap.open(0); // ����Ƶ
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
//	Ptr<BackgroundSubtractorMOG2>  bg_model = createBackgroundSubtractorMOG2(100, 25, false); // ��˹���ģ�ͱ�����ģ
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
//		// ����ģ��
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
//				printf("\t>��������(Background update)�Ѵ�\n");
//			}
//			else
//			{
//				printf("\t>��������(Background update)�ѹر�\n");
//			}
//		}
//	}
//
//	return 0;
//}

/*
����opencv3.0�У�ʹ�÷�ʽ��һ��������3.0�л�����BackgroundSubtractorMOG������ֻ��BackgroundSubtractorMOG2����ͨ��opencv�ٷ��ĵ�segment_objects.cpp��http://http://docs.opencv.org/master/dd/d9d/segment_objects_8cpp-example.html#a22��Ū���������ʹ�á�
---------------------
���ߣ�percy_ling
��Դ��CSDN
ԭ�ģ�https://blog.csdn.net/mumu0627/article/details/52741220
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�
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
