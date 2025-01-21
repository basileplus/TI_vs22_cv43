#ifndef SHAPE_H
#define SHAPE_H
#include "init.h"

class Segmentation;


class Shape
{
    public:
        Shape(const Segmentation& s);
        Shape();
        virtual ~Shape();

        void compute(const Segmentation& s);

        void signature(const Mat& label);
        void signature(const Mat& label,cv::Point2d c);
        void detectTriangle(const Mat& label);
        void detectRectangle(const Mat& label);
        void detectCircle(const Mat& label);
        void detectHexagone(const Mat& label);
        void analysis(void);
        Mat HoughTransform (const Mat I, int Ntheta, int Nrho );
        void HoughTransformAnalysis (Mat& H, int Dtheta, int Drho, int Nmax);
        Mat t_center;
        Mat t_limits;
        std::vector<int> t_area;

        //Mat t_sig;
        std::list< vector<int> > t_sig;
        Mat t_ir;
        Mat t_hu;
        std::vector<int> t_perim;
        std::vector<Moments> t_mom;


    protected:
    private:
};

#endif // SHAPE_H
