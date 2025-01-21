#ifndef SIMILARITY_H
#define SIMILARITY_H
#include "init.h"
using namespace cv;
using namespace std;

float fSAD( Mat I0, Mat I1, Point p0, Point p1, Point s);
float fSADN( Mat I0, Mat I1, Point p0, Point p1, Point s);
float fSADcol( Mat I0, Mat I1, Point p0, Point p1, Point s);
float fHAD( Mat I0, Mat I1, Point p0, Point p1, Point s);


void LTP(Mat I, uchar Th, Mat &U, Mat &L);
float fLTP( Mat U0, Mat L0, Mat U1, Mat L1, Point c0, Point c1, Point s);


#endif
