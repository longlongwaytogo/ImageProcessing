#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
 
#include <iostream>
#include <process.h>
#include <windows.h>
 
using namespace std;
using namespace cv;
 
static void help()
{
	cout << "\nThis program demonstrates GrabCut segmentation -- select an object in a region\n"
		"and then grabcut will attempt to segment it out.\n"
		"Call:\n"
		"./grabcut <image_name>\n"
		"\nSelect a rectangular area around the object you want to segment\n" <<
		"\nHot keys: \n"
		"\tESC - quit the program\n"
		"\tr - restore the original image\n"
		"\tn - next iteration\n"
		"\n"
		"\tleft mouse button - set rectangle\n"
		"\n"
		"\tCTRL+left mouse button - set GC_BGD pixels\n"
		"\tSHIFT+left mouse button - set GC_FGD pixels\n"
		"\n"
		"\tCTRL+right mouse button - set GC_PR_BGD pixels\n"
		"\tSHIFT+right mouse button - set GC_PR_FGD pixels\n" << endl;
}
 
const Scalar RED = Scalar(0, 0, 255);
const Scalar PINK = Scalar(230, 130, 255);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar LIGHTBLUE = Scalar(255, 255, 160);
const Scalar GREEN = Scalar(0, 255, 0);
 
const int BGD_KEY = CV_EVENT_FLAG_CTRLKEY;
const int FGD_KEY = CV_EVENT_FLAG_SHIFTKEY;
 
static void getBinMask(const Mat& comMask, Mat& binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		CV_Error(CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}
 
class GCApplication
{
public:
	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
	static const int radius = 2;
	static const int thickness = -1;
 
	void reset();
	void setImageAndWinName(const Mat& _image, const string& _winName);
	void showImage() const;
	void mouseClick(int event, int x, int y, int flags, void* param);
	int nextIter();
	int getIterCount() const { return iterCount; }
 
	const Rect *getRect() { return &rect; }
private:
	void setRectInMask();
	void setLblsInMask(int flags, Point p, bool isPr);
 
	const string* winName;
	const Mat* image;
	Mat mask;
	Mat bgdModel, fgdModel;
 
	uchar rectState, lblsState, prLblsState;
	bool isInitialized;
 
	Rect rect;
	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
	int iterCount;
};
 
void GCApplication::reset()
{
	if (!mask.empty())
		mask.setTo(Scalar::all(GC_BGD));
	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear();  prFgdPxls.clear();
 
	isInitialized = false;
	rectState = NOT_SET;
	lblsState = NOT_SET;
	prLblsState = NOT_SET;
	iterCount = 0;
}
 
void GCApplication::setImageAndWinName(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	mask.create(image->size(), CV_8UC1);
	reset();
}
 
void GCApplication::showImage() const
{
	if (image->empty() || winName->empty())
		return;
 
	Mat res;
	Mat binMask;
	if (!isInitialized)
		image->copyTo(res);
	else
	{
		getBinMask(mask, binMask);
		image->copyTo(res, binMask);
	}
 
	vector<Point>::const_iterator it;
	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
		circle(res, *it, radius, BLUE, thickness);
	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
		circle(res, *it, radius, RED, thickness);
	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
		circle(res, *it, radius, LIGHTBLUE, thickness);
	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
		circle(res, *it, radius, PINK, thickness);
 
	if (rectState == IN_PROCESS || rectState == SET)
		rectangle(res, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);
 
	imshow(*winName, res);
}
 
void GCApplication::setRectInMask()
{
	assert(!mask.empty());
	mask.setTo(GC_BGD);
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image->cols - rect.x);
	rect.height = min(rect.height, image->rows - rect.y);
	printf("setRectInMask rect(x=%d, y=%d, w=%d, h=%d)\n", rect.x, rect.y, rect.width, rect.height);
	(mask(rect)).setTo(Scalar(GC_PR_FGD));
}
 
void GCApplication::setLblsInMask(int flags, Point p, bool isPr)
{
	vector<Point> *bpxls, *fpxls;
	uchar bvalue, fvalue;
	if (!isPr)
	{
		bpxls = &bgdPxls;
		fpxls = &fgdPxls;
		bvalue = GC_BGD;
		fvalue = GC_FGD;
	}
	else
	{
		bpxls = &prBgdPxls;
		fpxls = &prFgdPxls;
		bvalue = GC_PR_BGD;
		fvalue = GC_PR_FGD;
	}
	if (flags & BGD_KEY)
	{
		bpxls->push_back(p);
		circle(mask, p, radius, bvalue, thickness);
	}
	if (flags & FGD_KEY)
	{
		fpxls->push_back(p);
		circle(mask, p, radius, fvalue, thickness);
	}
}
 
