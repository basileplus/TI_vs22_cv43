#include "Segmentation.h"

Segmentation::Segmentation()
{
    //ctor
}
Segmentation::Segmentation(const Mat& im)
{
    rows = im.rows;
    cols = im.cols;


}

Segmentation::~Segmentation()
{
    //dtor
    Iregions.release();
    if(!Icontours.empty()) Icontours.release();
}

/** --------------------------------
Segmentation : étiquetage en composantes connexes (connected component labeling)
par reconstruction geodesique
im : image binaire
-------------------------------- **/
Mat Segmentation::etiquetage_cc(const Mat& im)
{
   Morpho m;
    Mat I=Mat::zeros(im.rows, im.cols, CV_8U);
    Mat res=Mat::zeros(im.rows, im.cols, CV_8U);
    int n=0;
    for(int i=0; i< im.rows; i++)
        for(int j=0; j< im.cols; j++)
            if((im.at<uchar>(i,j)!=0) and (res.at<uchar>(i,j)==0)) {
                n++;
                I.at<uchar>(i,j)=1;
                res+=m.reconstruction(I, im);
                I.at<uchar>(i,j)=0;
                //break;
        }

    return(res);

}
/**--------------------------------
Segmentation : étiquetage en composantes connexes (connected component labeling)
par croissance de régions
utilisant une pile pour stocker les pixels à traiter
im : image binaire
--------------------------------**/
Mat Segmentation::regions(const Mat& im)
{
    Mat res=Mat::zeros(im.rows, im.cols, CV_32S);
    Mat ToDo=Mat::zeros(im.rows, im.cols, CV_8U) ;
    std::vector<Point2d> L ; //(im.rows*im.cols);

    int e=1;

    for (int i= 0; i< im.rows; i++)
        for (int j= 0; j< im.cols; j++){
        res.at<int>(i,j)=0;
        ToDo.at<uchar>(i,j)=1;
        }

    for (int i= 0; i< im.rows; i++)
        for (int j= 0; j< im.cols; j++){
            Point2d p(j,i);
            if(ToDo.at<uchar>(p)!=0){
                if(im.at<uchar>(p)!=0){
//                    // cout << "size : " << (int)L.size() << endl;
                    L.push_back(p);
                    ToDo.at<uchar>(p)=0;
                    res.at<int>(p)=e;

                    do{
                        Point2d q=L.back();
                        L.pop_back();
                       // cout << "size : " << L.size() << endl;
                           /*===============A COMPLETER =============================*/


                                /*===============FIN A COMPLETER =============================*/


                    }while(L.size()>0);

                    e++;
            }
    }
    }
    nr=e-1;

    cout << "Nb regions : " << e << endl;

    res.copyTo(Iregions);
    ToDo.release();

    cout << "Fin regions " << endl;
    return(res);

}
/**
select pixels of label k
**/
Mat Segmentation::select_reg(const Mat R, int k )
{
    Mat Rk=Mat::zeros(R.rows, R.cols, CV_8U);
    for(int i=0; i<R.rows; i++)
        for(int j=0; j<R.cols; j++){
            if(R.at<int>(i,j)==k)
                Rk.at<uchar>(i,j)=255;
            else Rk.at<uchar>(i,j)=0;
            }
return(Rk);
}
/**
finds the first pix of a contour in order to build it
**/
Point2d Segmentation::first_pix(const Mat I)
{
    Point2d p;
    int ok=0;
    int im=0, jm=0;
    for(int i=0; i<I.rows; i++)
        for(int j=0; j<I.cols; j++)
            if(( ( int)I.at<uchar>(i,j)!=0) and (ok==0)){
                p=Point(i,j);
                ok=1;
                //cout << "point " << i << "p "<< p.x << endl;
                    }
    return(p);

}


Mat Segmentation::add_region(Mat& R, Mat& I, int k)
{
    for(int i=0; i<R.rows; i++)
        for(int j=0; j<R.cols; j++)
        if(I.at<uchar>(i,j)!=0)
        R.at<int>(i,j)=k;

return(R);
}

/*--------------------------------
Segmentation : detection du contour englobant de regions
(permet de se passer des contours internes)
--------------------------------*/

Mat Segmentation::contours(Mat &I, int n)
{
    // image de la region k
    cout << "debut contours " << endl;
    Mat P=Mat::zeros(I.rows, I.cols, CV_8U);
    Icontours=Mat::zeros(I.rows, I.cols, CV_32S);

    //.create(I.rows, I.cols, CV_32S);

    Mat Rk=Mat::zeros(I.rows, I.cols, CV_8U);;
     Mat C =Mat::zeros(I.rows, I.cols, CV_32S);

    for(int k=1; k<= n; k++) {


            Rk=select_reg(Iregions,  k );

            Morpho m;
            C = m.contours(Rk);
            Point2d p= first_pix(Rk);
            P.at<uchar>(p.x, p.y)=255;
            Mat A= m.reconstruction(P, C);
            P.at<uchar>(p.x, p.y)=0;
            Icontours=add_region(Icontours, A, k);

             waitKey(10);

            }

    return(Icontours);
}
/*
Display regions in false colors
*/
void Segmentation::Display(string s, Mat I )
{
	Mat C;
	C=Mat::zeros(I.rows, I.cols, CV_8UC3);
	int v=255;
	std::vector<Vec3b> label(nr+1);
	for(int k=1; k<=nr; k++)   label[k]=Vec3b(rand() % 255, rand() % 255,rand() % 255 );

	for(int i=0; i<I.rows; i++){
		for(int j=0; j<I.cols; j++){
            int k= I.at<int>(i,j);
            if ( (k>0) && (k<=nr) ) C.at<Vec3b>(i,j)=  label[k];
		}
	}
	imshow(s, C);

}


