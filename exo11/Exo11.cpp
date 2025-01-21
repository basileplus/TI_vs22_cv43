
#include "init.h"
using namespace cv;
using namespace std;



void InitWindows(int Width, int Height)
{
	// Creation de fenetres d'affichage
	namedWindow( "Initial Video", cv::WINDOW_AUTOSIZE);moveWindow("Initial Video", 10, 10);
	namedWindow( "1- Simple Motion Detection", cv::WINDOW_AUTOSIZE ); moveWindow("1- Simple Motion Detection", Width, 10);
	namedWindow( "2- Filtering", cv::WINDOW_AUTOSIZE ); 	moveWindow("2- Filtering", 2*Width, 10);
	namedWindow( "3- Normalization", cv::WINDOW_AUTOSIZE); moveWindow("3- Normalization", 0, Height);
	namedWindow( "4- Background update (mean)", cv::WINDOW_AUTOSIZE); moveWindow("4- Background update (mean)", Width, Height);
    namedWindow( "5- Background update (median)", cv::WINDOW_AUTOSIZE ); moveWindow("5- Background update (median)", 2*Width, Height);
    namedWindow( "6- U", cv::WINDOW_AUTOSIZE ); moveWindow("6- U", 0, 2*Height);
    namedWindow( "7- V", cv::WINDOW_AUTOSIZE ); moveWindow("7- V", Width, 2*Height);
    namedWindow( "8- Motion vectors", cv::WINDOW_AUTOSIZE); moveWindow("8- Motion vectors", 2*Width, 2*Height);



}

int main( int argc, char** argv )
{
    Mat I0, It, It_1;								  //	Initial and current images
	Mat N0, Nt ;							  //	Images after photometric normalization
	Mat Ft;									  //	Current Foreground image
	Mat Bt, Bt_median;
    Mat Vx, Vy, V;
    // Initialise
    Configuration config;
	config.Load("../config_exo11.cfg");
    config.ReadConfigIm();
    Color c(config, config.n0, I0, false);
    InitWindows(WIDTH_WINDOW, HEIGHT_WINDOW);	//	Initialization of the windows for display

    I0.copyTo(Bt);
    I0.copyTo(Bt_median);
    I0.copyTo(It_1);

	N0=c.PhotometricNormalization(I0);			// Photometric normalization

	// Loop on the images
	for(int n=config.n0+1; n<=config.nend; n++){

            Color c(config, n, It, false);
            Motion m(config);


			imshow( "Initial Video", It );


			Ft=m.SimpleBackgroundSubtract(I0, It, 0);
			imshow( "1- Simple Motion Detection", Ft );

			//simple background subtraction + filtering
			Ft=m.SimpleBackgroundSubtract(I0, It, 1);
			imshow( "2- Filtering", Ft );

			//photometric normalization
			Nt=c.PhotometricNormalization(It);
			Ft=m.SimpleBackgroundSubtract(N0, Nt, 1);
			imshow( "3- Normalization", Ft );


			//background update (mean)
			Ft=m.SimpleBackgroundSubtract(Bt, It, 0);
			imshow( "4- Background update (mean)", Ft);
			m.BackgroundUpdateMean(Bt, It);


            //background update (median)
			Ft=m.SimpleBackgroundSubtract(Bt_median, It, 0);
			imshow( "5- Background update (median)", Ft);
			Bt_median = m.BackgroundUpdateMedian(I0, It, It_1);


     /*       m.HornSchunck(It_1, It, Vx, Vy) ;
			V=m.DrawMotion(It_1, It, 5,Vx, Vy);
			imshow("6- U", Vx);
			imshow("7- V", Vy);
			imshow("8- Motion vectors", V);*/

            It.copyTo(It_1);

			waitKey(0); // Attente

	}

	return 0;
}

