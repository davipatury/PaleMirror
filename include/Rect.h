#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

class Rect
{
public:
    Rect();
    Rect(float x, float y, float w, float h);

    Rect Add(Vec2 vec);
    Vec2 Center();
    float CenterDistance(Rect rect);
    bool Contains(Vec2 vec);

    float x;
    float y;
    float z;
    float w;
    float h;
};

#endif // RECT_H
