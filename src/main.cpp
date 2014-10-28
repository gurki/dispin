#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"
#include "ctmf.h"

using namespace std;

typedef std::chrono::high_resolution_clock hrc;


////////////////////////////////////////////////////////////////////////////////
int main(const int argc, const char* argv[])
{
    //  load and display image
    cv::Mat input = cv::imread("../data/disparity.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    
    const int width = input.cols;
    const int height = input.rows;
    
    cv::imshow("window", input);
    cv::waitKey();
    
    //  start measuring time
    hrc::time_point t0 = hrc::now();
    
/*
     pure c from here ...
 */
    
    //  create necessary buffers
    uchar buffer1[width * height];
    uchar buffer2[width * height];
    
    std::memcpy(buffer1, input.data, sizeof buffer1);
    
    //  process
    sharpenEdges(buffer1, width, height, buffer2, 0);
    fillLinear(buffer2, width, height, 0, 0, true);
    ctmf(buffer2, buffer1, width, height, width, width, 2, 1, 512*1024);

/*
     ... to here
 */
    
    //  stop measruing time
    cout << (float)(hrc::now() - t0).count() / 1e6 << endl;
    
    //  plot
    cv::Mat output = cv::Mat(height, width, CV_8UC1, buffer1);
    cv::Mat sideBySide;
    cv::hconcat(input, output, sideBySide);
    cv::imshow("window", sideBySide);
    cv::waitKey();
    
    return 0;
}