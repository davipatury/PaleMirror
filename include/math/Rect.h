#ifndef RECT_H
#define RECT_H

#define EMPTY_RECT Rect(0, 0, 0, 0)

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
    inline bool operator==(Rect const& rect) {
        return x == rect.x && y == rect.y && w == rect.w && h == rect.h;
    }
    inline bool operator!=(Rect const& rect) { return !(operator==(rect)); }
    inline SDL_Rect ToSDLRect() {
        return SDL_Rect{(int) x, (int) y, (int) w, (int) h};
    }

    float CenterDistance(Rect rect);
    bool Contains(Vec2 vec);
    bool Collides(Rect r);

    float x;
    float y;
    float z;
    float w;
    float h;
};

#endif // RECT_H
