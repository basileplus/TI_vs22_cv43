#include "Shape.h"

Shape::Shape(const Segmentation& s)
{
    t_center.create(s.nr+1, 2, CV_32S);
    t_hu.create(s.nr+1, 7, CV_64F);
    t_limits.create(s.nr+1, 4, CV_64F); // row min et max, cols min et max
    t_area.resize(s.nr+1);
    t_mom.resize(s.nr+1);
    t_limits.create(s.nr+1, 4, CV_64F);
}
Shape::Shape()
{

}

Shape::~Shape()
{
    //dtor
}

/*************************************
Calcule plusieurs parametres de forme :
* extraits a partir des regions :
   - aire
   - limites du rectangle englobant
   - moments génométriques, de HU
   - barycentre
* extraits a partir des contours
   - perimetre
   - signature
**************************************/
void Shape::compute(const Segmentation& s)
{
    cout << "Parametres de forme " << endl;
    if(!s.Iregions.empty()) {
        Mat r(s.rows, s.cols, CV_8U);
        double  	hu[7] ;
        // processing of all regions
        for(int k=1; k<=s.nr; k++) {
            t_area[k]=0;//aire
            t_limits.at<double>(k,0)= s.rows; //row min
            t_limits.at<double>(k,1)= s.cols; // col min
            t_limits.at<double>(k,2)= 0; //row max
            t_limits.at<double>(k,3)= 0; //col max

            for(int i=0; i<s.rows; i++)
                for(int j=0; j<s.cols; j++){
                    r.at<uchar>(i,j)=0;
                    if(s.Iregions.at<int>(i,j)==k){
                        r.at<uchar>(i,j)=255;
                        //aire
                        t_area[k] ++;
                        //boite englobante
                        t_limits.at<double>(k,0)= testMin(i, t_limits.at<double>(k,0));
                        t_limits.at<double>(k,1)= testMin(j, t_limits.at<double>(k,1));
                        t_limits.at<double>(k,2)= testMax(i, t_limits.at<double>(k,2));
                        t_limits.at<double>(k,3)= testMax(j, t_limits.at<double>(k,3));

                       }
                        //else r.at<uchar>(i,j)=0;
            }
            //moments
            t_mom[k] = cv::moments(r);
            //centre d'inertie
            t_center.at<int>(k, 0) = t_mom[k].m10 / t_mom[k].m00;
            t_center.at<int>(k, 1) = t_mom[k].m01 / t_mom[k].m00;
            //moments de Hu
            HuMoments( t_mom[k],  hu);
            for(int i=0; i<8; i++) t_hu.at<double>(i)=hu[i];
            //test

//    }
}
r.release();
}
    if(!s.Icontours.empty()) {
        t_perim.resize(s.nr+1);
        for(int k=1; k<=s.nr; k++) t_perim[k]=0;
        Mat a(s.rows, s.cols, CV_8U);
         // processing of all regions
        for(int k=1; k<=s.nr; k++) {
            t_perim[k]=0;//aire
            for(int i=0; i<s.rows; i++)
                for(int j=0; j<s.cols; j++){
                a.at<uchar>(i,j)=0;
                    if(s.Icontours.at<int>(i,j)==k){
                        a.at<uchar>(i,j)=255;
                        t_perim[k] ++;
                      }
           }
        // calcul de la signature
        int xc = t_center.at<int>(k, 0);
        int yc = t_center.at<int>(k, 1);
        signature(a, Point2d(yc, xc));

    }
   cout << "Perimetres :" << endl;
   for(int k=1; k<=s.nr; k++)
    if(t_area[k] > 10) {
                cout << "--------------------------------------------------------" << endl;

                cout << "Region n° " << k << " de taille " << t_area[k] << " px" << " de perimetre " << t_perim[k] << endl;
                cout << " x =[" << t_limits.at<double>(k,0) <<", " << t_limits.at<double>(k,2) <<
                                "], y= [" << t_limits.at<double>(k,1) <<", " << t_limits.at<double>(k,3) << "] " <<   " xo,yo " << t_center.at<int>(k, 0) << " " << t_center.at<int>(k, 1) << endl;
                cout << "Moments centres :" << "mu02 = " << t_mom[k].mu20 <<   "; mu20 = " << t_mom[k].mu02 <<   "; mu11 = " << t_mom[k].mu11 <<  endl;
                cout << "mu02/mu20 = " << t_mom[k].mu20/ t_mom[k].mu02 <<  endl;
                cout << "--------------------------------------------------------" << endl;
                }


    a.release();
    }
    else cout << "ERROR : shape parameters can not be computed because the contours image does not exist " << endl;


//} else cout << "ERROR : shape parameters can not be computed because the regions do not exist " << endl;

}

