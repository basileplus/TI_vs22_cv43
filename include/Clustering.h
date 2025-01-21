#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "init.h"
class Configuration;
class Color;

class Clustering
{
    public:
        Clustering();
        virtual ~Clustering();

        void histo_1D(const Color& c, bool display);
        Mat histo_2D(Mat in) const;
        Mat kmeans1D(Mat I, int K, int channel);
        Mat kmeans2D(Mat I, int K,  int c1, int c2);
        void colorstats(const Configuration& cfg, const Mat& im, const Mat& labels);
        Mat select_label(const Mat& labels, float color[]) const;
        Mat select_label(const Mat& labels, Mat &centers, float color[], int n) const;
        Mat threshold(Mat I, int t1, int t2);

        Mat hysteresis(const Mat& src, int Sb, int Sh);

        int histSize;
        Mat b_hist, g_hist, r_hist;
        int hist_w;
        int hist_h;
        int bin_w ;
        Mat stats; // average, std dev, min, max


    protected:
    private:
};

#endif // CLUSTERING_H
