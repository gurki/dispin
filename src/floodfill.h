#ifndef FLOOD_FILL_H
#define FLOOD_FILL_H

#include <stack>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
//  forward declarations
void floodFillIterative(
   uchar* data,
   const int width,
   const int height,
   const vec2& pos,
   const uchar value,
   const uchar maskValue
);

void floodFill(
    uchar* data,
    const int width,
    const int height,
    const vec2& pos,
    const uchar value,
    const uchar maskValue
);

void layerFloodFill(
    uchar* data,
    const int width,
    const int height
);


////////////////////////////////////////////////////////////////////////////////
void floodFillIterative(
   uchar* data,
   const int width,
   const int height,
   const vec2& pos,
   const uchar value,
   const uchar maskValue)
{
   stack<vec2> poss;
   
   poss.push(pos);
   
   while(!poss.empty())
   {
       const vec2& curr = poss.top();
       
       if (curr.x < 0 || curr.x >= width || curr.y < 0 || curr.y >= height) {
           return;
       }
       
       const int id = curr.y * width + curr.x;
       
       if (data[id] != maskValue || data[id] == value) {
           return;
       }
       
       data[id] = value;
       
       poss.pop();
       poss.push(vec2(pos.x - 1, pos.y));
       poss.push(vec2(pos.x + 1, pos.y));
       poss.push(vec2(pos.x, pos.y - 1));
       poss.push(vec2(pos.x, pos.y + 1));
   }
}


////////////////////////////////////////////////////////////////////////////////
void floodFill(
    uchar* data,
    const int width,
    const int height,
    const vec2& pos,
    const uchar value,
    const uchar maskValue)
{
    //  out of image
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return;
    }
    
    const int id = pos.y * width + pos.x;
    
    //  not a hole
    if (data[id] != maskValue) {
        return;
    }
    
    //  fill current pixel and proceed to neighbours
    data[id] = value;
    
    floodFill(data, width, height, vec2(pos.x - 1, pos.y), value, maskValue);
    floodFill(data, width, height, vec2(pos.x + 1, pos.y), value, maskValue);
    floodFill(data, width, height, vec2(pos.x, pos.y - 1), value, maskValue);
    floodFill(data, width, height, vec2(pos.x, pos.y + 1), value, maskValue);
}


////////////////////////////////////////////////////////////////////////////////
void layerFloodFill(
    uchar* data,
    const int width,
    const int height)
{
    //  (prioritised) fill holes from fore- (255) to background (1)
    for (uchar layer = 255; layer > 0; layer--)
    {       
        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                const int id = row * width + col;
                
                //  if we found a pixel on the current layer, fill all holes surrounding it
                if (data[id] == layer)
                {
                    floodFill(data, width, height, vec2(col - 1, row), layer, 0);
                    floodFill(data, width, height, vec2(col + 1, row), layer, 0);
                    floodFill(data, width, height, vec2(col, row - 1), layer, 0);
                    floodFill(data, width, height, vec2(col, row + 1), layer, 0);
                }
            }
        }
    }
}


#endif