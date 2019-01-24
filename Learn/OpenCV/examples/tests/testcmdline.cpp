#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	CommandLineParser parser(argc,argv,
	{
		//	格式：命令  | 默认值 | 提示
		/*对于命令参数有如下细节：
		1. @表示按照顺序出现在exe后面,程序会根据出现顺序赋值给各个变量
		2. 非@参数使用'='运算符，如：fps=60 
		3. 非@参数可以使用-，或--开头，如： -fps或--fps
		4. 同一参数可以有多个别名称对应，如：help h usage ?，四个表达都可以显示帮助信息
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

// 使用测试
// testCmdLine.exe 12.jpg 3.jpg 0 --path=D:\tmp -fps=50

// docs 连接：https://docs.opencv.org/3.0-beta/modules/core/doc/command_line_parser.html