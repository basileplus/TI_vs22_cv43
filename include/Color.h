#ifndef COLOR_H
#define COLOR_H
#include "init.h"

class Configuration;

class Color
{
    public:
        Color();
        Color(Configuration  cfg, int no_im, Mat &image, bool type);
        virtual ~Color();
        int imread_grayscale(Configuration  cfg, int no_im, Mat &image);
        int imread_rgb(Configuration  cfg, int no_im, Mat &image);
        Mat convert(const Mat& in, const string& type, bool display);
        Mat select_channel(const Mat& im, int channel);
        void reshape(const Mat& in, bool display);
        Mat convert_rgb(const Mat& im) const;
        Mat PhotometricNormalization(Mat It);
        Mat HistoStretching(Mat It);
        // Mat LogTransform(Mat It, int R);
        Mat LogTransform(Mat It, float R);

        Mat ExpTransform(Mat It, float R);

        Mat t_im[3] ; // " plans couleur
        Mat im2C;
        bool color;


    protected:

    private:

};

#endif // COLOR_H
