#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "linear.h"
#include "floodfill.h"
#include "ctmf.h"

using namespace std;

typedef std::chrono::high_resolution_clock hrc;


////////////////////////////////////////////////////////////////////////////////
static inline int getIdAtScale(
    const int width,
    const int height, 
    const int x, 
    const int y, 
    const int scale)
{
    //    const int xoff = width * (1 - pow(2, floor(-scale / (3.0f / 2.0f))));
    //    const int yoff = height * (1 - pow(2, ceil(-scale / 2.0f)));
    const int xoff = (scale == 0 ? 0 : 2.0f / 3.0f * width);
    const int yoff = height * (scale == 0 ? 0 : 1 - pow(2, -scale + 1));
    
    const int xs = floor(x / pow(2, scale));
    const int ys = floor(y / pow(2, scale));
    
    return (yoff + ys) * width + (xoff + xs);
}


////////////////////////////////////////////////////////////////////////////////
void fillPyramid(uchar* data, const int width, const int height)
{
    //  allocate single channel mipmap
    const int width2 = 3.0f / 2.0f * width;
    const int height2 = height;
    
    uchar mipmap[width2 * height2];
    memset(mipmap, 0, sizeof mipmap);

    //  copy data to first mipmap level
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            const int id1 = row * width + col;
            const int id2 = row * width2 + col;
            mipmap[id2] = data[id1];
        }
    }
    
    //  build all levels up to 2x2 or even 1x1
    for (int i = 1; i < ceil(log(width)) + 3; i++)
    {
        for (int row = 0; row < height; row += 2 * i)
        {
            for (int col = 0; col < width; col += 2 * i)
            {
                //  get pixel neighbours on previous level
                const int s = 2 * (i - 1);
                const uchar c = mipmap[getIdAtScale(width2, height2, col, row, i - 1)];
                const uchar r = mipmap[getIdAtScale(width2, height2, col + s, row, i - 1)];
                const uchar rd = mipmap[getIdAtScale(width2, height2, col + s, row + s, i - 1)];
                const uchar d = mipmap[getIdAtScale(width2, height2, col, row + s, i - 1)];
    
                //  compute downsampled value
//                const uchar val = c;                          //  exact
                const uchar val = MAX(MAX(c, r), MAX(rd, d)); //  max
//                const uchar val = ((int)c + r + rd + d) / 4;  //  average
                
                //  assign value at current level
                const int id = getIdAtScale(width2, height2, col, row, i);
                mipmap[id] = val;
            }
        }
    }
    
    //  fill maks in original data with neighbourhood information
    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            int l = 0;
            const int maxl = ceil(log(width)) + 3;
            
            int id = row * width + col;
            uchar val = data[id];
            
            while(val == 0 && l++ <= maxl) {
                const int ids = getIdAtScale(width2, height2, col, row, l);
                val = mipmap[ids];
            }
            
            data[id] = val;
        }
    }
    
//    cv::Mat img = cv::Mat(height, width, CV_8UC1, data);
//    cv::imshow("window", img);
//    cv::waitKey();
}


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
    fillPyramid(buffer2, width, height);
//    fillLinear(buffer2, width, height, 0, 0, true);
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