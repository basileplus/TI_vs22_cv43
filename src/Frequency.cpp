#include "Frequency.h"


#define REAL 0
#define IMAG 1



Frequency::Frequency(Mat I)
{
    DFT_rows = getOptimalDFTSize( I.rows );
    DFT_cols = getOptimalDFTSize( I.cols );
}
Frequency::Frequency(Mat I, Configuration  cfg)
{
    W = cfg.W;
    DFT_rows = getOptimalDFTSize( I.rows );
    DFT_cols = getOptimalDFTSize( I.cols );
}
Frequency::~Frequency()
{
    //dtor
}


Mat Frequency::FFT_spectrum(Mat I)
{
// [expand]
    Mat J;
    I.copyTo(J);
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( J.rows );
    int n = getOptimalDFTSize( J.cols ); // on the border add zero values
    copyMakeBorder(J, padded, 0, m - J.rows, 0, n - J.cols, BORDER_CONSTANT, Scalar::all(0));

// [complex_and_real]
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

// [dft]
    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
// [magnitude]
    split(complexI, imTFD);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    split(complexI, planes);
     magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];


// [log]
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

// [crop_rearrange]
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

// [normalize]
    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
    int Width=400;
    int Height=400;
    waitKey();
    return magI;


}



/* ------------------------------
fft_z
renvoie partie reelle et imaginaire de la FFT
--------------------------------*/
Mat Frequency::fft_z(Mat I)
{
    Mat Ipad;
    copyMakeBorder(I, Ipad, 0, DFT_rows - I.rows, 0, DFT_cols - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(Ipad), Mat::zeros(Ipad.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);

    dft(complexI, complexI);
    cout << " " << complexI.type() << " " << complexI.channels() << endl;
//
    split(complexI,imTFD);//split source


return(complexI);
}
/* ------------------------------
ifft2
prend en entree les parties reelle et imaginaire de la TF
--------------------------------*/
Mat Frequency::ifft(Mat complexI)
{
   Mat Ires; //(complexI.rows, complexI.cols, CV_8U);

   cv::dft(complexI, Ires,cv::DFT_INVERSE|cv::DFT_SCALE|cv::DFT_REAL_OUTPUT );
   return(Ires);

}
/* ------------------------------
fftshift
--------------------------------*/
Mat Frequency::fftshift(Mat im)
{
    Mat imF;
    im.copyTo(imF);
    imF = im(Rect(0, 0, im.cols & -2, im.rows & -2));
    int cx = imF.cols/2;
    int cy = imF.rows/2;
    Mat q0(imF, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(imF, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(imF, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(imF, Rect(cx, cy, cx, cy)); // Bottom-Right
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    split(imF,imTFD);//split source
   // Display("fftshift", imTFD[0]);

    return(imF);


    }

void Frequency::displayTFD( string s)
{


    magnitude(imTFD[0], imTFD[1], imTFD[0]);// planes[0] = magnitude
    Mat D;
    (imTFD[0]).copyTo(D);

    D += Scalar::all(1);                    // switch to logarithmic scale
    log(D, D);
    normalize(D, D, 0, 1, NORM_MINMAX);
    imshow(s, D);

}

/* ------------------------------
Masking
--------------------------------*/
Mat Frequency::masking(Mat I, Mat M)
{
    Mat tmp;
    I.copyTo(tmp);
    int n= I.channels();
    cout << "n channels : " << n<< endl;
    for(int j=0; j<I.cols; j++)
        for(int i=0; i<I.rows; i++)
            if(M.at<float>(i,j)!=0)  for(int k=0; k<n; k++)   tmp.at<Vec2f>(i,j)[k] = I.at<Vec2f>(i,j)[k];
            else                     for(int k=0; k<n; k++)   tmp.at<Vec2f>(i,j)[k] = 0;

   return(tmp);

}

/* ------------------------------
Gaussian Low-Pass filtering
ne necessite pas de fftshift
--------------------------------*/
Mat Frequency::GaussLowPass(Mat imFFT0, float sigma_c, float sigma_r)
{
   Mat imFFT_f;
   imFFT0.copyTo(imFFT_f);
   Mat tmp[2];
   int nrows = imFFT0.rows;
   int ncols = imFFT0.cols;
   int cr = nrows/2;
   int cc = ncols/2;
   Mat G;
   G.create(nrows, ncols, CV_32F);


   for(int fr=0; fr<nrows; fr++)
        for(int fc=0; fc<ncols; fc++) {
            float dr = (fr - cr) ;
            float dc = (fc - cc) ;

            G.at<float>(fr,fc) = exp( -dr * dr   / (2*sigma_r*sigma_r)  -dc* dc   / (2*sigma_c*sigma_c)       );

            }

   split(imFFT0, imTFD);

   multiply(imTFD[0], G, imTFD[0]);
   multiply(imTFD[1], G, imTFD[1]);

   merge(imTFD, 2, imFFT_f);

   return(imFFT_f);

}

/* ------------------------------
Gaussian High-Pass filtering
ne necessite pas de fftshift
--------------------------------*/
Mat Frequency::GaussHighPass(Mat imFFT0, float sigma_c, float sigma_r)
{
   Mat tmp[2];
   int nrows = imFFT0.rows;
   int ncols = imFFT0.cols;
   int cr = nrows/2;
   int cc = ncols/2;

   Mat G=Mat::zeros(nrows, ncols, CV_32F);

    /*===============A COMPLETER =============================*/

   for(int fr=0; fr<nrows; fr++)
        for(int fc=0; fc<ncols; fc++) {

           float dr = (fr - cr) ;
            float dc = (fc - cc) ;

            G.at<float>(fr,fc) = 1- exp( -dr * dr   / (2*sigma_r*sigma_r)  -dc* dc   / (2*sigma_c*sigma_c)       );
            }
    /*===============FIN A COMPLETER =============================*/


   split(imFFT0, imTFD);

   multiply(imTFD[0], G, imTFD[0]);
   multiply(imTFD[1], G, imTFD[1]);

   merge(imTFD, 2, imFFT0);

   return(imFFT0);

}


/* ------------------------------
Gaussian Pass-band filtering
ne necessite pas de fftshift
--------------------------------*/
Mat Frequency::GaussBandPass(Mat imFFT0, float sigma_c, float sigma_r, float f0c, float f0r)
{
   Mat tmp[2];
   int nrows = imFFT0.rows;
   int ncols = imFFT0.cols;
   int cr = nrows/2;
   int cc = ncols/2;

   Mat G;
   G.create(nrows, ncols, CV_32F);

   for(int fr=0; fr<nrows; fr++)
        for(int fc=0; fc<ncols; fc++) {
            float dr = (fr - (cr + f0r)) ;
            float dc = (fc - (cc + f0c)) ;

            G.at<float>(fr,fc) = exp( -dr * dr   / (2*sigma_r*sigma_r) -  dc*dc/ (2*sigma_c*sigma_c)      );

            }


   split(imFFT0, tmp);

   multiply(tmp[0], G, tmp[0]);
   multiply(tmp[1], G, tmp[1]);


   merge(tmp, 2, imFFT0);

   return(imFFT0);

}



