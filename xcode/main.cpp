#include <iostream>
#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"

using namespace std;



////////////////////////////////////////////////////////////////////////////////
int main(const int argc, const char* argv[])
{
    //  load and display image
    cv::Mat orig = cv::imread("../data/disparity.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    
    cv::imshow("window", orig);
    cv::waitKey();
    
    cv::Mat disp;
    orig.copyTo(disp);
    
    //  process
    const int width = disp.cols;
    const int height = disp.rows;
    
     fillLinear(disp.data, width, height, 1, 0);
    // layerFloodFill(data, width, height);
    
    //  plot
    cv::Mat output;
    cv::hconcat(orig, disp, output);
    cv::imshow("window", output);
    cv::waitKey();
    
    return 0;
}