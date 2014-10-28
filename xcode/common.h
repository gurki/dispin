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
//  mark pixels
//  default:
//    set all zero pixels to white (255)
//    set all non-zero pixels to black (0)
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