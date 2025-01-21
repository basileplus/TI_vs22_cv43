
#include "init.h"
#include "tools.h"

/* ------------------------------------------------------
Display with a conversion to CV_8U and normalization between 0 and 255
 -------------------------------------------------------*/
void Display(string s, Mat &I)
{
    Mat res(I.rows, I.cols, CV_8U);
    Zeros(res);
    double minVal, maxVal;
    minMaxLoc(I, &minVal, &maxVal);
    if(maxVal!=minVal) I.convertTo(res, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    else I.convertTo(res, (CV_8U), 1, 0);
    imshow(s, res);
}

/* ------------------------------------------------------
Display in False colors
 -------------------------------------------------------*/
void DisplayFalseColors(string s, Mat I )
{
	Mat C;
	C.create(I.rows, I.cols, CV_8UC3);
	int v=255;
	for(int i=0; i<I.rows; i++){
		for(int j=0; j<I.cols; j++){
            C.at<Vec3b>(i,j)[0]=  ((v- (int)I.at<int>(i,j))*255/v)%255;
			C.at<Vec3b>(i,j)[1]=  ((v/2-abs(v/2-(int)I.at<int>(i,j)))*510/v)%255;
			C.at<Vec3b>(i,j)[2]=  ((int)I.at<int>(i,j)*255/v)%255 ;
		}
	}
    imshow(s, C);
}
/* ------------------------------------------------------
Display a vector
 -------------------------------------------------------*/
#define PTS_MIN 100
void Display(vector<int> v, string title)
{
    // largeur de l'affichage = size de v
    int n   =  v.size();

    if(n> PTS_MIN) {

        // hauteur de l'affichage = max de v
        int m   =  v[0];
        for(int i= 1; i< n; i++) if (v[i]>m) m=v[i];

        Mat I;
        I.create(m, n, CV_8U);
        Zeros(I);

        for( int i = 0; i < n-1; i++ ) {
            Point2d p1(i, m-v[i]);
            Point2d p2(i+1, m-v[i+1]);
            line( I, p1, p2, Scalar( 255, 0, 0), 2, 8, 0  );
        }

        imshow(title, I);
        waitKey(1);
        }


}
/* ------------------------------------------------------
Save an image after normalization between 0 and 255
 -------------------------------------------------------*/
void Save(string s, Mat &I)
{
    Mat res=Mat::zeros(I.rows, I.cols, CV_8U);
    double minVal, maxVal;
    minMaxLoc(I, &minVal, &maxVal);
    I.convertTo(res, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    imwrite(s, res);
}
/* ------------------------------------------------------
Init a Mat with zeros
 -------------------------------------------------------*/
void Zeros(Mat &I )
{
switch(I.type()){
    case CV_8U :
        for (int i=0; i<I.rows; i++)
            for (int j=0; j<I.cols; j++){
                I.at<uchar>(i,j)=0;
            }
        break;
    case CV_32F :
        for (int i=0; i<I.rows; i++)
            for (int j=0; j<I.cols; j++){
                I.at<float>(i,j)=0;
            }
        break;
    case CV_8UC3 :
        for (int i=0; i<I.rows; i++)
            for (int j=0; j<I.cols; j++){
                I.at<Vec3b>(i,j)[0]=I.at<Vec3b>(i,j)[1]=I.at<Vec3b>(i,j)[2]=0;
            }
        break;
    default : cout << "warning : no initialisation with Zeros" << endl;
    break;
    }

}
/* ------------------------------------------------------
Finds local max in a Mat
 -------------------------------------------------------*/
// research of local maxima
Mat LocalMax(Mat &I, int W)
{
    Mat J;
    J.create(I.rows, I.cols, CV_8U);

    Zeros(J);

    int w= W/2;
    if(I.type()==CV_8U){

    for (int i=w; i<I.rows-w; i++)
        for (int j=w; j<I.cols-w; j++){
            uchar val=255;
            for (int u=-w; u<=w; u++)
                for (int v=-w; v<=w; v++)
                    if(I.at<uchar>(i,j)<I.at<uchar>(i+u,j+v)) val=0;

            J.at<uchar>(i,j)= val;
    }
    }
    else if (I.type()==CV_32F){
    cout << "float" << endl;
    for (int i=w; i<I.rows-w; i++)
        for (int j=w; j<I.cols-w; j++){
            uchar val=255;
            for (int u=-w; u<=w; u++)
                for (int v=-w; v<=w; v++)
                    if(I.at<float>(i,j)<I.at<float>(i+u,j+v)) val=0;

            J.at<uchar>(i,j)= val;
    }


    }
    return(J);
}

/*-----------------------------------------------
Concersion of a Mat to CV_8U Mat (useful for display purposes)
------------------------------------------------*/
Mat ConvertMat2CV_8U(Mat I)
{
	Mat J;
	J.create(I.rows, I.cols, CV_8U);
	double minVal, maxVal;
	minMaxLoc(I, &minVal, &maxVal);
	I.convertTo(J, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	return(J);
}

/*-----------------------------------------------
vector var: returns the std dev value of v
------------------------------------------------*/
float vector_var(vector<int> v)
{
	float moy =0.;
	float var =0.;
	for(int i=0; i< v.size(); i++){
                moy+=v[i];
                var+=v[i]*v[i];
             }
            moy/=v.size();
            var=sqrt(var/v.size() - moy*moy);
	return(var);
}

/*-----------------------------------------------
vector_max: returns the maximum value of v and computes the argmax imax
------------------------------------------------*/
int vector_max(vector<int> v, int &imax){

    int vmax =v[0];
    imax=0;
    for(int i=1; i< v.size(); i++)
        if(v[i]>vmax) {
            vmax=v[i];
            imax=i;
            }

	return(vmax);

}
/*-----------------------------------------------
vector_min : returns the minimum value of v and computes the argmin imin
------------------------------------------------*/

int vector_min(vector<int> v, int &imin){

    int vmin =v[0];
    imin=0;
    for(int i=1; i< v.size(); i++)
        if(v[i]<vmin) {
        vmin=v[i];
        imin=i;
        }

	return(vmin);

}

/*-----------------------------------------------
vector moy: returns the mean value of v
------------------------------------------------*/
float vector_moy(vector<int> v)
{
	float moy =0.;

    for(int i=0; i< v.size(); i++)        moy+=v[i];
	moy/=v.size();
	return(moy);
}


/** ******************************************
vector_localmax :
Recherche de maxima locaux :
- detection des plages de points de valeur > seuil x val max
- Les donnees sont decalees pour commencer par la valeur minimale de facon a ce que les maxima ne se situent pas a cheval sur le debut et la fin du vecteur

***********************************************/
vector<Point2d> vector_localmax(vector<int> v, float seuil )
{
    /** recopie des valeurs pour la recherche sur les bords **/
    vector<int> t(v.size());
    vector<Point2d> m;
    int imax, tmax, im;
    // the vector is shift so that it starts with the minimum value
    int d;
    int vmin= vector_min(v, d);
    int vmax= vector_max(v, im);
    for(int i=d; i< v.size(); i++) t[i-d]=v[i];
    for(int i=0; i< d; i++) t[v.size()-d+i]=v[i];

    for(int i=1; i< t.size(); i++){
    //cout << t[i] << endl;
    //detecte si on est sur un max
        if(  t[i]> seuil*vmax){
            bool audessus;
            tmax=t[i];
            //tant que l'on est au dessus du seuil on cherche le max
            do{
                i++;
                audessus=false;
                if(i<t.size()-1)
                    if(t[i]>seuil*vmax)
                    {
                        audessus=true;
                        if(t[i]>tmax) {
                            tmax=t[i];
                            imax=i;
                            }
                    }

            }while(audessus==true);
            m.push_back(Point2d(imax+d, tmax));
            //cout << "tmax " << tmax << " taille " << m.size() << endl;
            }

            }


    return(m);

}

/*-----------------------------------------------
Select the val in the image and returns the corresponding binary image
------------------------------------------------*/
Mat select_val(Mat I, unsigned char val)
{
    Mat J(I.rows, I.cols, CV_8U);
    for(int i=0; i<I.rows; i++)
        for(int j=0; j<I.cols; j++){
            if(I.at<uchar>(i,j)==val)
                J.at<uchar>(i,j)=255;
            else   J.at<uchar>(i,j)=0;

    }
    return(J);

}
