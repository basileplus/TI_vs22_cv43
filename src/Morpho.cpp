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
Mat Morpho::erode(const Mat& src, const Mat& es, int N)
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
        for (int i = r; i < src.rows - r; i++) // Parcours des pixels de l'image
        {
            for (int j = c; j < src.cols - c; j++)
            {
                bool erodePixel = true;

                // Parcourir l'élément structurant
                for (int y = -r; y <= r; y++)
                {
                    for (int x = -c; x <= c; x++)
                    {
                        // Vérifier si l'élément structurant est entièrement contenu
                        if (es.at<uchar>(y + r, x + c) != 0 &&
                            tmp.at<uchar>(i + y, j + x) == 0)
                        {
                            erodePixel = false;
                            break;
                        }
                    }
                    if (!erodePixel) break;
                }

                // Mettre à jour le pixel dans la matrice destination
                dst.at<uchar>(i, j) = erodePixel ? 255 : 0;
            }
        }


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
    Mat tmp = Mat::zeros(src.rows, src.cols, CV_8U); // Matrice temporaire
    src.copyTo(tmp);
    for(int k=0; k<N; k++){
        /*===============A COMPLETER =============================*/
        for (int i = r; i < src.rows - r; i++) // Parcours des pixels de l'image
        {
            for (int j = c; j < src.cols - c; j++)
            {
                bool dilatePixel = false;

                // Parcourir l'élément structurant
                for (int y = -r; y <= r; y++)
                {
                    for (int x = -c; x <= c; x++)
                    {
                        // Vérifier si un pixel couvert par l'élément structurant est blanc
                        if (es.at<uchar>(y + r, x + c) != 0 &&
                            tmp.at<uchar>(i + y, j + x) != 0)
                        {
                            dilatePixel = true;
                            break;
                        }
                    }
                    if (dilatePixel) break;
                }

                // Mettre à jour le pixel dans la matrice destination
                dst.at<uchar>(i, j) = dilatePixel ? 255 : 0;
            }
        }
        // Copier le résultat de cette itération dans tmp pour la prochaine
        dst.copyTo(tmp);
        /*===============A COMPLETER =============================*/
}
    return (dst);

}
/*---------------------------------------------------------
Binary closing : dilation and erosion
----------------------------------------------------------*/
Mat Morpho::closing(const Mat& src)
{
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
    Mat tmp = dilate(src, element, 1); // Appliquer une dilatation
    Mat dst = erode(tmp, element, 1); // Appliquer une érosion
    return dst;
    return (dst);
}
/*---------------------------------------------------------
Binary opening : erosion and dilation
----------------------------------------------------------*/
Mat Morpho::opening(const Mat& src)
{
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
    Mat tmp = erode(src, element, 1); // Appliquer une érosion
    Mat dst = dilate(tmp, element, 1); // Appliquer une dilatation
    return dst;
}
/*---------------------------------------------------------
Binary morpho contour
----------------------------------------------------------*/
Mat Morpho::contours(const Mat& src)
{
    int w = 3;
    Mat es = getStructuringElement(MORPH_RECT, Size(w, w), Point(1, 1));
    Mat eroded = erode(src, es, 1); // Appliquer une érosion
    Mat A = src - eroded; // Calculer la différence pour les contours
    return A;
}
/*---------------------------------------------------------
geodesic reconstruction
----------------------------------------------------------*/
Mat Morpho::reconstruction(const Mat& marker, const Mat& mask)
{
    Mat A, B;
    marker.copyTo(A);
    marker.copyTo(B);
    int n = 1;

    do {
        A.copyTo(B); // Copier l'état précédent
        A = dilate(A, getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1)), 1);
        A = min(A, mask); // Contraindre par le masque
        n = countNonZero(A != B); // Vérifier convergence
    } while (n > 0);

    return A;
}

/*---------------------------------------------------------
detection of the points corresponding to a passing through zero
----------------------------------------------------------*/

Mat Morpho::zeros(const Mat& src)
{
    Mat I, J;
    src.convertTo(I, CV_32F); // Convertir en float pour manipuler les valeurs
    Mat shifted;
    J = Mat::zeros(I.rows, I.cols, CV_8U);

    for (int i = 1; i < I.rows - 1; i++)
    {
        for (int j = 1; j < I.cols - 1; j++)
        {
            // Vérifier un passage de signe dans la zone locale
            float val = I.at<float>(i, j);
            bool sign_change = (val * I.at<float>(i + 1, j) < 0) ||
                (val * I.at<float>(i, j + 1) < 0);
            J.at<uchar>(i, j) = sign_change ? 255 : 0; // Marquer les passages
        }
    }
    return J;
}
