#include "Color.h"
#include "Stats.h"

Stats::Stats()
{
    //ctor
    histSize = 256;
    hist_w = 512;
    hist_h = 400;
    bin_w = cvRound( (double) hist_w/histSize );


}
Stats::~Stats()
{
    //dtor
}

/*--------------------------------
Histogram : computation and display
im : color or gray image
--------------------------------*/
Mat Stats::histo_1D_3( const Mat& im, bool display)
{
      Mat t_im[3] ;
      split(im, t_im);
      float range[] = { 0, 256 } ;
      const float* histRange = { range };
      bool uniform = true; bool accumulate = false;

      calcHist( &t_im[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
      if(im.channels()==3){
        calcHist( &t_im[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &t_im[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
       }

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

      // Draw the histograms for B, G and R

      normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      if (im.channels()==3)
      {
          normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
          normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    }

      for( int i = 1; i < histSize; i++ )
      {
          cv::line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                           Scalar( 255, 0, 0), 2, 8, 0  );
         if (im.channels()==3) {
              cv::line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                               Scalar( 0, 255, 0), 2, 8, 0  );
              cv::line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                               Scalar( 0, 0, 255), 2, 8, 0  );
                               }
      }


  if(display){
      namedWindow("Histo", WINDOW_AUTOSIZE );
      imshow("Histo", histImage );
  }

return(histImage);

}

/*--------------------------------
Histogram : computation and display (in grey levels)
im : color or gray image
--------------------------------*/
Mat Stats::histo_1D( const Mat& im, int channel, bool display)
{
    Mat tmp;
    H1D.create(256, 256, CV_32F);
    Mat H1Ddisp(256, 256, CV_8U);
    Zeros(H1D);
    Zeros(H1Ddisp);
    if(im.channels()>1){
          Mat t_im[3] ;
          split(im, t_im);
          tmp=t_im[channel];
          }
    else tmp=im;
    float hist_max=0;
    /*===============A COMPLETER =============================*/
    for (int i = 1; i < tmp.rows; i++) {
        for (int j = 1; j < tmp.cols; j++) {
            int val = tmp.at<uchar>(i, j);
            H1D.at<float>(val) += 1.;
            hist_max = std::max(hist_max, H1D.at<float>(val));
        }
    }
    /*===============FIN A COMPLETER =============================*/
	  for( int i = 1; i < 256; i++ ){
        int x= 256- (H1D.at<float>(i)/hist_max)*255 ;
        cv::line( H1Ddisp, Point(i, x), Point(i, 255), Scalar( i, 0, 0), 1, 8, 0  );
        }


return(H1Ddisp);

}

/*--------------------------------
Histogram : computation and display (in white and black)
with two thresholds displayed with color bars
INPUTS :
- im : color or gray image
- channel : channel of the image for which the histogram is computed
- th_l, th_h : low and hogh thresholds
- display : a boolean true to dusplay, and false otherwise

OUTPUT :
- a mat reepresenting the histogram : cols are n and rows are h(n)
--------------------------------*/
Mat Stats::histo_1D( const Mat& im,  int channel, int th_l, int th_h, bool display)
{
    Mat tmp;
    H1D.create(256, 256, CV_32F);
    Mat H1Ddisp(256, 256, CV_8UC3);
    Zeros(H1D);
    Zeros(H1Ddisp);
    // if the image has more than one channel, they are split and the channel of interest is kept
    if(im.channels()>1){
          Mat t_im[3] ;
          split(im, t_im);
          tmp=t_im[channel];
          }
    else tmp=im;

    // computes histogram and finds max (for normalization)
    float hist_max=0;
     /*===============A COMPLETER =============================*/
    for (int i = 1; i < tmp.rows; i++) {
		for (int j = 1; j < tmp.cols; j++) {
			int val = tmp.at<uchar>(i, j);
			H1D.at<float>(val) += 1.;
			hist_max = std::max(hist_max, H1D.at<float>(val));
		}
	}
    /*===============FIN A COMPLETER =============================*/

    // draws histogram with the two threshold bars
    int r,g,b;
    switch(channel){
        case 0: r=255; g=b=0;
                break;
        case 1: g=255; r=b=0;
                break;
        break;
        case 2: b=255; r=g=0;
                break;
        default: r=g=b=255;
        break;
    }
    for( int i = 1; i < 256; i++ ){
        int x= 256- (H1D.at<float>(i)/hist_max)*255 ;
        cv::line( H1Ddisp, Point(i, x), Point(i, 255), Scalar(b, g, r), 1, 8, 0  );

        }
    cv::line( H1Ddisp, Point(th_l, 0), Point(th_l, 255), Scalar(255 , 255, 255), 1, 8, 0  );
    cv::line( H1Ddisp, Point(th_h, 0), Point(th_h, 255), Scalar(255 , 255, 255), 1, 8, 0  );

    return(H1Ddisp);

}

/*--------------------------------
Histogram 2D : computation and display
im : color  image
channel0 and channel 1: ccolor channels used for the 2D histo
--------------------------------*/
Mat Stats::histo_2D( const Mat& im, int channel0, int channel1, bool display)
{

 Mat H2Ddisp(256, 256, CV_8UC3);
 H2D.create(256, 256, CV_32F);
    for(int i=0; i<256; i++)
        for(int j=0; j<256; j++){
        H2Ddisp.at<Vec3b>(i,j)=Vec3b(0,0,0);
         H2D.at<float>(i,j)=0;
         }

      if (im.channels()==3) {
          Mat t_im[3] ;
          split(im, t_im);
          Mat im0 = t_im[channel0];
          Mat im1 = t_im[channel1];

          for(int i=0; i<im.rows; i++)
             for(int j=0; j<im.cols; j++){
             int ii=im0.at<uchar>(i,j);
             int jj=im1.at<uchar>(i,j);
             H2D.at<float>(ii,jj)+=1.0;
             H2Ddisp.at<Vec3b>(ii,jj)=im.at<Vec3b>(i,j);
             }
        if(display) {
        Display("Histo 2D", H2Ddisp);

        }
        }

return(H2Ddisp);

}


/*--------------------------------
Histogram 2D : computation and display
im : color  image
channel0 and channel 1: ccolor channels used for the 2D histo
--------------------------------*/
Mat Stats::histo_2D( const Mat& im, int channel0, int channel1, int th0_0, int th0_1,  int th1_0, int th1_1, bool display)
{

 Mat H2Ddisp(256, 256, CV_8UC3);
 H2D.create(256, 256, CV_32F);
    for(int i=0; i<256; i++)
        for(int j=0; j<256; j++){
        H2Ddisp.at<Vec3b>(i,j)=Vec3b(0,0,0);
         H2D.at<float>(i,j)=0;
         }

      if (im.channels()==3) {
          Mat t_im[3] ;
          split(im, t_im);
          Mat im0 = t_im[channel0];
          Mat im1 = t_im[channel1];

          for(int i=0; i<im.rows; i++)
             for(int j=0; j<im.cols; j++){
             int ii=im0.at<uchar>(i,j);
             int jj=im1.at<uchar>(i,j);
             H2D.at<float>(ii,jj)+=1.0;
             H2Ddisp.at<Vec3b>(ii,jj)=im.at<Vec3b>(i,j);
             }

        cv::line( H2Ddisp, Point(0, th0_0), Point(255, th0_0), Scalar(255 , 255, 255), 1, 8, 0  );
        cv::line( H2Ddisp, Point(0, th0_1), Point(255, th0_1), Scalar(255 , 255, 255), 1, 8, 0  );

        cv::line( H2Ddisp, Point(th1_0, 0), Point(th1_0, 255), Scalar(255 , 255, 255), 1, 8, 0  );
        cv::line( H2Ddisp, Point(th1_1, 0), Point(th1_1, 255), Scalar(255 , 255, 255), 1, 8, 0  );
        if(display) {
        Display("Histo 2D", H2Ddisp);

        }
        }

return(H2Ddisp);

}


/* -------------------------------------------------
statsCooc

Calcule des statistiques (cooccurrence) globales dans l'image
- I : image en couleur
- type_vs : type de voisinage
- display : affichage
--------------------------------------------------*/

Mat Stats::statsCooc( Mat &I, int channel,  int type_vs )
{
    Point vs[9]= {   Point(-1,-1), Point(0, -1), Point(1,-1),
                    Point(-1,0),                Point(1,0),
                    Point(-1,1), Point(0,1), Point(1,1)};
    int scale =1;
    Cooc.create(256/scale, 256/scale, CV_32F);
    Mat p_disp(256/scale, 256/scale, CV_8U);


    Mat t_im[3] ;
    split(I, t_im);
    Mat im= t_im[channel];

    Point v= vs[type_vs];

    int N=0;


    Zeros(Cooc);
    Zeros(p_disp);

    for (int i=1; i< I.rows-1; i++)
        for(int j=1; j< I.cols-1; j++){
            int val1 = im.at<uchar>(i, j);
            int val2 = im.at<uchar>(i+ v.y, j+v.x);
            Cooc.at<float>(val1/scale, val2/scale)+=1.;
            N++;

        }
  for(int n=0; n<256/scale; n++)
        for(int m=0; m<256/scale; m++) {
            if( Cooc.at<float>(n, m)>=255 ) p_disp.at<uchar>(n, m)= 255;
            else p_disp.at<uchar>(n, m)= (uchar)Cooc.at<float>(n, m) ;
        }
    return(p_disp);
        }

/*--------------------------------
Compute the color statistics of labels
im : color image CV_8UC3
labels : labels from a clustering
returns the image of label
--------------------------------*/
void Stats::colorstats(const Configuration& cfg, const Mat& im, const Mat& labels)
{
    //stats : nb, average RGB, standard deviation RGB, min RGB, max RGB
    int K = cfg.K;
    stats.create(K+1,13, CV_64F);
    imshow("3-Clustering ", labels*255/3);
    waitKey(1);



    //initialisation
    for(int l=0; l<=K; l++){
        stats.at<double>(l,  0) = 0.;
        for(int k=0; k<3; k++){
             stats.at<double>(l,  k+1) = 0.;
             stats.at<double>(l,  k+4) = 0.;
             stats.at<double>(l,  k+7) = (double) UCHAR_MAX;
             stats.at<double>(l,  k+10) = 0.;

         }
         }

    for(int i=0; i<im.rows; i++)
        for(int j=0; j<im.cols; j++){
            int l= labels.at<uchar>(i,j);
            Vec3b c =im.at<Vec3b>(i,j);
            stats.at<double>(l,  0) ++;

           for(int k=0; k<3; k++){
                    stats.at<double>(l,  k+1)   +=   (double) c.val[k];
                     stats.at<double>(l,  k+4)   +=  ( (double)c.val[k]  * (double)c.val[k] );
                     stats.at<double>(l,  k+7)   =   testMin(stats.at<double>(l,  k+7), (double) c.val[k]);
                     stats.at<double>(l,  k+10)  =   testMax(stats.at<double>(l,  k+10),  (double)c.val[k]);
                     }
             }

    for(int l=1; l<=K; l++){
    int n=stats.at<double>(l,  0);
        for(int k=0; k<3; k++){
            stats.at<double>(l,  k+1) = stats.at<double>(l,  k+1) /n;
            stats.at<double>(l,  k+4) =  stats.at<float>(l,  k+4)/ n ;
        }
        }

}

