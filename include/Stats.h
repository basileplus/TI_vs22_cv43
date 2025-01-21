#ifndef STATS_H
#define STATS_H

#include "init.h"
class Configuration;
class Color;

class Stats
{
    public:
        Stats();
        virtual ~Stats();

        Mat histo_1D_3(const Mat& c, bool display);
        Mat histo_1D( const Mat& im, int channel, bool display);
        Mat histo_1D( const Mat& im, int channel, int th_l, int th_h, bool display);

        //Mat histo_1D( const Mat& im, int channel);

        Mat  histo_2D( const Mat& im, int channel0, int channel1, bool display);
        Mat histo_2D( const Mat& im, int channel0, int channel1, int th0_0, int th0_1,  int th1_0, int th1_1, bool display);

        //Mat histo_2D( const Mat& im, int channel0, int channel1);
        void colorstats(const Configuration& cfg, const Mat& im, const Mat& labels);
        Mat statsCooc( Mat &I, int channel, int type_vs );

        int histSize;
        Mat b_hist, g_hist, r_hist;
        int hist_w;
        int hist_h;
        int bin_w ;
        Mat stats; // average, std dev, min, max
        Mat H1D;
        Mat H2D;
        Mat Cooc;


    protected:
    private:
};

#endif // STATS_H
