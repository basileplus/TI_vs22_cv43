#include "init.h"
using namespace cv;
using namespace std;

void Display(string s, Mat &I);
void Display(vector<int> v, string title);
void DisplayFalseColors(string s, Mat I );
void Save(string s, Mat &I);
void Zeros(Mat &I);

Mat LocalMax(Mat &I, int W);

Mat ConvertMat2CV_8U(Mat I);
void Display(vector<int> v, string title);
float vector_var(vector<int> v);
float vector_moy(vector<int> v);
int vector_max(vector<int> v, int &imax);
int vector_min(vector<int> v, int &imin);
int vector_max(vector<int> v);
vector<Point2d> vector_localmax(vector<int> v, float seuil);
Mat select_val(Mat I, unsigned char val);
