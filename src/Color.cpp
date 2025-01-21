#include "Color.h"

Color::Color()
{
    //ctor
}
Color::Color(Configuration  cfg, int no_im, Mat &image, bool type)
{
   // cout << type << endl;
   Mat im;
  if (type==false) {
    imread_grayscale(cfg, no_im, im);
    color =0;
  }
  else {
    imread_rgb(cfg, no_im, im);
    im2C.create(im.cols, im.rows, CV_8UC2);

    color =1;
    }
  if(cfg.scale!=1)
      resize(im, image, Size(), 1./cfg.scale, 1./cfg.scale, INTER_LINEAR);
  else im.copyTo(image);

}

Color::~Color()
{
    //dtor
}
/**-------------------------------------------------------------
imread_grayscale
Opens an  image (color or grey) from the parameters of the configuration file and transforms it into gray
INPUT
- cfg : configuration
- no_im :number of image
INPUT-OUTPUT :
- image
OUTPUT :
- 0 when fails, 1 otherwise
--------------------------------------------------------------**/
int Color::imread_grayscale(Configuration  cfg, int no_im, Mat &image)
{
	ostringstream ss;
	ss << std::setfill('0') << std::setw(cfg.format) << no_im;
	string path = cfg.imdir + cfg.imname + ss.str() + cfg.imext;
	image = imread(path, cv::IMREAD_ANYDEPTH);
//	cout << cfg.imdir << endl;
//	cout << cfg.imname << endl;
//    cout << path << endl;

	if(!image.data )
	{
		cout << "Impossible de charger l'image " << path << endl ;
		return 0;
	}
	else {
        if(image.channels()>1){
            cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }

        if(image.depth() != CV_8U) {

           cout << "conversion" << endl;
            image.convertTo(image, CV_8U);
        }
	}
	return(1);
}

/**-------------------------------------------------------------
imread_rgb
Opens a color image from the parameters of the configuration file
INPUT
- cfg : configuration
- no_im :number of image
INPUT-OUTPUT :
- image
OUTPUT :
0 when fails, 1 otherwise
--------------------------------------------------------------**/

int Color::imread_rgb(Configuration  cfg, int no_im, Mat &image)
{
	ostringstream ss;
	ss << std::setfill('0') << std::setw(cfg.format) << no_im;
	string path = cfg.imdir + cfg.imname + ss.str() + cfg.imext;
//	cout << cfg.imdir << endl;
//	cout << cfg.imname << endl;
//	cout << ss.str()<< endl;
//		cout << cfg.imext<< endl;
//	cout << path << endl;
	path = cfg.imdir + cfg.imname + ss.str() + cfg.imext;
	image = imread(path, cv::IMREAD_COLOR);
	if(! image.data )
	{
		cout << "Impossible de charger l'image " << path << endl ;
		system("pause");
		return 0;
	}
	else return(1);
}

/**-------------------------------------------------------------
convert
Color conversion
INPUT
- cfg : configuration
- no_im :number of image
INPUT-OUTPUT :
- image
OUTPUT :
0 when fails, 1 otherwise
--------------------------------------------------------------**/
//comparaison donne 1 et non 0 avec fichier de configuration
Mat Color::convert(const Mat& in, const string& type, bool display)
{

    Mat out=Mat::zeros(in.rows, in.cols, CV_8UC3);
    //cout << type << "  " << type.compare("rgb") << endl;
    if(type.compare("RGB")==0) in.copyTo(out);
    else if(type.compare("rgb")==0) out= convert_rgb(in);
    else if(type.compare("HSV")==0) cvtColor(in, out, cv::COLOR_RGB2HSV);
    else if(type.compare("HLS")==0) cvtColor(in, out, cv::COLOR_RGB2HLS);
    else if(type.compare("XYZ")==0) cvtColor(in, out, cv::COLOR_RGB2XYZ);
    else if(type.compare("YUV")==0) cvtColor(in, out, cv::COLOR_RGB2YUV);
    else if(type.compare("YCbCr")==0) cvtColor(in, out, cv::COLOR_RGB2YCrCb);
    else if(type.compare("Lab")==0) cvtColor(in, out, cv::COLOR_RGB2Lab);
    else if(type.compare("gray")==0) {
        cvtColor(in, out, cv::COLOR_RGB2GRAY);
        color =false;
    }
    if(display) {
        namedWindow( type );
        imshow( type, out );
    }
    return(out);
}


/**-----------------------
Select Color component/channel
-------------------------**/
Mat Color::select_channel(const Mat& im, int channel)
{
    split(im, t_im);
    return(t_im[channel]);


}


