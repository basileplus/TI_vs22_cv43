#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "init.h"

class Configuration;
class Frequency
{
    public:
        Frequency();
        Frequency(Mat I);
        Frequency(Mat I, Configuration cfg);
        Mat fft_z(Mat I);
        Mat fft(Mat I);
        Mat ifft(Mat complexI);
        Mat fftshift(Mat imF);
        Mat GaussLowPass(Mat imFFT0, float sigma_c, float sigma_r);
        Mat GaussHighPass(Mat imFFT0, float sigma_c, float sigma_r);
        Mat GaussBandPass(Mat imFFT0, float sigma_c, float sigma_r, float f0c, float f0r);
        Mat masking(Mat I, Mat M);
        void displayTFD( string s);

        virtual ~Frequency();
        Mat FFT_spectrum(Mat I);

        int W;
                int    DFT_rows, DFT_cols ;
        Mat imTFD[2];

    protected:

    private:
};

#endif // Frequency_H
