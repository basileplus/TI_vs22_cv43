#ifndef MORPHO_H
#define MORPHO_H

#include "init.h"

class Morpho
{
    public:
        Morpho();
        virtual ~Morpho();

        Mat erode(const Mat& src, int k);
        Mat erode(const Mat& src, const Mat&es, int k);
        Mat dilate(const Mat& src, int k);
        Mat dilate(const Mat& src, const Mat&es, int k);
        Mat closing(const Mat& src);
        Mat opening(const Mat& src);
        Mat contours(const Mat& src);
        Mat reconstruction(const Mat& src1, const Mat& src2);
        Mat zeros(const Mat& src );
    protected:
    private:
};

#endif // MORPHO_H
