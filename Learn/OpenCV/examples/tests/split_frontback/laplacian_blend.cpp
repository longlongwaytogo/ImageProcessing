// https://www.cnblogs.com/riddick/p/8922381.html
// 拉普拉斯融合（laplacian blending）
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <comm/filesystem.h>

using namespace std;
using namespace cv;

/*
说明：
	金字塔从下到上依次为：[0,1,...,level-1]层
	blendMask为图像掩模
	maskGaussianPyramid为金字塔每一层掩模
	resultLapPyr 存放每一层金字塔中直接用左右两图Laplacian变换拼成的图像
*/

class LaplacianBlend
{
private:
	Mat m_left;
	Mat m_right;
	Mat m_blendMask;

	// laplacian Pyramids
	vector<Mat> m_leftLapPyr;
	vector<Mat> m_rightLapPyr;
	vector<Mat> m_resultLapPyr;

	Mat m_resultHighestLevel;
	Mat m_leftHighestLevel;
	Mat m_rightHighestLevel;

	// mask 为三通道方便矩阵相乘
	vector<Mat> m_maskGassianPyramid;

	int m_levels;

private:
	void buildPyramids()
	{
		buildLaplacianPyramid(m_left, m_leftLapPyr, m_leftHighestLevel);
		buildLaplacianPyramid(m_right, m_rightLapPyr, m_rightHighestLevel);
		buildGaussianPyramid();
	}

	void buildLaplacianPyramid(Mat& img, vector<Mat>& lapPyr, Mat& highestLevel)
	{
		lapPyr.clear();
		Mat currentImg = img;

		for (int i = 0; i < m_levels; i++)
		{
			Mat down, up;
			pyrDown(currentImg, down);
			pyrUp(down, up, currentImg.size());
			Mat lap = currentImg - up; // 计算残差
			lapPyr.push_back(lap);
			currentImg = down;
		}
		currentImg.copyTo(highestLevel); // 将最后一级保存到HighestLevel中

	}

	// 使用拉普拉斯金字塔重建图像
	Mat reconstructImgFromLapPyramid()
	{
		// 从左右laplacian图像拼成resultLapPyr金字塔中每一层
		// 从上到下插值放大并与残差相加，即得到blend图像结果
		Mat currentImg = m_resultHighestLevel;
		for (int i = m_levels - 1; i >= 0; i--)
		{
			Mat up;
			pyrUp(currentImg, up, m_resultLapPyr[i].size());
			currentImg = up + m_resultLapPyr[i];
		}
		return currentImg;
	}
	// 计算合并后的拉普拉斯残差金字塔
	void blendLapPyrs()
	{
		// 获得每层金字塔中直接用左右图Laplacian变换拼成的图像m_resultlapPyr
		m_resultHighestLevel = m_leftHighestLevel.mul(m_maskGassianPyramid.back()) +
			m_rightHighestLevel.mul(Scalar(1.0, 1.0, 1.0) - m_maskGassianPyramid.back());

		for (int i = 0; i < m_levels; i++)
		{
			Mat A = m_leftLapPyr[i].mul(m_maskGassianPyramid[i]);
			Mat antiMask = Scalar(1.0, 1.0, 1.0) - m_maskGassianPyramid[i];
			Mat B = m_rightLapPyr[i].mul(antiMask);
			Mat blendedLevel = A + B;

			m_resultLapPyr.push_back(blendedLevel);
		}
	}

	void buildGaussianPyramid()
	{
		// 金字塔内容为每一层掩模
		assert(m_leftLapPyr.size() > 0);
		m_maskGassianPyramid.clear();
		Mat currentImg;
		cvtColor(m_blendMask, currentImg, CV_GRAY2BGR);

		// 保存mask金字塔的每一层图像
		m_maskGassianPyramid.push_back(currentImg); // 0-level

		currentImg = m_blendMask;
		for (int i = 1; i < m_levels + 1; ++i)
		{
			Mat _down;
			if (m_leftLapPyr.size() > i)  
				pyrDown(currentImg, _down, m_leftLapPyr[i].size());
			else
				pyrDown(currentImg, _down, m_leftHighestLevel.size());

			Mat down;
			cvtColor(_down, down, CV_GRAY2BGR);

			m_maskGassianPyramid.push_back(down);
			currentImg = _down;
		}

	}

