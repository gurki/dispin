#ifndef MIPMAP_H
#define MIPMAP_H


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
void fillPyramid(
    uchar* data, 
    const int width, 
    const int height)
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


#endif