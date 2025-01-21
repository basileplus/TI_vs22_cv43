#ifndef MOTION_H
#define MOTION_H

#include "init.h"
using namespace cv;
using namespace std;

class Motion
{
public:
    //constructor for bgs
    Motion();
	 Motion(Configuration   config);

private:
	int  bs_th; // threshold to detect motion
	double bs_alpha; 		// mixture parameter a
    int hs_itermax=2;
	double hs_alpha=1;

	int simi_norm_th=0;
	int simi_angle_th=0;
    int motion_threshold=255;
    bool  Similarity(float Norm_p, float Norm_q, float Angle_p, float Angle_q);
    int grow_velocity(Mat Vn, Mat Va, int It_1, int j0,  Mat &ToDo, Mat &R);

public:
    Mat SimpleBackgroundSubtract(Mat I0, Mat It, bool filter);
    void BackgroundUpdateMean(Mat &Bt, Mat It);
	Mat  BackgroundUpdateMedian(Mat I1, Mat I2, Mat I3);
	void HornSchunck(Mat I0, Mat I1, Mat &U, Mat &V);
	void MotionNormAngle(Mat Vx, Mat Vy, Mat &Vn, Mat &Va);
	void MotionHistogram(Mat Vn, Mat Va, Mat &H, int N);
	void MotionSegment(Mat Vn, Mat Va, Mat &R);
	Mat DrawMotion(Mat I0, Mat I1, int a, Mat Vx, Mat Vy);
};

#endif
