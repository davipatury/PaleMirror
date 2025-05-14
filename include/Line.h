#ifndef LINE_H
#define LINE_H

#include "Vec2.h"

class Line
{
public:
    Line(Vec2 src, Vec2 dst);
    Vec2 src;
    Vec2 dst;

    Vec2 Dir();
    float Angle();
};

#endif // LINE_H
