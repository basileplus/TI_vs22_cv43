
#include "init.h"
using namespace cv;
using namespace std;





//  type :1 : histo
/****************************************************************************
*  TP
*  ANALYSE de FORMES
*
*  Author : Michèle Gouiffès

****************************************************************************/


using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, 0);
	namedWindow( "Image binaire", cv::WINDOW_AUTOSIZE ); moveWindow("Image binaire", Width, 0);
	namedWindow( "Image Segmentee", cv::WINDOW_AUTOSIZE ); 	moveWindow("Image Segmentee", 2*Width, 0);
	namedWindow( "Contours des regions", cv::WINDOW_AUTOSIZE ); moveWindow("Contours des regions", 3*Width, 0);

}

int main( int argc, char** argv )
{
    // Initialise
    Configuration config;
	config.Load("../config_exo10.cfg");
    config.ReadConfigIm();
    config.ReadConfigStats();
    config.ReadConfigCluster();


    int s;
    config.Get("s", s);

    Mat im, im_e, im_b;
    bool display = false;

    InitWindows(400, 200);
    // Loop on the images
    for (int i = config.n0; i <= config.nend; i++) {
        cout << "Image " << i << endl;

        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);


        Mat im_c = c.convert(im, config.colorspace, display);
        imshow("Image initiale", im_c);




        // Etalement de l'histogramme sur le channel 2

        // Extraction du deuxième canal
        vector<Mat> channels;
        split(im_c, channels);
        Mat second_channel = channels[2]; // Le deuxième canal
        imshow("Second Channel", second_channel);

        //histogramme initial
        Stats st;
        Mat H1D;
        H1D = st.histo_1D(im_c, 2, display);
        imshow("Initial Histo", H1D);

        // Appliquer un flou gaussien à l'image
        GaussianBlur(im_c, im_c, Size(11, 11), 0);
        imshow("Image floue 1 ", im_c);

        //histogramme etire
        channels[2] = c.HistoStretching(channels[2]); // on etire que le channel 2
        merge(channels, im_e);
        imshow("Etirement : image", im_e);
        H1D = st.histo_1D(im_e, 2, display);
        imshow("Etirement : histo", H1D);

        // Extraction du deuxième canal apres etirement
        split(im_e, channels);
        second_channel = channels[2]; // Le deuxième canal
        imshow("Second Channel after stretching", second_channel);

        // Appliquer un flou gaussien à l'image
        GaussianBlur(im_e, im_e, Size(21, 21), 0);
        imshow("Image floue 2", im_e);

        Clustering cl;
        im_b = cl.kmeans1D(im_e, config.K, 2);
        //im_b = select_val(im_b, 2);
        Display("Image binaire", im_b);

        Segmentation s(im_b);

        Mat im_r = s.regions(im_b);
        s.Display("Image Segmentee", im_r);
        

        waitKey(0);
    }
}





