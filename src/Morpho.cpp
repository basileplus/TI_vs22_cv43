#include "Morpho.h"

Morpho::Morpho()
{
    //ctor
}

Morpho::~Morpho()
{
    //dtor
}

/*---------------------------------------------------------
erode : binary erosion with a rectangular element of size 3x3. Builds the structuring element and calls the erosion function
INPUT :
- image src
- N: number of successive erosions
OUTPUT :
-eroded image
----------------------------------------------------------*/

Mat Morpho::erode(const Mat& src, int N)
{
    // r and c are the coordinates of the centroid and w is the width of the element
    int r=1;
    int c=1;
    int w=3;

    Mat es = getStructuringElement(MORPH_RECT , Size(w, w), Point(r, c) );
    Mat dst=erode(src, es, N);
    return (dst);

}
/*---------------------------------------------------------
erode : binary erosion.
INPUT :
- image src
- N: number of successive erosions
OUTPUT :
-eroded image
----------------------------------------------------------*/
Mat Morpho::erode(const Mat& src, const Mat&es, int N)
{
    int h= es.rows;
    int w= es.cols;
    int c= w/2;
    int r= h/2;
    Mat tmp=Mat::zeros(src.rows, src.cols, CV_8U);
    Mat dst=Mat::zeros(src.rows, src.cols, CV_8U);
    src.copyTo(tmp);

    for(int k=0; k<N; k++)
   {

    /*===============A COMPLETER =============================*/



    /*===============FIN A COMPLETER =============================*/

        dst.copyTo(tmp);
        }

    return (dst);

}

/*---------------------------------------------------------
dilate : binary dilation with a rectangular element of size 3x3. Builds the structuring element and calls the dilation function
INPUT :
- image src
- N: number of successive erosions
OUTPUT :
-dilated image
----------------------------------------------------------*/

Mat Morpho::dilate(const Mat& src, int N)
{
    // r and c are the coordinates of the centroid and w is the width of the element

    int r=1;
    int c=1;
    int w=3;
    int h=3;
    Mat es = getStructuringElement(MORPH_RECT , Size(w, h), Point(r, c) );
    Mat dst= dilate(src, es, N);
    return (dst);


}
/*---------------------------------------------------------
dilate : binary dilation.
INPUT :
- image src
- N: number of successive erosions
OUTPUT :
-dilated image
----------------------------------------------------------*/
Mat Morpho::dilate(const Mat& src, const Mat&es, int N)
{
    int h= es.rows;
    int w= es.cols;
    int c= w/2;
    int r= h/2;
    Mat dst=Mat::zeros(src.rows, src.cols, CV_8U);
    src.copyTo(dst);
    for(int k=0; k<N; k++){

/*===============A COMPLETER =============================*/



                /*===============A COMPLETER =============================*/

}
    return (dst);

}
/*---------------------------------------------------------
Binary closing : dilation and erosion
----------------------------------------------------------*/
Mat Morpho::closing(const Mat& src)
{
    Mat element = getStructuringElement(MORPH_RECT , Size(3, 3), Point(1, 1) );
    Mat dst=Mat::zeros(src.rows, src.cols, CV_8U);

    /*===============A COMPLETER =============================*/


    /*===============FIN A COMPLETER =============================*/

    return (dst);

}
/*---------------------------------------------------------
Binary opening : erosion and dilation
----------------------------------------------------------*/
Mat Morpho::opening(const Mat& src)
{
    Mat element = getStructuringElement(MORPH_RECT , Size(3, 3), Point(1, 1) );
    Mat dst=Mat::zeros(src.rows, src.cols, CV_8U);
    /*===============A COMPLETER =============================*/

    /*===============FIN A COMPLETER =============================*/

    return (dst);

}
/*---------------------------------------------------------
Binary morpho contour
----------------------------------------------------------*/
Mat Morpho::contours(const Mat& src)
{

    int r=1;
    int c=1;
    int w=3;
    Mat A;
    src.copyTo(A);
    Mat es = getStructuringElement(MORPH_RECT , Size(w, w), Point(r, c) );
    /*===============A COMPLETER =============================*/


    /*===============FIN A COMPLETER =============================*/

    return (A);

}
/*---------------------------------------------------------
geodesic reconstruction
----------------------------------------------------------*/
Mat Morpho::reconstruction(const Mat& src1, const Mat& src2)
{
        Mat A, B, C;
        int n =0;
        src1.copyTo(A);
        src1.copyTo(B);
        src1.copyTo(C);

        do{
    /*===============A COMPLETER =============================*/


    /*===============FIN A COMPLETER =============================*/

                }while (n>0);

        return(C);

}

/*---------------------------------------------------------
detection of the points corresponding to a passing through zero
----------------------------------------------------------*/

Mat Morpho::zeros(const Mat& src )
{
     Mat I, J; //.Mat::create(src.rows, src.cols, CV_8U);
     src.convertTo(I, (CV_8U), 1, 0);
     I.copyTo(J);
    /*===============A COMPLETER =============================*/

    /*===============FIN A COMPLETER =============================*/

     return(J);

}
