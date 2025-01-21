
/****************************************************************************
*  Motion :
*
*
*  Author : Michèle Gouiffès 12/02/2018

****************************************************************************/

#include "init.h"
#include "Motion.h"

using namespace cv;
using namespace std;

Motion::Motion()
{
	bs_alpha = 0.7;
	bs_th = 150;
	hs_itermax=2;
	hs_alpha=1;

	simi_norm_th=0;
	simi_angle_th=0;
    motion_threshold=255;

}
Motion::Motion(Configuration   config)
{
	config.Get("bs_alpha", bs_alpha);
	config.Get("bs_th", bs_th);

	config.Get("hs_itermax", hs_itermax);
	config.Get("hs_alpha", hs_alpha);

	config.Get("simi_angle_th", simi_angle_th);
	config.Get("simi_norm_th", simi_norm_th);
    config.Get("motion_threshold", motion_threshold);
}


/* -------------------------------------------------------
Simple Background Subraction  :
- Bt : background image
- It : current image
- filter = 0(resp. 1) without (resp with) morphological filtering
---------------------------------------------------------*/
Mat Motion::SimpleBackgroundSubtract(Mat Bt, Mat It, bool filter)
{
	Mat dIt;
	It.copyTo(dIt);

	/* a completer
	*/
	// mathematical morphology when filter = TRUE
	if( filter){
		/* a completer*/
	}
	return(dIt);
}


/* -------------------------------------------------------
Background update using a temporal mean
- Bt : image of the current background
- It : current image of the video
---------------------------------------------------------*/
void Motion::BackgroundUpdateMean(Mat &Bt, Mat It)
{
	/* A completer */
	}

/* -------------------------------------------------------
Background update using the median image of 3  frames

- I1 to I3 : 3 frames used for computation of the median
- returns B : image of the current background
---------------------------------------------------------*/
Mat Motion::BackgroundUpdateMedian(Mat I1, Mat I2, Mat I3)
{
	Mat B(I1.rows, I1.cols, CV_8U);
	B.zeros(I1.rows, I1.cols, CV_8U);
	//table of the values before sorting
	Vec<int, 3> tab;
	//table of the values after sorting
	Vec<int, 3> tab_sorted;

	for(int i = 0; i < I1.rows; i++)
	{
		for (int  j = 0; j < I1.cols; j++)
		{
			tab(0)	= (int) I1.at<uchar>(i,j);
			tab(1)	= (int) I2.at<uchar>(i,j);
			tab(2)	= (int) I3.at<uchar>(i,j);

			cv::sort(tab, tab_sorted, cv::SORT_ASCENDING);
			B.at<uchar>(i,j)	=  tab(1);
		}
	}
	return(B);
}

/* -------------------------------------------------------
DrawMotion :  (for display purpose)
- It_1 : previous image
- It : current image
- a : amplification of the motion for display
- U, V : velocity x, y
- vectors (output): image of the motion vectors
---------------------------------------------------------*/
Mat Motion::DrawMotion(Mat It_1, Mat It, int a, Mat U, Mat V)
{
	int nrows = It_1.rows;
	int ncols = It_1.cols;

	Mat vectors;
	cvtColor(It,vectors,cv::COLOR_GRAY2RGB);
	vectors.zeros(nrows, ncols, CV_8UC3);

	int w=11;

	Point p1;
	Point p2;
	for(int i=0; i<nrows-w; i+=w)
	{
		for(int j=0; j<ncols-w; j+=w)
		{
			double mu_u=0;
			double mu_v=0;
			for(int k=0; k<w; k++)
			{
				for(int l=0; l<w; l++)
				{
					mu_u+= ((float)(U.at<float>(i+k,j+l)));
					mu_v+= ((float)(V.at<float>(i+k,j+l)));
				}
			}

			p1.x=j;
			p1.y=i;
			p2.x=j;
			p2.y=i;

			double u=mu_u/(w*w);
			double v=mu_v/(w*w);
			if(  (i+u >= 0) && (i+u < It_1.rows) &&
				(j+v>= 0) && (j+v < It_1.cols) )
				{
					p2.x=(int)(j+a*u);
					p2.y=(int)(i+a*v);
				}

			circle(vectors, p1, 1, CV_RGB(255,255,0),1, 8, 0);
			line(vectors, p2, p1, CV_RGB(255,255,0),1,8,0);

		}

	}
	return(vectors);

}

/* -------------------------------------------------------
Optical Flow Horn et Schunck
- It_1 : previous image (uchar)
- It : current image	(uchar)
- U,  V : velocity		(float)
- alpha : regularization parameter
---------------------------------------------------------*/
void Motion::HornSchunck(Mat It_1, Mat It, Mat &U, Mat &V)
{
	int nrows = It_1.rows;
	int ncols = It_1.cols;

	Mat Gx(nrows, ncols, CV_32F);
	Mat Gy(nrows, ncols, CV_32F);
	Mat Gt(nrows, ncols, CV_32F);

    Filtering f;
	f.gradients_HS(It_1, It, Gx, Gy, Gt);

	U.create(nrows, ncols, CV_32F);
	V.create(nrows, ncols, CV_32F);
	U.zeros(nrows, ncols, CV_32F);
	V.zeros(nrows, ncols, CV_32F);

	float u, v, gx, gy, gt;

	//iterations for minimization
	for(int k=0; k<hs_itermax; k++)
	{

	/* a completer*/



		//smoothing of (u,v) to prepare the next iteration
		int W=2;
		Size s(2*W+1, 2*W+1);
		GaussianBlur(U, U, s, W, W, BORDER_DEFAULT );
		GaussianBlur(V, V, s, W, W, BORDER_DEFAULT );
	}
}


