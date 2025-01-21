#include "Filtering.h"

Filtering::Filtering()
{
    //ctor
}

Filtering::Filtering(Configuration config)
{
    if(!config.Get("W", W))
	{
		cout << "Problem to get W (in Filtering.cpp), check configVideo.cfg. The default value is used.  "  << endl;
		W = 3;
	}


}

Filtering::~Filtering()
{
    //dtor
}

/* -------------------------------------------------------
Gaussian Kernel
width of the kernel given by W
sigma =W/2
---------------------------------------------------------*/
Mat  Filtering::GaussKernel ()
{
	Mat g=Mat::zeros(W,W, CV_32F);
	int sigma= W/2;
    float A = 1. /(2*PI*sigma*sigma);
    /*===============A COMPLETER =============================*/


    /*===============FIN A COMPLETER =============================*/

//	for(int i=0; i<W; i++)
//		for(int j=0; j<W; j++)
//            g.at<float>(i,j)/=c;


	return(g);
}

/* -------------------------------------------------------
Laplacian Kernel
8-connexity
size 3 x 3
---------------------------------------------------------*/
Mat  Filtering::LaplacianKernel ()
{
	Mat L=Mat::zeros(3,3, CV_32F);
	//Zeros(L);

    float a =- 1./8;

    L.at<float>(0,0) = a;
    L.at<float>(0,1) = a;
    L.at<float>(0,2) = a;
    L.at<float>(1,0) = a;
    L.at<float>(1,1) = -8*a;
    L.at<float>(1,2) = a;
    L.at<float>(2,0) = a;
    L.at<float>(2,1) = a;
    L.at<float>(2,2) = a;

	return(L);
}



 /* -------------------------------------------------------
Sobel Kernels
size 3 x 3

---------------------------------------------------------*/
void  Filtering::SobelKernel (Mat &Kx, Mat &Ky)
{
    Kx=Mat::zeros(3,3, CV_32F);
    Ky=Mat::zeros(3,3, CV_32F);

    float a = 0.25;
    float b = 0.5;

    Kx.at<float>(0,0) = -a;
    Kx.at<float>(0,1) = -b;
    Kx.at<float>(0,2) = -a;
    Kx.at<float>(1,0) =  0;
    Kx.at<float>(1,1) = 0;
    Kx.at<float>(1,2) =0;
    Kx.at<float>(2,0) = a;
    Kx.at<float>(2,1) = b;
    Kx.at<float>(2,2) = a;

    Ky.at<float>(0,0) = -a;
    Ky.at<float>(1,0) = -b;
    Ky.at<float>(2,0) = -a;
    Ky.at<float>(0,1) = 0;
    Ky.at<float>(1,1) =0;
    Ky.at<float>(2,1) =0;
    Ky.at<float>(0,2) = a;
    Ky.at<float>(1,2) = b;
    Ky.at<float>(2,2) = a;

}

 /* -------------------------------------------------------
Convolve I with kernel K

---------------------------------------------------------*/
Mat Filtering::convolve(Mat I, Mat K)
{
    Mat J=Mat::zeros(I.rows, I.cols, CV_32F);

    int w = K.rows/2;
    float res;
    /*===============A COMPLETER =============================*/


    /*===============FIN A COMPLETER =============================*/


return(J);
}


/* -------------------------------------------------------
Norm Gradient
- Gx, Gy : components of spatial gradient
- Norm, Angle : norm and orientation
---------------------------------------------------------*/
void Filtering::gradientNorm(Mat Gx, Mat Gy, Mat &Norm, Mat &Angle )
{

	double u, v, a;
	Norm=Mat::zeros(Gx.rows, Gx.cols, CV_8U);
	Angle=Mat::zeros(Gx.rows, Gx.cols, CV_8U);
	for(int i=1; i<Gx.rows-1; i++){
		for(int j=1; j<Gx.cols -1; j++){
			u=Gx.at<float>(i,j);
			v=Gy.at<float>(i,j);
			Norm.at<uchar>(i,j)=	(uchar) sqrt((double)u*u+v*v)/sqrt((double)2) ;

			if ((u>0) && (v>=0) )
				a=atan(v/u);
			if ((u>0) && (v<0) )
				a=atan(v/u)+2*PI;
			if ((u<0) )
				a=atan(v/u)+PI;
			if ((u==0) && (v>0) )
				a=PI/2;
			if ((u==0) && (v<0) )
				a=3*PI/2;
			Angle.at<uchar>(i,j)= (uchar)( (a /2*PI)*255);

		}
	}
}

/* -------------------------------------------------------
Temporal Gradient
- It_1 : previous image
- It : current image
- Gt : temporal gradient
---------------------------------------------------------*/
void Filtering::gradient_t(Mat It_1, Mat It, Mat &Gt )
{
	Gt=Mat::zeros(It_1.rows, It_1.cols, CV_64F);
	for(int i=1; i<It_1.rows-1; i++){
		for(int j=1; j<It_1.cols -1; j++){
			double gt=0;
			for(int k=-1; k<=1; k++){
				for(int l=-1; l<=1; l++){
					gt+=(double)It.at<uchar>(i+k, j+l) - (double)It_1.at<uchar>(i+k, j+l);
					}
			}
			Gt.at<double>(i,j)= (double)(gt/9);
		}
	}
}
/* -------------------------------------------------------
gradients used in Horn & Schunck optical flow method
- It_1 : previous image
- It : current image
Outputs :
- Gx, Gy : spatial gradients
- Gt : temporal gradient
---------------------------------------------------------*/
void Filtering::gradients_HS(Mat It_1, Mat It, Mat &Gx, Mat &Gy, Mat &Gt)
{
	int nrows=It_1.rows;
	int ncols=It_1.cols;

	Gx.zeros(nrows, ncols, CV_32F);
	Gy.zeros(nrows, ncols, CV_32F);
	Gt.zeros(nrows, ncols, CV_32F);

	for(int i=0; i<nrows-1; i++){
		for(int j=0; j<ncols-1; j++){
					Gy.at<float>(i,j)=	((float)It.at<uchar>(i,j+1) - (float)It.at<uchar>(i,j) +
							(float)It.at<uchar>(i+1,j+1) - (float)It.at<uchar>(i+1,j) +
							(float)It_1.at<uchar>(i,j+1) - (float)It_1.at<uchar>(i,j) +
							(float)It_1.at<uchar>(i+1,j+1) - (float)It_1.at<uchar>(i+1,j))/4 ;

					Gx.at<float>(i,j)=	((float)It.at<uchar>(i+1,j) - (float)It.at<uchar>(i,j) +
							(float)It.at<uchar>(i+1,j+1) - (float)It.at<uchar>(i,j+1) +
							(float)It_1.at<uchar>(i+1,j) - (float)It_1.at<uchar>(i,j) +
							(float)It_1.at<uchar>(i+1,j+1) - (float)It_1.at<uchar>(i,j+1))/4 ;

					Gt.at<float>(i,j)=	((float)It.at<uchar>(i,j)	- (float)It_1.at<uchar>(i,j) +
							(float)It.at<uchar>(i+1,j)	- (float)It_1.at<uchar>(i+1,j) +
							(float)It.at<uchar>(i,j+1)	- (float)It_1.at<uchar>(i,j+1) +
							(float)It.at<uchar>(i+1,j+1)- (float)It_1.at<uchar>(i+1,j+1))/4 ;
		}
	}

}