/**-----------------------
Reshape : séparation des trois plans couleur --> t_im[]
Construction d'une image à 2 canneaux (pas utile)oo
display : affichage ou non
-------------------------**/
void Color::reshape(const Mat& im, bool display)
{

    split(im, t_im);
    if(color==1){

        cv::merge(t_im, 2, im2C);
    }
    if(display) {
        namedWindow( "channel 1" );
        imshow( "channel 1", t_im[0] );
        namedWindow( "channel 1" );
        imshow( "channel 2", t_im[1] );
        namedWindow( "channel 3" );
        imshow( "channel 3", t_im[2] );
    }

}
/**-------------------------------------------------------------
convert_rgb
Converts an image to the rgb model
--------------------------------------------------------------**/
Mat Color::convert_rgb(const Mat& im ) const
{
    Mat out=Mat::zeros(im.rows, im.cols, CV_8UC3);

    for (int i= 0; i< im.rows; i++)
            for (int j= 0; j< im.cols; j++){

                float R = (float)im.at<Vec3b>(i,j).val[0];
                float G = (float)im.at<Vec3b>(i,j).val[1];
                float B = (float)im.at<Vec3b>(i,j).val[2];
                float I = testMax((R+G+B), 0.01);
                float r =R/I;
                float g =G/I;
                float b =B/I;
                out.at<Vec3b>(i,j) = Vec3b((uchar)(r *255), (uchar)(g *255), (uchar)(b *255));
            }

return(out);
}
/* -------------------------------------------------------
Etirement
- It : src image
- Jt : dst image (after normalization)
---------------------------------------------------------*/
Mat Color::HistoStretching(Mat It)
{
	Mat Jt=Mat::zeros(It.rows, It.cols, CV_8U);
    /*===============A COMPLETER =============================*/
	// Etire l'histogramme de l'image It
	double minVal, maxVal;
	minMaxLoc(It, &minVal, &maxVal);
	It.convertTo(Jt, (CV_8U), 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

    /*===============FIN A COMPLETER =============================*/

	return(Jt);
}
/* -------------------------------------------------------
LogTransform
- It : src image
- Jt : dst image (after normalization)
---------------------------------------------------------*/
Mat Color::LogTransform(Mat It, float R)
{
    double minVal, maxVal;
	float c= 1/cv::log(1+R);
	Mat Jt=Mat::zeros(It.rows, It.cols, CV_32F);
    Mat out=Mat::zeros(It.rows, It.cols, CV_8U);
	It.convertTo(Jt, (CV_32F), 1,1);
	cv::log(Jt, Jt);
	Jt= Jt *c;
	minMaxLoc(Jt, &minVal, &maxVal);
	Jt.convertTo(out, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	return(out);
}

/* -------------------------------------------------------
ExpTransform
- It : src image
- Jt : dst image (after normalization)
---------------------------------------------------------*/
Mat Color::ExpTransform(Mat It, float R)
{
    double minVal, maxVal;
	//float c= 1/cv::exp(1+R);
	Mat Jt=Mat::zeros(It.rows, It.cols, CV_32F);
    Mat out=Mat::zeros(It.rows, It.cols, CV_8U);
	It.convertTo(Jt, (CV_32F), 1/255.,0);
	minMaxLoc(Jt, &minVal, &maxVal);
	//cout << "min " << minVal << " max : " << maxVal << endl;
	pow(Jt,R, Jt);

    minMaxLoc(Jt, &minVal, &maxVal);
	//cout << "min " << minVal << " max : " << maxVal << endl;
	minMaxLoc(Jt, &minVal, &maxVal);
	Jt.convertTo(out, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	return(out);
}

/* -------------------------------------------------------
Photometric normalization :
- It : src image
- Jt : dst image (after normalization)
---------------------------------------------------------*/
Mat Color::PhotometricNormalization(Mat It)
{
	Mat tmp=Mat::zeros(It.rows, It.cols, CV_32F);
	Mat Jt=Mat::zeros(It.rows, It.cols, CV_8U);

	int n;
	double it;
	double mu =0;
	double sigma=0;

	for(int i = 0; i < It.rows; i++)
	{
		for (int j = 0; j < It.cols; j++)
		{
			it		=	(double)It.at<uchar>(i,j);
			mu		+=	(double)it;
			sigma	+=	(double)it*it;
		}
	}
	n=It.rows*It.cols;
	mu=mu/n;
	sigma= sqrt(sigma/n - mu*mu);


	It.convertTo(tmp, (CV_32F));
	tmp= (tmp-mu );///sigma;


	double minVal, maxVal;
	minMaxLoc(tmp, &minVal, &maxVal);
	tmp.convertTo(Jt, (CV_8U), 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	return(Jt);
}