/* -------------------------------------------------------
Convert velocity value in Norm+angle
- U, V velocities in x and y
- Norms, Angles : norm and angle
---------------------------------------------------------*/
void Motion::MotionNormAngle(Mat U, Mat V, Mat &Norm, Mat &Angle)
{
	int nrows=U.rows;
	int ncols=V.cols;
	Norm.create(nrows, ncols, CV_32F);
	Angle.create(nrows, ncols, CV_32F);
	double u,v, n, a;
	for(int i=0; i<nrows-1; i++){
			for(int j=0; j<ncols-1; j++){

				u=(double)U.at<float>(i,j);
				v=(double)V.at<float>(i,j);

				n= sqrt((double)u*u+v*v)/sqrt((double)2);

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
				//normalization
				a*=255.0/(2*PI);

				Norm.at<float>(i,j)= (float) (n);
				Angle.at<float>(i,j)= (float) (a);
			}
	}

}

/* -------------------------------------------------------
Similarity
- Norm_p, Norm_q : velocity norm at neighboring pixels p and q
- Angle_p, Angle_q : velocity orientations t neighboring pixels p and q

returns 1 if both norms and angles of velocity are similar (<threshold)
---------------------------------------------------------*/
bool Motion::Similarity(float Norm_p, float Norm_q, float Angle_p, float Angle_q)
{

	bool crit_n= (fabs(Norm_p-  Norm_q) <simi_norm_th);
  float diff_angle = fabs(Angle_p-  Angle_q);
  //cout << "diff angle = " << diff_angle << endl;
  if(diff_angle>128) diff_angle-=128;
	bool crit_a =(diff_angle <simi_angle_th);
	return((crit_n&&crit_a));

}


/* ------------------------------------------------------------------------------ */
int Motion::grow_velocity(Mat Vn, Mat Va, int It_1, int j0,  Mat &ToDo, Mat &R)
/* ------------------------------------------------------------------------------ */
{
	Point *P;
	Point p, q;
	Mat InList;
	Vec2b Valp, Valq;
	int x,y;
	float moy_n=0, moy_a=0;
	int n=0;

	P=new Point[Vn.rows*Vn.cols];
	//random label (color) for display
	uchar r = (unsigned char) (rand()%255);
	uchar g = (unsigned char) (rand()%255);
	uchar b = (unsigned char)  (rand()%255);

	InList.create(Vn.rows, Vn.cols, CV_8U);
	for(int i=0; i< Vn.rows; i++)	{
		for(int j=0; j< Vn.cols; j++) {
			InList.at<uchar>(i,j) =0;
		}
	}

	int u=0;
	P[u]=Point(It_1,j0);

	while (u>=0)
	{
		p=P[u];
		u--;
		x=p.x; y=p.y;

		if(ToDo.at<uchar>(x,y)!=0)
		{
			//label the pixel
			R.at<Vec3b>(x,y)[0]=(unsigned char) r;
			R.at<Vec3b>(x,y)[1]=(unsigned char) g;
			R.at<Vec3b>(x,y)[2]=(unsigned char) b;
			//computation of the mean velocity of the region
			n++;

			moy_n+=Vn.at<float>(x,y);
			moy_a+=Va.at<float>(x,y);
			//the pixel will not be processed anymore
			ToDo.at<uchar>(x,y)=0;

			for (int i=x-1; i<=x+1; i++)
				for (int j=y-1; j<=y+1; j++)
					if( (i>=0) && (i<Vn.rows) && (j>=0) && (j<Vn.cols))
					{
						q.x=i;
						q.y=j;
						if((InList.at<uchar>(i,j)==0) && (ToDo.at<uchar>(i,j)==1))
						{
							if(((float)Vn.at<float>(i,j)> (float)motion_threshold)){

									bool crit=Similarity((float)Vn.at<float>(i,j), (float)Vn.at<float>(x,y),
										(float)Va.at<float>(i,j), (float)Va.at<float>(x,y));

									if(crit){
										u++;
										P[u]= Point(i,j);
										InList.at<uchar>(i,j)=1;

							}
							}
						}
					}
		}
	}

	delete[] P;
	return(n);
	}

/* ------------------------------------------------------------------------------ */
void Motion::MotionSegment(Mat Vn, Mat Va, Mat &R)
/* ------------------------------------------------------------------------------ */
{
	int n=0;
	Mat ToDo;
	ToDo.create(Vn.rows, Vn.cols, CV_8U);
	Mat Un, Ua;
	Un.create(Vn.rows, Vn.cols, CV_32F);
	Ua.create(Va.rows, Va.cols, CV_32F);

	Vn.convertTo(Un, (CV_32F), 25, 0);
	Va.convertTo(Ua, (CV_32F), 1, 0);

	R.create(Vn.rows, Vn.cols, CV_8UC3);

	for(int i=0; i< Vn.rows; i++)
	{
		for(int j=0; j< Vn.cols; j++)
		{
			ToDo.at<uchar>(i,j) =1;
			R.at<Vec3b>(i,j)[0]=0;
			R.at<Vec3b>(i,j)[1]=0;
			R.at<Vec3b>(i,j)[2]=0;
		}
	}

	for(int i=0; i< Vn.rows; i++)
	{
		for(int j=0; j< Vn.cols; j++)
		{
			if(ToDo.at<uchar>(i,j)!=0){

				if (((float)Un.at<float>(i,j))>motion_threshold)	// threshold to detect motion
				{
					grow_velocity(Un, Ua, i, j, ToDo, R);

				}
			}
		}
	}

}



