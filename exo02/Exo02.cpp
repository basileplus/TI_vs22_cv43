/***********************************************************************
Exo 2 : Modification d' Histogrammes

Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Initial Image", cv::WINDOW_AUTOSIZE);moveWindow("Initial Image", 0, 0);
	namedWindow( "Initial Histo", cv::WINDOW_AUTOSIZE);moveWindow("Initial Histo", 0, Height);

	namedWindow( "Etirement : image", cv::WINDOW_AUTOSIZE ); moveWindow("Etirement : image", (Width), 0);
    namedWindow( "Etirement : histo", cv::WINDOW_AUTOSIZE ); moveWindow("Etirement : histo", (Width), Height);

	namedWindow( "Log : image", cv::WINDOW_AUTOSIZE ); moveWindow("Log : image", 2*(Width), 0);
    namedWindow( "Log : histo", cv::WINDOW_AUTOSIZE ); moveWindow("Log : histo", 2*(Width), Height);

	namedWindow( "Exp : image", cv::WINDOW_AUTOSIZE ); moveWindow("Exp : image", 3*(Width), 0);
    namedWindow( "Exp : histo", cv::WINDOW_AUTOSIZE ); moveWindow("Exp : histo", 3*(Width), Height);

	namedWindow( "Egalisation : image", cv::WINDOW_AUTOSIZE ); moveWindow("Egalisation : image", 4*(Width), 0);
    namedWindow( "Egalisation : histo", cv::WINDOW_AUTOSIZE ); moveWindow("Egalisation : histo", 4*(Width), Height);


}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo2.cfg");
    config.ReadConfigIm();    // Initialise
    config.ReadConfigStats();
    //config.ReadConfigCluster();

    Mat im, im_e;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;


        //image en couleur
        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);
        imshow("Initial Image", im);

        // Conversion dans espace de couleur
        Mat im_c = c.convert(im, config.colorspace, display);
        imshow("Image initiale", im_c);

        // Extraction du deuxième canal
        vector<Mat> channels;
        split(im_c, channels);
        Mat second_channel = channels[2]; // Le deuxième canal (index 1)
        imshow("Second Channel", second_channel);

        //histogramme initial
        Stats st;
        Mat H1D;
        H1D = st.histo_1D(im_c, 2, display);
        imshow("Initial Histo", H1D );

          //histogramme etire
        im_e=c.HistoStretching(second_channel);
        imshow("Etirement : image", im_e);
        H1D = st.histo_1D(im_e, 2, display);
        imshow("Etirement : histo", H1D );


        im_e=c.LogTransform(second_channel, 1.5);
        imshow("Log : image", im_e);
        H1D = st.histo_1D(im_e, 2, display);
        Display("Log : histo", H1D );

        im_e=c.ExpTransform(second_channel, 1);
        imshow("Exp : image", im_e);
        H1D = st.histo_1D(im_e, 2, display);
        Display("Exp : histo", H1D );

 /*       histogramme egalise
        equalizeHist(im_c, im_e);
        imshow("Egalisation : image", im_e);
        H1D = st.histo_1D(im_e, 2, display);
        imshow("Egalisation : histo", H1D );*/


        waitKey(0);
        destroyAllWindows();
    }

    return 1;
}


