 #include <opencv2/opencv.hpp>
 #include <iostream>
 #include <string>
 
 using namespace std;
 using namespace cv;
 
 /************************************************************************/
 /* ˵����
 *���������µ�������Ϊ [0,1��...��level-1] ��
 *blendMask Ϊͼ�����ģ
 *maskGaussianPyramidΪ������ÿһ�����ģ
 *resultLapPyr ���ÿ���������ֱ����������ͼLaplacian�任ƴ�ɵ�ͼ��
 */
 /************************************************************************/
 
 
 class LaplacianBlending {
 private:
     Mat left;
     Mat right;
     Mat blendMask;
 
     //Laplacian Pyramids
     vector<Mat> leftLapPyr, rightLapPyr, resultLapPyr;
     Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;
     //maskΪ��ͨ������������
     vector<Mat> maskGaussianPyramid; 
 
     int levels;
 
     void buildPyramids() 
     {
         buildLaplacianPyramid(left, leftLapPyr, leftHighestLevel);
         buildLaplacianPyramid(right, rightLapPyr, rightHighestLevel);
         buildGaussianPyramid();
     }
 
     void buildGaussianPyramid() 
     {
         //����������Ϊÿһ�����ģ
         assert(leftLapPyr.size()>0);
 
         maskGaussianPyramid.clear();
         Mat currentImg;
         cvtColor(blendMask, currentImg, CV_GRAY2BGR);
         //����mask��������ÿһ��ͼ��
         maskGaussianPyramid.push_back(currentImg); //0-level
 
         currentImg = blendMask;
         for (int l = 1; l<levels + 1; l++) {
             Mat _down;
             if (leftLapPyr.size() > l)
                 pyrDown(currentImg, _down, leftLapPyr[l].size());
             else
                 pyrDown(currentImg, _down, leftHighestLevel.size()); //lowest level
 
             Mat down;
             cvtColor(_down, down, CV_GRAY2BGR);
             //add color blend mask into mask Pyramid
             maskGaussianPyramid.push_back(down);
             currentImg = _down;
         }
     }
 
     void buildLaplacianPyramid(const Mat& img, vector<Mat>& lapPyr, Mat& HighestLevel)
     {
         lapPyr.clear();
         Mat currentImg = img;
         for (int l = 0; l<levels; l++) {
             Mat down, up;
             pyrDown(currentImg, down);
             pyrUp(down, up, currentImg.size());
             Mat lap = currentImg - up;
             lapPyr.push_back(lap);
             currentImg = down;
         }
         currentImg.copyTo(HighestLevel);
     }
 
     Mat reconstructImgFromLapPyramid() 
     {
         //������laplacianͼ��ƴ�ɵ�resultLapPyr��������ÿһ��
         //���ϵ��²�ֵ�Ŵ���в���ӣ�����blendͼ����
         Mat currentImg = resultHighestLevel;
         for (int l = levels - 1; l >= 0; l--)
         {
             Mat up;
             pyrUp(currentImg, up, resultLapPyr[l].size());
             currentImg = up + resultLapPyr[l];
         }
         return currentImg;
     }
 
     void blendLapPyrs() 
     {
         //���ÿ���������ֱ����������ͼLaplacian�任ƴ�ɵ�ͼ��resultLapPyr
         resultHighestLevel = leftHighestLevel.mul(maskGaussianPyramid.back()) +
             rightHighestLevel.mul(Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid.back());
         for (int l = 0; l<levels; l++) 
         {
             Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
             Mat antiMask = Scalar(1.0, 1.0, 1.0) - maskGaussianPyramid[l];
             Mat B = rightLapPyr[l].mul(antiMask);
             Mat blendedLevel = A + B;
 
             resultLapPyr.push_back(blendedLevel);
         }
     }
 
 public:
     LaplacianBlending(const Mat& _left, const Mat& _right, const Mat& _blendMask, int _levels) ://construct function, used in LaplacianBlending lb(l,r,m,4);
         left(_left), right(_right), blendMask(_blendMask), levels(_levels)
     {
         assert(_left.size() == _right.size());
         assert(_left.size() == _blendMask.size());
         //����������˹�������͸�˹������
         buildPyramids();
         //ÿ�������ͼ��ϲ�Ϊһ��
         blendLapPyrs();
     };
 
     Mat blend() 
     {
         //�ؽ�������˹������
         return reconstructImgFromLapPyramid();
     }
 };
 
 Mat LaplacianBlend(const Mat &left, const Mat &right, const Mat &mask) 
 {
     LaplacianBlending laplaceBlend(left, right, mask, 10);
     return laplaceBlend.blend();
 }
 
 int main() {
     Mat img8UL = imread("data/apple.jpg");
     Mat img8UR = imread("data/orange.jpg");
 
     int imgH = img8UL.rows;
     int imgW = img8UL.cols;
 
     imshow("left", img8UL);
     imshow("right", img8UR);
 
     Mat img32fL, img32fR; 
     img8UL.convertTo(img32fL, CV_32F);
     img8UR.convertTo(img32fR, CV_32F);
 
     //����mask
     Mat mask = Mat::zeros(imgH, imgW, CV_32FC1);
     mask(Range::all(), Range(0, mask.cols * 0.5)) = 1.0;
 
     Mat blendImg = LaplacianBlend(img32fL, img32fR, mask);
 
     blendImg.convertTo(blendImg, CV_8UC3);
     imshow("blended", blendImg);
 
     waitKey(0);
     return 0;
 }