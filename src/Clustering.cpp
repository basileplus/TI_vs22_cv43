#include "Color.h"
#include "Morpho.h"
#include "Clustering.h"


Clustering::Clustering()
{

    histSize = 256;
    hist_w = 512;
    hist_h = 400;
    bin_w = cvRound( (double) hist_w/histSize );


}

Clustering::~Clustering()
{
    //dtor
}


/* ------------------------------------------------------------
hysteresis thresholding
INPUT :
- Sb : low threshold
- Sh : high threshold
OUTPUT :
- binary image Mat
---------------------------------------------------------------*/
Mat Clustering::hysteresis(const Mat& src, int Sb, int Sh)
{
        Mat im_Sb, im_Sh;
        Mat res, res2;

        cv::threshold(src, im_Sb, Sb, 255, CV_8U);
        cv::threshold(src, im_Sh, Sh, 255, CV_8U);

        Morpho m;
        res= m.reconstruction(im_Sh, im_Sb);

        return(res);

}

/* ------------------------------------------------------------
K-means clustering
INPUT :
- I : initial image
- K : number of clusters expected
- channel : number of the channel to be treated
OUTPUT :
-  image of K color classes Mat
---------------------------------------------------------------*/
Mat Clustering::kmeans1D(Mat I, int K, int channel)
{
    float q= 256/K;     //largeur de chaque classe
    float error=0;
    Mat Im;
    Stats s;
    Mat H;
    if(I.channels()>1){
        s.histo_1D(I, channel, 0);
        Mat t[3];
        split(I, t);
        Im=t[channel];

    }
    else s.histo_1D( I, 0, 0);
    H=s.H1D;

    std::vector<float> centroids_t0(K), centroids_t1(K);
    std::vector<int> nb(K);

    for(int k=0; k<K; k++) centroids_t0[k]=(k+0.5)*q;       //centres initiaux des K classes
    int iter=0;

    do{
        for(int k=0; k<K; k++){
          centroids_t1[k]=0;
          nb[k]=0;
        }

        /*===============A COMPLETER =============================*/
        // Attribution de chaque pixel a une classe      
        for (int i = 0; i < Im.rows; i++) {
            for (int j = 0; j < Im.cols; j++) {
                float val = Im.at<uchar>(i, j);
                float dmin = 256;
                int kmin = 0;

                // Trouver le centre de cluster le plus proche
                for (int k = 0; k < K; k++) {
                    float d = fabs(val - centroids_t0[k]);
                    if (d < dmin) {
                        dmin = d;
                        kmin = k;
                    }
                }

                // Mise à jour du centroid et du nombre de pixels pour le cluster kmin
                centroids_t1[kmin] += val;
                nb[kmin]++;
            }
        }

        // Calcul des nouveaux centroids en prenant la moyenne des pixels assignés
        error = 0;
        for (int k = 0; k < K; k++) {
            if (nb[k] > 0) {
                centroids_t1[k] /= nb[k];
            }

            // Calcul de l'erreur comme la somme des différences entre les centroids de l'itération précédente et actuelle
            error += fabs(centroids_t1[k] - centroids_t0[k]);
        }

        // Copier les nouveaux centroids pour la prochaine itération
        centroids_t0 = centroids_t1;

        iter++;


    /*===============FIN A COMPLETER =============================*/

        iter++;
    }while((iter<100) && (error>0));


    for (int i=0; i<Im.rows; i++)
        for (int j=0; j<Im.cols; j++){
            float val =Im.at<uchar>(i,j);
            float dmin=256;
            int kmin=0;
            for(int k=0; k<K; k++) {
                float d= fabs(val-centroids_t0[k]);
                if(d<dmin) {
                   dmin=d;
                   kmin=k;
                 }
                }
            Im.at<uchar>(i,j)=kmin;
            }

    return(Im);

    }






/*--------------------------------
Compute the color statistics of labels
im : color image CV_8UC3
labels : labels from a clustering
returns the image of label
--------------------------------*/
void Clustering::colorstats(const Configuration& cfg, const Mat& im, const Mat& labels)
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

/*--------------------------------
Select the label which is the closest (in terms of color) to a reference color
Labels : images of the labeks
color : color value
n : nb channels
returns the image of label
--------------------------------*/
Mat Clustering::select_label(const Mat& labels, float color[]) const
{
    Mat out(labels);
    int lmin=0;
    int K =stats.rows;
    float dmin= 3* UCHAR_MAX;

    for(int l=1; l<K; l++){
        float d1 = stats.at<double>(l, 1)  - color[0];
        float d2 = stats.at<double>(l, 2) - color[1];
        float d3 = stats.at<double>(l, 3) - color[2];
        float d = sqrt((d1*d1+d2*d2+d3*d3)/3);

        if (d<dmin) {
            lmin=l;
            dmin=d;
            }

        }
    for (int i= 0; i< labels.rows; i++)
        for (int j= 0; j< labels.cols; j++){
            if(( int)labels.at<uchar>(i,j)==lmin) out.at<uchar>(i,j) = 1;
             else out.at<uchar>(i,j) =0;
        }

    return(out);


    }

/*--------------------------------
Select the label which is the closest (in terms of color) to color[]
Labels : images of the labeks
centers : centers of clusters
color : color value
n : nb channels
returns the image of label
--------------------------------*/
Mat Clustering::select_label(const Mat& labels, Mat &centers, float color[], int n) const
{
    Mat out(labels);
    int kmin=0;

    int clusterCount =centers.rows;
    if(n==2){
        float x1 = centers.at<Vec2f>(0).val[0];
        float y1 = centers.at<Vec2f>(0).val[1];
        float x2 = color[0];
        float y2 = color[1];

        float dmin = DE2(x1,y1,x2,y2);


        for(int k=1; k<centers.rows; k++){
            x1 = centers.at<Vec2f>(k).val[0];
            y1 = centers.at<Vec2f>(k).val[1];

            float d = DE2(x1,y1,x2,y2);
             if (d<dmin) {
                kmin=k;
                dmin=d;
            }

        }

    }
    else{
        float x1 = centers.at<float>(0);
        float x2 = color[0];
        float dmin=abs(x1- x2);

        for(int k=1; k<centers.rows; k++){
            x1 = centers.at<float>(k);
            float d=abs(x1 -x2);
            if (d<dmin) {
                kmin=k;
                dmin=d;
                }
        }

    }

     kmin=   ( kmin+1)*255/clusterCount;
     for (int i= 0; i< labels.rows; i++)
         for (int j= 0; j< labels.cols; j++){
             if(( int)labels.at<uchar>(i,j)==kmin) out.at<uchar>(i,j) = 1;
             else out.at<uchar>(i,j) =0;
                    }


    return(out);

}

/*--------------------------------
Simple thresholding : puts 1 when t1 <= I <t2
returns a binary image
--------------------------------*/
Mat Clustering::threshold(Mat I, int t1, int t2)
{
    Mat res(I.rows, I.cols, CV_8U);
    for(int i=0; i< I.rows; i++)
        for(int j=0; j< I.cols; j++) {
            if ( (I.at<uchar>(i,j)>=t1) && (I.at<uchar>(i,j)<t2)) res.at<uchar>(i,j)=255;
            else res.at<uchar>(i,j)=0;

        }
        return(res);
}
