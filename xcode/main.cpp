#include <iostream>
#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
int main(const int argc, const char* argv[])
{
    //  load and display image
    cv::Mat disp = cv::imread("../data/disparity.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    uchar* data = disp.data;
    
    cv::imshow("window", disp);
    cv::waitKey();
    
    cv::Mat disp0;
    disp.copyTo(disp0);
    
    //  process
    const int width = disp.cols;
    const int height = disp.rows;
    
    layerFloodFill(data, width, height);
    
    //  plot
    cv::Mat output;
    cv::hconcat(disp0, disp, output);
    cv::imshow("window", output);
    cv::waitKey();
    
    return 0;
}