void GCApplication::mouseClick(int event, int x, int y, int flags, void*)
{
	// TODO add bad args check
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
	{
		bool isb = (flags & BGD_KEY) != 0,
			isf = (flags & FGD_KEY) != 0;
		if (rectState == NOT_SET && !isb && !isf)
		{
			rectState = IN_PROCESS;
			rect = Rect(x, y, 1, 1);
		}
		if ((isb || isf) && rectState == SET)
			lblsState = IN_PROCESS;
	}
	break;
	case CV_EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
	{
		bool isb = (flags & BGD_KEY) != 0,
			isf = (flags & FGD_KEY) != 0;
		if ((isb || isf) && rectState == SET)
			prLblsState = IN_PROCESS;
	}
	break;
	case CV_EVENT_LBUTTONUP:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			rectState = SET;
			setRectInMask();
			assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();
		}
		if (lblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), false);
			lblsState = SET;
			showImage();
		}
		break;
	case CV_EVENT_RBUTTONUP:
		if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			prLblsState = SET;
			showImage();
		}
		break;
	case CV_EVENT_MOUSEMOVE:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();
		}
		else if (lblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), false);
			showImage();
		}
		else if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			showImage();
		}
		break;
	}
}
 
int GCApplication::nextIter()
{
	if (isInitialized)
		grabCut(*image, mask, rect, bgdModel, fgdModel, 1);
	else
	{
		if (rectState != SET)
			return iterCount;
 
		if (lblsState == SET || prLblsState == SET)
			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);
		else
			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
 
		isInitialized = true;
	}
	iterCount++;
 
	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear(); prFgdPxls.clear();
 
	return iterCount;
}
 
GCApplication gcapp;
 
static void on_mouse(int event, int x, int y, int flags, void* param)
{
	gcapp.mouseClick(event, x, y, flags, param);
}
const char *window_name = "Result";
Mat g_srcImage;
 
static void my_grabcut(Mat &image, Rect &r)
{
	Mat mask;
	Mat bgdModel, fgdModel;
	//先认为源图所有区域都为背景
	mask.create(image.size(), CV_8UC1);
	mask.setTo(GC_BGD);
	Rect rect(Point(0, 0), Point(image.size().width - 1, image.size().height - 1));
	if(r.width > 1 && r.height > 1)
	{
		printf("轮廓矩形正常(x=%d, y=%d, w=%d, h=%d)...\n", r.x, r.y, r.width, r.height);
		rect = r;
	}
	//确保矩形区域合法
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image.cols - rect.x);
	rect.height = min(rect.height, image.rows - rect.y);
	//设置矩形区域为前景色
	(mask(rect)).setTo(Scalar(GC_PR_FGD));
	//利用opencv grabCut扣图
	grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
	Mat res;
	Mat binMask;
	getBinMask(mask, binMask);
	image.copyTo(res, binMask);
	//显示图
	imshow(window_name, res);
}
 
void func(Mat &image, int threshold_value)
{
	Mat secondImg;
	Mat grayImage;
	vector<vector<Point>> contours;
 
	//灰度化原图
	cvtColor(image, grayImage, CV_BGR2GRAY);
	//二值化灰度图，大津法
	threshold(grayImage, secondImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//查找二值化图的轮廓
	findContours(secondImg, contours, cv::noArray(), RETR_TREE, CHAIN_APPROX_SIMPLE);
	//根据找到的轮廓得到最大的矩形区域
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	if (contours.size() > 0 && contours[0].size() > 0)
	{
		x1 = x2 = contours[0][0].x;
		y1 = y2 = contours[0][0].y;
		for (size_t i = 0; i < contours.size(); i++)
		{
			for (size_t j = 0; j < contours[i].size(); j++)
			{
				x1 = min(x1, contours[i][j].x);
				y1 = min(y1, contours[i][j].y);
				x2 = max(x2, contours[i][j].x);
				y2 = max(y2, contours[i][j].y);
			}
		}
	}
	Rect r(x1, y1, x2, y2);
 
	/// 初始化自定义的阈值函数
	my_grabcut(image, r);
	printf("finish grabcut\n");
}
 
unsigned int _stdcall _ThreadRun(void *)
{
	func(g_srcImage, 0);
	return 0;
}
 
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		help();
		return 1;
	}
	string filename = argv[1];
	if (filename.empty())
	{
		cout << "\nDurn, couldn't read in " << argv[1] << endl;
		return 1;
	}
	//load source image file
	Mat image = imread(filename, 1);
	if (image.empty())
	{
		cout << "\n Durn, couldn't read image filename " << filename << endl;
		return 1;
	}
	image.copyTo(g_srcImage);
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	//imshow("gray image", grayImg);
 
	//创建扣图线程
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, _ThreadRun, NULL, 0, NULL);
	CloseHandle(hThread);
 
	help();
 
	const string winName = "image";
	namedWindow(winName, WINDOW_AUTOSIZE);
	setMouseCallback(winName, on_mouse, 0);
 
	gcapp.setImageAndWinName(image, winName);
	gcapp.showImage();
 
	for (;;)
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case '\x1b':
			cout << "Exiting ..." << endl;
			goto exit_main;
		case 'r':
			cout << endl;
			gcapp.reset();
			gcapp.showImage();
			break;
		case 'n':
			int iterCount = gcapp.getIterCount();
			cout << "<" << iterCount << "... ";
			int newIterCount = gcapp.nextIter();
			if (newIterCount > iterCount)
			{
				gcapp.showImage();
				cout << iterCount << ">" << endl;
			}
			else
				cout << "rect must be determined>" << endl;
			break;
		}
	}
 
exit_main:
	destroyWindow(winName);
	return 0;
}
// https://blog.csdn.net/xiejianjun417/article/details/81323214