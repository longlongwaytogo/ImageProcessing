#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	CommandLineParser parser(argc,argv,
	{
		//	��ʽ������  | Ĭ��ֵ | ��ʾ
		/*�����������������ϸ�ڣ�
		1. @��ʾ����˳�������exe����,�������ݳ���˳��ֵ����������
		2. ��@����ʹ��'='��������磺fps=60 
		3. ��@��������ʹ��-����--��ͷ���磺 -fps��--fps
		4. ͬһ���������ж�������ƶ�Ӧ���磺help h usage ?���ĸ���ﶼ������ʾ������Ϣ
		*/
		"{help h usage ? |      | print this message   }"
		"{@image1        |1.jpg | image1 for compare   }"
		"{@image2        |2.jpg | image2 for compare   }"
		"{@repeat        |1     | number               }"
		"{path           |.     | path to file         }"
		"{fps            | -1.0 | fps for output video }"
		"{N count        |100   | count of objects     }"
		"{ts timestamp   |      | use time stamp       }"
 	});

	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	int N = parser.get<int>("N");
	double fps = parser.get<double>("fps");
	String path = parser.get<String>("path");

	bool use_time_stamp = parser.has("timestamp");

	String img1 = parser.get<String>(0);
	String img2 = parser.get<String>(1);
	int repeat = parser.get<int>(2);

	cout << "image1:" << img1 << std::endl;
	cout << "image2:" << img2 << endl;
	cout << "repeat:" << repeat << endl;
	cout << "use_time_stamp:" << use_time_stamp << endl;

	cout << "path:" << path << endl;
	cout << "fps:" << fps << endl;
	
	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}
	return 0;
}

// ʹ�ò���
// testCmdLine.exe 12.jpg 3.jpg 0 --path=D:\tmp -fps=50

// docs ���ӣ�https://docs.opencv.org/3.0-beta/modules/core/doc/command_line_parser.html