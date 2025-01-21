#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "init.h"

class Segmentation
{
    public:
        Segmentation();
        Segmentation(const Mat& im);
        virtual ~Segmentation();
        Point2d first_pix(const Mat I);
        Mat  grow1region(Mat& I, Point2d p);
        Mat add_region(Mat& R, Mat& I, int k);
        Mat regions(const Mat& im);
       // Mat contours(void);
        Mat contours(Mat &I, int n);
        Mat hysteresis(Mat &im, int Sb, int Sh);
        Mat etiquetage_cc(const Mat& im);
        Mat select_reg(const Mat R, int k );
        void  Display(string s, Mat I );

        Mat Iregions;
        Mat Icontours;

        int nr; // nb regions ou external contours
        int rows;
        int cols;

    protected:
    private:
};

#endif // SEGMENTATION_H
