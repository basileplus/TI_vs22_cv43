#ifndef FILTERING_H
#define FILTERING_H
#include "init.h"
class Configuration;


class Filtering
{
    public:
        Filtering();
        Filtering(Configuration config);
        virtual ~Filtering();

        Mat  GaussKernel ();
        Mat  LOGKernel ();
        Mat  LaplacianKernel ();
        void SobelKernel (Mat &Kx, Mat &Ky);
        Mat convolve(Mat I, Mat K);
        void gradientSobel(Mat I, Mat &Gx, Mat &Gy );
        void gradientNorm(Mat Gx, Mat Gy, Mat &Norm, Mat &Angle );
        void  gradient_t(Mat It_1, Mat It, Mat &Gt );
        void gradients_HS(Mat It_1, Mat It, Mat &Gx, Mat &Gy, Mat &Gt);

        int W ; // width of the filtering kernel

    protected:
    private:
};

#endif // FILTERING_H
