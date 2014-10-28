#ifndef LINEAR_H
#define LINEAR_H

#include "common.h"


/*
    comments in the vertical versions
 */


////////////////////////////////////////////////////////////////////////////////
//  forward declarations
void fillLinearHorizontalSegment(
    uchar* data,
    const int width,
    const int height,
    vec2 pos0,
    vec2 pos1
);

void fillLinearHorizontal(
    uchar* data,
    const int width,
    const int height,
    const int offset = 0,
    const uchar thresh = 0
);

void fillLinearVerticalSegment(
    uchar* data,
    const int width,
    const int height,
    vec2 pos0,
    vec2 pos1
);

void fillLinearVertical(
    uchar* data,
    const int width,
    const int height,
    const int offset = 0,
    const uchar thresh = 0
);

void fillLinear(
    uchar* data,
    const int width,
    const int height,
    const int offset = 0,
    const uchar thresh = 0,
    const bool average = true
);


////////////////////////////////////////////////////////////////////////////////
void fillLinearHorizontalSegment(
    uchar* data,
    const int width,
    const int height,
    vec2 pos0,
    vec2 pos1)
{
    pos0.x = MAX(pos0.x, 0);
    pos1.x = MIN(pos1.x, width - 1);
    
    const int id0 = pos0.y * width + pos0.x;
    const int id1 = pos1.y * width + pos1.x;
    
    const uchar val0 = data[id0] > 0 ? data[id0] : data[id1];
    const uchar val1 = data[id1] > 0 ? data[id1] : data[id0];
    
    const int delta = (int)val1 - (int)val0;
    const int dist = pos1.x - pos0.x;
    float n = dist;
    
    for (int col = pos1.x; col >= pos0.x; col--)
    {
        const int id = pos0.y * width + col;
        
        data[id] = val0 + (uchar)(delta * n / dist);
        n -= 1.0f;
    }
}


////////////////////////////////////////////////////////////////////////////////
void fillLinearHorizontal(
    uchar* data,
    const int width,
    const int height,
    const int offset,
    const uchar thresh)
{
    for (int row = 0; row < height; row++)
    {
        bool isSearching = false;
        vec2 pos0;
        
        for (int col = 0; col < width; col++)
        {
            const int id = row * width + col;
            
            if (!isSearching && data[id] <= thresh) {
                isSearching = true;
                pos0 = vec2(col - 1 - offset, row);
            }
            
            if (isSearching && (data[id] > thresh || col == width - 1)) {
                vec2 pos1(col + offset, row);
                fillLinearHorizontalSegment(data, width, height, pos0, pos1);
                isSearching = false;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void fillLinearVerticalSegment(
    uchar* data,
    const int width,
    const int height,
    vec2 pos0,
    vec2 pos1)
{
    //  don't go out of image range
    pos0.y = MAX(pos0.y, 0);
    pos1.y = MIN(pos1.y, height - 1);
    
    //  get linear indices
    const int id0 = pos0.y * width + pos0.x;
    const int id1 = pos1.y * width + pos1.x;
    
    //  get start and end values
    //  assign other side if we started/ended at a border
    const uchar val0 = data[id0] > 0 ? data[id0] : data[id1];
    const uchar val1 = data[id1] > 0 ? data[id1] : data[id0];
    
    //  create linear function
    const int delta = (int)val1 - (int)val0;
    const int dist = pos1.y - pos0.y;
    float n = dist;
    
    //  go from end to start and assign linearly interpolated values
    for (int row = pos1.y; row >= pos0.y; row--)
    {
        const int id = row * width + pos0.x;
        
        data[id] = val0 + (uchar)(delta * n / dist);
        n -= 1.0f;
    }
}


////////////////////////////////////////////////////////////////////////////////
void fillLinearVertical(
    uchar* data,
    const int width,
    const int height,
    const int offset,
    const uchar thresh)
{
    for (int col = 0; col < width; col++)
    {
        bool isSearching = false;
        vec2 pos0;
        
        for (int row = 0; row < height; row++)
        {
            const int id = row * width + col;
            
            //  activate search mode if we found a dark enough pixel
            if (!isSearching && data[id] <= thresh) {
                isSearching = true;
                //  remember starting position
                pos0 = vec2(col, row - 1 - offset);
            }
            
            //  if we reached a bright pixel again or the end of the image, fill the line segment
            if (isSearching && (data[id] > thresh || row == height - 1)) {
                vec2 pos1(col, row + offset);
                fillLinearVerticalSegment(data, width, height, pos0, pos1);
                isSearching = false;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void fillLinear(
    uchar* data,
    const int width,
    const int height,
    const int offset,
    const uchar thresh,
    const bool average)
{
    //  fill hor and vert seperately
    uchar vert[width * height];
    std::memcpy(vert, data, sizeof vert);
    
    fillLinearHorizontal(data, width, height, offset, thresh);
    fillLinearVertical(vert, width, height, offset, thresh);
    
    //  blend
    for (int i = 0; i < width * height; i++)
    {
        if (average) {
            data[i] = 0.5f * data[i] + 0.5f * vert[i];
        } else {
            data[i] = MAX(data[i], vert[i]);
        }
    }
}


#endif