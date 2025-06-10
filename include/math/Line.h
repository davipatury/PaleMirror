#ifndef LINE_H
#define LINE_H

#include "math/Vec2.h"

class Line
{
public:
    Line(Vec2 source, Vec2 destination);
    Vec2 source;
    Vec2 destination;

    Vec2 Dir();
    float Angle();
    Vec2 MidPoint();
};

#endif // LINE_H
