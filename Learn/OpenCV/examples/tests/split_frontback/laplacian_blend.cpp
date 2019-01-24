// https://www.cnblogs.com/riddick/p/8922381.html
// ������˹�ںϣ�laplacian blending��
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <comm/filesystem.h>

using namespace std;
using namespace cv;

/*
˵����
	���������µ�������Ϊ��[0,1,...,level-1]��
	blendMaskΪͼ����ģ
	maskGaussianPyramidΪ������ÿһ����ģ
	resultLapPyr ���ÿһ���������ֱ����������ͼLaplacian�任ƴ�ɵ�ͼ��
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

	// mask Ϊ��ͨ������������
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
			Mat lap = currentImg - up; // ����в�
			lapPyr.push_back(lap);
			currentImg = down;
		}
		currentImg.copyTo(highestLevel); // �����һ�����浽HighestLevel��

	}

	// ʹ��������˹�������ؽ�ͼ��
	Mat reconstructImgFromLapPyramid()
	{
		// ������laplacianͼ��ƴ��resultLapPyr��������ÿһ��
		// ���ϵ��²�ֵ�Ŵ���в���ӣ����õ�blendͼ����
		Mat currentImg = m_resultHighestLevel;
		for (int i = m_levels - 1; i >= 0; i--)
		{
			Mat up;
			pyrUp(currentImg, up, m_resultLapPyr[i].size());
			currentImg = up + m_resultLapPyr[i];
		}
		return currentImg;
	}
	// ����ϲ����������˹�в������
	void blendLapPyrs()
	{
		// ���ÿ���������ֱ��������ͼLaplacian�任ƴ�ɵ�ͼ��m_resultlapPyr
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
		// ����������Ϊÿһ����ģ
		assert(m_leftLapPyr.size() > 0);
		m_maskGassianPyramid.clear();
		Mat currentImg;
		cvtColor(m_blendMask, currentImg, CV_GRAY2BGR);

		// ����mask��������ÿһ��ͼ��
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

			// ����������˹�������͸���������
			buildPyramids();

			// ÿһ�������ͼ��ϲ�
			blendLapPyrs();
		}

		Mat blend()
		{   // �ؽ�������˹������
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

	// ת��Ϊ32λ��������
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
һ��������˹�ںϻ�������

1. ����ͼ��L��R���Լ���ֵ��ģmask����������������level��

2. �ֱ����L��R�������Ӧ��������˹�в������������Ϊlevel������������˹�������²�����˵�ͼ�񣨳ߴ���С��ͼ�񣬵�level+1�㣩��

������˹�в�����������������£���LͼΪ����

(1) ��L���и�˹�²����õ�downL��OpenCV��pyrDown()��������ʵ�ִ˹��ܡ�Ȼ���ٶ�downL���и�˹�ϲ����õ�upL��OpenCV��pyrUp()��������ʵ�ִ˹��ܡ�

(2) ����ԭͼL��upL֮��Ĳв�õ�һ���в�ͼlapL0����Ϊ�в��������Ͷ˵�ͼ��

(3) ��downL��������(1) (2)���������ϼ���в�ͼlapL1, lap2, lap3.....lapN�������õ�һϵ�вв�ͼ����Ϊ������˹�в��������

(4)������˹ �в��������һ����level��ͼ�񡣶�������Ҫ������level+1��ĸ�˹�²���ͼtopL���Ա����ʹ�á�

3. ��ֵ��ģmask�²���������˹��������ͬ������pyrDown()ʵ�֣�����level+1�㡣

4. ����mask������ÿһ���maskͼ����Lͼ��Rͼ��������˹�в��������Ӧ���ͼ��ϲ�Ϊһ��ͼ�������õ��ϲ����������˹�в��������ͬʱ������˵�mask������2�б�����topL��topR�ϲ�ΪtopLR��

5. ��topLRΪ��������˵�ͼ������pyrUp()������topLR���и�˹�ϲ������õ�upTopLR������upTopLR�벽��4�кϲ���Ĳв��������Ӧ���ͼ����ӣ��ؽ����ò��ͼ��

6. �ظ�����5��ֱ���ؽ�����0�㣬Ҳ���ǽ�������Ͷ˵�ͼ�񣬼�blendImg�������
*/