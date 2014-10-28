#include <iostream>
#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
void sharpenEdges(
    const uchar* data,
    const int width,
    const int height,
    uchar* out,
    const uchar thresh = 0)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            const int id = row * width + col;
            
            out[id] = data[id];

            if (data[id] == 0) {
                continue;
            }
            
            const uchar& val = data[id];
            
            const uchar vall = col > 0 ? data[id-1] : val;
            const uchar valr = col < width-1 ? data[id+1] : val;
            const uchar valu = row > 0 ? data[id-width] : val;
            const uchar vald = row < height - 1 ? data[id+width] : val;

            const uchar min = MIN(MIN(vall, valr), MIN(valu, vald));

            if (min <= thresh) {
                const uchar max = MAX(MAX(vall, valr), MAX(valu, vald));
                out[id] = max;
            }
        }
    }
}


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
    
//    fillLinear(disp.data, width, height, 1, 0);
//    layerFloodFill(data, width, height);
    sharpenEdges(disp.data, width, height, sharp.data);
    
    //  plot
//    cv::Mat output;
//    cv::hconcat(orig, disp, output);
//    cv::imshow("window", output);
    cv::imshow("window", sharp);
    cv::waitKey();
    
    return 0;
}