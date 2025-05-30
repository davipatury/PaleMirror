#ifndef ISORECT_H
#define ISORECT_H

#include "math/Line.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class IsoRect
{
public:
    IsoRect();
    IsoRect(float x, float y, float w, float h);
    IsoRect(Rect rect);

    Vec2 Center();
    Vec2 Right();
    Vec2 Left();
    Vec2 Top();
    Vec2 Bottom();

    IsoRect Add(Vec2 vec);
    void SetWidth(float w);
    float CenterDistance(IsoRect rect);
    bool Contains(Vec2 vec);

    float x;
    float y;
    float z;
    float w;
    float h;
    float catOp;
};

#endif // ISORECT_H
