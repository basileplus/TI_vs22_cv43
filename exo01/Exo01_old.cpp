/***********************************************************************
Exo 1 Image, couleur, histogrammes
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;

//#define FILE "C://Users//gouiffes//Documents//Enseignement//TP_vs_cv43//TI_vs19_cv43//config_exo1b.cfg"

void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "1-Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("1-Initial Image", 0, 0);
	namedWindow( "2-Color space change ", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color space change ", 0, Height);
    namedWindow( "2-Color 0", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 0", Width, 0);
    namedWindow( "2-Color 1", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 1", Width, Height);
    namedWindow( "2-Color 2", cv::WINDOW_AUTOSIZE ); moveWindow("2-Color 2", Width, 2*Height);

	namedWindow( "3-1D Histogram (0)", cv::WINDOW_AUTOSIZE ); 	moveWindow("3-1D Histogram (0)", 2*Width, 0);
	namedWindow( "3-1D Histogram (1)", cv::WINDOW_AUTOSIZE ); 	moveWindow("3-1D Histogram (1)", 2*Width, Height);
	namedWindow( "3-1D Histogram (2)", cv::WINDOW_AUTOSIZE ); 	moveWindow("3-1D Histogram (2)", 2*Width, 2*Height);

	namedWindow( "4-2D Histogram (01)", cv::WINDOW_AUTOSIZE ); moveWindow("4-2D Histogram (01)", 3*Width, 0);
    namedWindow( "4-2D Histogram (12)", cv::WINDOW_AUTOSIZE ); moveWindow("4-2D Histogram (12)", 3*Width, Height);
    namedWindow( "4-2D Histogram (20)", cv::WINDOW_AUTOSIZE ); moveWindow("4-2D Histogram (20)", 3*Width, 2*Height);

	namedWindow( "Image seuillee", cv::WINDOW_AUTOSIZE ); moveWindow("Image seuillee", 4*Width, 0);

}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo1b.cfg");
    config.ReadConfigIm();    // Initialise
    int th1, th2, th3, th4;
    config.Get("th1", th1);
    config.Get("th2", th2);
    config.Get("th3", th3);
    config.Get("th4", th4);

    Mat RGB;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;

        //changement espace couleur
        Color c(config, i, RGB, true);
        InitWindows(RGB.cols, RGB.rows);
        imshow("1-Initial Image", RGB);

        Mat im =c.convert(RGB, config.colorspace, display);
        imshow("2-Color space change ", im);

        Mat channel;
        Stats st;

        channel= c.select_channel(im, 0);
        imshow("2-Color 0", channel);
        Mat H1D1 = st.histo_1D(channel, 0, display);
        imshow("3-1D Histogram (0)", H1D1 );


        channel= c.select_channel(im, 1);
        imshow("2-Color 1", channel);
        Mat H1D2 = st.histo_1D(channel, 1, display);
        imshow("3-1D Histogram (1)", H1D2 );

        channel= c.select_channel(im, 2);
        imshow("2-Color 2", channel);
        Mat H1D3 = st.histo_1D(channel, 2, display);
        imshow("3-1D Histogram (2)", H1D3 );
        //waitKey(0);

        Mat H2D1 = st.histo_2D( im, 0, 1, display);
        Display("4-2D Histogram (01)", H2D1);
        Mat H2D2 = st.histo_2D( im, 1, 2, display);
        Display("4-2D Histogram (12)", H2D2);
        Mat H2D3 = st.histo_2D( im, 2, 0, display);
        imshow("4-2D Histogram (20)", H2D3);

        channel= c.select_channel(im, 0);

        Clustering cl;
        channel= c.select_channel(im, 2);
        Mat im_s1=cl.threshold(channel, th1, th2);

        channel= c.select_channel(im, 0);
        Mat im_s2=cl.threshold(channel, th3, th4);


        Mat im_s;
        bitwise_and(im_s2, im_s1, im_s);
        Display("Image seuillee", im_s);
        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}