/*************************************
Signature du contour (n'est valable que dans le cas de formes convexes pour lesquelles le barycentre est situe a l'interieur de la forme)
Modifie l'attribut t_sig : std::vector<vector<int>> t_sig;
Input :
- I : image binaire du contour
- Point2D : barycentre
- k : indice de la classe
**************************************/
void Shape::signature(const Mat& I, Point2d c )
{
    std::vector<int> s;
    std::vector<Point2d> L(I.rows*I.cols);
    Mat ToDo;
    I.copyTo(ToDo);
    // detection du premier point
    for(int i=0; i<I.rows; i++)
        for(int j=0; j<I.cols; j++){
            if(I.at<uchar>(i,j)!=0){
                Point2d p(i, j);
                L.push_back(p);
                ToDo.at<uchar>(i,j)=0;
               // cout << "i, j" << i <<" " << j << " "<< p.x << " " << p.y << endl;
                i=I.rows; // pour sortir de la boucle
                j=I.cols;

            }
        }

    do{
        Point2d p=L.back();
        L.pop_back();
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++){
                    if( i!=0 || j!=0)   {
                        int u= p.x+i;
                       int v= p.y+j;
                        if (((u>=0) && (u < I.rows )) && ((v >=0) && (v <I.cols)) ){
                            if((ToDo.at<uchar>(u, v)!=0)){
                                L.push_back(Point2d(u,v));
                                ToDo.at<uchar>(u, v)=0;
                                int d = (int) sqrt(pow((double) (u- c.x), 2) + pow((double) (v- c.y), 2));
                                s.push_back(d);
                            }

                        }
                    }
            }
        }while(!L.empty());

        t_sig.push_back(s);
       Display(s, "7-Signature ");

}



/**********************************************
Detection :
- rond
- triangle
- carré,
- rectangle
- hexagones
***********************************************/
#define N_MIN 50 // limite de taille
#define SEUIL_VAR 2
#define SEUIL_VAR2 25
void Shape::analysis ( )
{

     // largeur de l'affichage = size de v
    int n   =  t_sig.size();
    vector<int> v;
    float moy =0;
    float var =0;
    int imax;
    int  nrond=0;
    int ntri=0;

    int ncarre =0;
    int nrect=0;
    int nocto=0;

    vector<float> compacite(n);
    for(int i =0; i<n; i++){
        v = t_sig.back();
        t_sig.pop_back();

    }


cout << "---------------------"<< endl;
cout <<"RONDS : " << nrond << endl;
cout << "TRIANGLES : " << ntri <<  endl;
cout <<  "OCTOGONES : " << nocto << endl;
cout << "---------------------"<< endl;

}



/**********************************************
Hough Transform
I : binary image of contours
Ntheta, Nrho : bins of the Hough transform
***********************************************/

Mat Shape::HoughTransform (const Mat I, int Ntheta, int Nrho )
{
    int Rmax =sqrt(I.rows*I.rows+ I.cols*I.cols);
    Mat H=Mat::zeros(Ntheta, Nrho, CV_32F);
    //H.create(Ntheta, Nrho, CV_32F);
    //H.zeros(Ntheta, Nrho, CV_8U);
    int q_theta = 180/Ntheta;
    int q_rho = Rmax/Nrho;



   for(int i=0; i<I.rows; i++)
        for(int j=0; j<I.cols; j++)
            if(I.at<uchar>(i,j)!=0)
            for (int theta=0; theta<180; theta++)
            {
                int r= i*cos(theta*PI/180)+j*sin(theta*PI/180);
                int rr=r/q_rho;
                int tt=theta/q_theta;
                //cout << " rr " << rr <<  " tt " << tt << endl;
                if( (rr<Nrho) and (tt<Ntheta))
                H.at<float>(tt, rr)= H.at<float>(tt, rr)+1;

            }

 Save("Hough.png", H);
 Display("Hough.png", H);
 waitKey(0);
 return(H);
}

/**********************************************
Hough Transform Analysis
H : Hough matrix
Dtheta : theta min between 2 lines (%)
Drho : rho min between 2 lines (%)
Nmax : nb maxima (nb lines)
**********************************************/

void Shape::HoughTransformAnalysis (Mat& H, int Dtheta, int Drho, int Nmax)
{


   int tmax, rmax;
   int n=0;
   int s_theta = (H.rows*Dtheta)/100;
   int s_rho = (H.cols*Drho)/100;
   vector<Point2d> v(Nmax);

   do{
    int vmax=0;
    for(int t=0; t<H.rows; t++) //theta
        for(int r=0; r<H.cols; r++) //rho
        {
            if (H.at<float>(t,r)>vmax){
                vmax=H.at<float>(t,r);
                tmax=t;
                rmax=r;
            }
        }
        int t0=max(tmax-s_theta, 0);
        int t1=min(tmax+s_theta, H.cols);
        int r0=max(rmax-s_rho,0);
        int r1=min(rmax+s_rho, H.rows);

        for(int t=t0; t<t1; t++) //theta
           for(int r=r0; r<r1; r++) //rho
             H.at<float>(t,r) =0 ;

        v[n]= Point2d(tmax, rmax);

        n++;

        }while (n<Nmax);

for (int i=0; i< Nmax; i++)
cout << v[i].x << "   " << v[i].y << endl;
}

