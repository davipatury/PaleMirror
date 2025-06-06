#ifndef RECT_H
#define RECT_H

#include "math/Vec2.h"

class Rect
{
public:
    Rect();
    Rect(float x, float y, float w, float h);

    Rect Add(Vec2 vec);

    inline Vec2 TopLeft() const { return { x, y }; }
    inline Vec2 TopRight() const { return { x + w, y }; }
    inline Vec2 BottomLeft() const { return { x, y + h }; }
    inline Vec2 BottomRight() const { return { x + w, y + h }; }
    inline Vec2 Center() const {
        return { x + (w / 2), y + (h / 2) };
    }

    float CenterDistance(Rect rect);
    bool Contains(Vec2 vec);

    float x;
    float y;
    float z;
    float w;
    float h;
};

#endif // RECT_H
