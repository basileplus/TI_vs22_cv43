/***********************************************************************
Exo 7 : Segmentation en regions
Michele GOUIFFES
***********************************************************************/
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Image initiale", cv::WINDOW_AUTOSIZE);moveWindow("Image initiale", 0, 0);
	namedWindow( "Image binaire", cv::WINDOW_AUTOSIZE);moveWindow("Image binaire", 0, Height);
	namedWindow( "Image segmentee", cv::WINDOW_AUTOSIZE ); moveWindow("Image segmentee", (Width), 0);


}

int main()
{
    // read configuration file
    Configuration config;
	config.Load("../config_exo6.cfg");
    config.ReadConfigIm();    // Initialise
    config.ReadConfigStats();
    config.ReadConfigCluster();
    //config.ReadConfigCluster();

    Mat im, im_e, im_b;
    bool display = false;
    for(int i = config.n0; i<= config.nend; i++){
        cout << "Image " << i << endl;

        Color c(config, i, im, true);
        InitWindows(im.cols, im.rows);
       

        Mat im_c =c.convert(im, config.colorspace, display);
        imshow("Image initiale", im_c);

        Clustering cl;
        im_b=cl.kmeans1D(im_c, config.K, 2);
         im_b=select_val(im_b,2);
        imshow("Image binaire", im_b);

        Segmentation s;
        //Mat im_e2=s.regions(im_b);
        //s.Display("Image segmentee", im_e2);



        waitKey(0);
        destroyAllWindows();
    }
   // Mat Labels;

    return 1;
}


