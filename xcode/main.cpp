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
            const uchar valul = row > 0 && col > 0 ? data[id-1-width] : val;
            const uchar valur = row > 0 && col < width-1 ? data[id+1-width] : val;
            const uchar valdl = row < height-1 && col > 0 ? data[id-1+width] : val;
            const uchar valdr = row < height-1 && col < width-1 ? data[id+1+width] : val;

            const uchar min1 = MIN(MIN(vall, valr), MIN(valu, vald));
            const uchar min2 = MIN(MIN(valul, valur), MIN(valdl, valdr));
            const uchar min = MIN(min1, min2);

            if (min <= thresh)
            {
                const uchar max1 = MAX(MAX(vall, valr), MAX(valu, vald));
                const uchar max2 = MAX(MAX(valul, valur), MAX(valdl, valdr));
                const uchar max = MAX(max1, max2);

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
    uchar* data1 = disp.data;
    uchar* data2 = sharp.data;
    
    for (int i = 0; i < 100; i++) {
        sharpenEdges(data1, width, height, data2);
        std::swap(data1, data2);
    }
    
    //  plot
//    cv::Mat output;
//    cv::hconcat(orig, disp, output);
//    cv::imshow("window", output);
    cv::imshow("window", sharp);
    cv::waitKey();
    
    return 0;
}