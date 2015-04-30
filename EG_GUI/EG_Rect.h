#ifndef EG_RECT_H_
#define EG_RECT_H_

#include "define.h"

struct EG_Rect
{
    int x;
    int y;
    int w;
    int h;



    void setXY(int posX, int posY)
    {
        x = posX;
        y = posY;
    }


    void setWH(int width, int height)
    {
        w = width;
        h = height;
    }

    void set(int x, int y, int width, int height)
    {
        setXY(x,y);
        setWH(width, height);
    }

};

#endif // EG_RECT_H_
