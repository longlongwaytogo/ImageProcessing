// LoadShowOutputImage

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <comm/filesystem.h>


using namespace std;
using namespace cv;

int main()
{
	std::string dataPath = FileSystem::getPath("qianmo_opencv\\data\\");
	Mat girlImge = imread(dataPath + "girl.jpg");
	imshow("girl",girlImge);
	
	// load image
	Mat image = imread(dataPath + "dota.jpg"/*,199*/);
	Mat logo = imread(dataPath + "dota_logo.jpg");
	
	namedWindow("dota");
	imshow("dota",image);
	namedWindow("logo");
	imshow("logo",logo);
	
	// 定义一个Mat类型，用于存放图像ROI
	Mat imageROI;
	imageROI = image(Rect(800,350,logo.cols,logo.rows));
	
	// 将logo加到原图上
	addWeighted(imageROI,0.5,logo,0.3,0.0,imageROI);
	
	// 显示结果
	namedWindow("origin+logo");
	imshow("origin+logo",image);
	
	// 输出一个jpg到工程目录
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(9);

	imwrite("write.jpg",image, compression_params);
	
	waitKey(0);
	return 0;
	
	
	
}
