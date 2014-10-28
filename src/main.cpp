#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock hrc;


////////////////////////////////////////////////////////////////////////////////
int main(const int argc, const char* argv[])
{
    //  load and display image
    cv::Mat orig = cv::imread("../data/disparity.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    
    cv::imshow("window", orig);
    cv::waitKey();
    
    cv::Mat disp;
    orig.copyTo(disp);

    cv::Mat sharp;
    orig.copyTo(sharp);
    
    //  process
    const int width = disp.cols;
    const int height = disp.rows;
    
    hrc::time_point t0 = hrc::now();
    
    sharpenEdges(disp.data, width, height, sharp.data);
    fillLinear(sharp.data, width, height);
    cv::medianBlur(sharp, sharp, 5);

    cout << (float)(hrc::now() - t0).count() / 1e6 << endl;
    
    //  plot
    cv::Mat output;
    cv::hconcat(orig, sharp, output);
    cv::imshow("window", output);
    cv::waitKey();
    
    return 0;
}