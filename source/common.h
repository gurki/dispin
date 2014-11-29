#ifndef COMMON_H
#define COMMON_H


////////////////////////////////////////////////////////////////////////////////
//  forward declarations
struct vec2;

void mark(
    uchar* data,
    const int width,
    const int height,
    const uchar value = 0,
    const uchar mark = 255,
    const uchar nonMark = 0
);

void sharpenEdges(
    const uchar* data,
    const int width,
    const int height,
    uchar* out,
    const uchar thresh = 0
);


////////////////////////////////////////////////////////////////////////////////
struct vec2
{
    vec2(const int x__ = 0, const int y__ = 0) :
    x(x__),
    y(y__)
    {};
    
    int x;
    int y;
};


////////////////////////////////////////////////////////////////////////////////
//  find and sharpen single-pixel edges
//    e.g. [0 30 200 210] -> [0 200 200 210]
//    CARE: does not work in place!
void sharpenEdges(
    const uchar* data,
    const int width,
    const int height,
    uchar* out,
    const uchar thresh)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            const int id = row * width + col;
            
            out[id] = data[id];

            //  nothing to do for mask pixels
            if (data[id] == 0) {
                continue;
            }
            
            const uchar& val = data[id];
            
            //  get all pixel neighbours
            const uchar vall = col > 0 ? data[id-1] : val;
            const uchar valr = col < width-1 ? data[id+1] : val;
            const uchar valu = row > 0 ? data[id-width] : val;
            const uchar vald = row < height - 1 ? data[id+width] : val;
            const uchar valul = row > 0 && col > 0 ? data[id-1-width] : val;
            const uchar valur = row > 0 && col < width-1 ? data[id+1-width] : val;
            const uchar valdl = row < height-1 && col > 0 ? data[id-1+width] : val;
            const uchar valdr = row < height-1 && col < width-1 ? data[id+1+width] : val;

            //  is current pixel adjacent to mask?
            const uchar min1 = MIN(MIN(vall, valr), MIN(valu, vald));
            const uchar min2 = MIN(MIN(valul, valur), MIN(valdl, valdr));
            const uchar min = MIN(min1, min2);

            if (min <= thresh)
            {
                //  if so, assign it the brightes neighbour to sharpen the edge
                const uchar max1 = MAX(MAX(vall, valr), MAX(valu, vald));
                const uchar max2 = MAX(MAX(valul, valur), MAX(valdl, valdr));
                const uchar max = MAX(max1, max2);

                out[id] = max;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
//  mark pixels with a certain value in a specific color
//  mark all other pixels in another color
//    default:
//      set all zero pixels to white (255)
//      set all non-zero pixels to black (0)
void mark(
    uchar* data,
    const int width,
    const int height,
    const uchar value,
    const uchar mark,
    const uchar nonMark)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            const int id = row * width + col;
            
            if (data[id] == value) {
                data[id] = mark;
            } else {
                data[id] = nonMark;
            }
        }
    }
}


#endif