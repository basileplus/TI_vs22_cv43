
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
	for(int i=config.n0; i<=config.nend; i++){
       cout << "Image " << i << endl;

        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);

        imshow("Image initiale", im);
        Mat im_c =c.convert(im, config.colorspace, display);
        imshow("Image convertie", im_c);

     // a completer
        waitKey(0);

}
	}





