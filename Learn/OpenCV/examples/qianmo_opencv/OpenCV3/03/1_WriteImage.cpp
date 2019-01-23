#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;


void createAlphaMat(Mat &mat)
{
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			Vec4b& rgba = mat.at<Vec4b>(i, j);
			rgba[0] = UCHAR_MAX; // blue
			rgba[1] =  saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // green
			rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) /((float)mat.rows) * UCHAR_MAX); // red
			rgba[3] = saturate_cast<uchar>(0.5 * rgba[1] + rgba[2]);
		}
	}
}


int main()
{
	// create alpha channel
	Mat mat(480, 640, CV_8UC4);
	createAlphaMat(mat);

	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("透明Alpha.png", mat, compression_params);
		imshow("生成的PNG图", mat);
		cout << "PNG图片的alpha数据保存完毕" << endl;

		waitKey(0);

	}
	catch (runtime_error& e) {
		std::cerr << "图形转换成PNG格式发生错误：" << e.what() << endl;
		return 1;
	}
	return 0;
}