	public:
		LaplacianBlend(const Mat& _left, const Mat& _right, const Mat& _blendMask, int  levels) :
			m_left(_left), m_right(_right), m_blendMask(_blendMask), m_levels(levels)
		{
			assert(_left.size() == _right.size());
			assert(_left.size() == _blendMask.size());

			// 创建拉普拉斯金字塔和搞死金字塔
			buildPyramids();

			// 每一层金字塔图像合并
			blendLapPyrs();
		}

		Mat blend()
		{   // 重建拉普拉斯金字塔
			return reconstructImgFromLapPyramid();
		}
	
};

Mat DoLaplacianBlend(const Mat& left, const Mat& right, const Mat& mask)
{
	LaplacianBlend laplacianBlend(left, right, mask, 10);
	return laplacianBlend.blend();
}

void main()
{
	std::string  filePath = FileSystem::getPath("qianmo_opencv\\data\\");
	Mat img8UL = imread(filePath + "apple.jpg");
	Mat img8UR = imread(filePath + "orange.jpg");

	imshow("left", img8UL);
	imshow("right", img8UR);


	int imgH = img8UR.rows;
	int imgW = img8UR.cols;

	// 转换为32位浮点数据
	Mat img32fL, img32fR;
	img8UL.convertTo(img32fL, CV_32F);
	img8UR.convertTo(img32fR, CV_32F);

	// create mask
	Mat mask = Mat::zeros(imgH, imgW,CV_32FC1);
	mask(Range::all(), Range(0, mask.cols * 0.5)) = 1.0;

	Mat blendImg = DoLaplacianBlend(img32fL, img32fR, mask);

	blendImg.convertTo(blendImg, CV_8UC3);
	imshow("blended",blendImg);

	waitKey(0);

	return;
}

/*
一、拉普拉斯融合基本步骤

1. 两幅图像L，R，以及二值掩模mask，给定金字塔层数level。

2. 分别根据L，R构建其对应的拉普拉斯残差金字塔（层数为level），并保留高斯金字塔下采样最顶端的图像（尺寸最小的图像，第level+1层）：

拉普拉斯残差金字塔构建方法如下，以L图为例：

(1) 对L进行高斯下采样得到downL，OpenCV中pyrDown()函数可以实现此功能。然后再对downL进行高斯上采样得到upL，OpenCV中pyrUp()函数可以实现此功能。

(2) 计算原图L与upL之间的残差，得到一幅残差图lapL0。作为残差金字塔最低端的图像。

(3) 对downL继续进行(1) (2)操作，不断计算残差图lapL1, lap2, lap3.....lapN。这样得到一系列残差图，即为拉普拉斯残差金字塔。

(4)拉普拉斯 残差金字塔中一共有level幅图像。而我们需要保留第level+1层的高斯下采样图topL，以便后面使用。

3. 二值掩模mask下采样构建高斯金字塔，同样利用pyrDown()实现，共有level+1层。

4. 利用mask金字塔每一层的mask图，将L图和R图的拉普拉斯残差金字塔对应层的图像合并为一幅图像。这样得到合并后的拉普拉斯残差金字塔。同时利用最顶端的mask将步骤2中保留的topL和topR合并为topLR。

5. 以topLR为金字塔最顶端的图像，利用pyrUp()函数对topLR进行高斯上采样，得到upTopLR，并将upTopLR与步骤4中合并后的残差金字塔对应层的图像相加，重建出该层的图像。

6. 重复步骤5，直至重建出第0层，也就是金字塔最低端的图像，即blendImg。输出。
*/