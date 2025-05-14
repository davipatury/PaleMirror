#include "Rect.h"

Rect::Rect() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
    h = 0;
}

Rect::Rect(float x, float y, float w, float h) {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = w;
    this->h = h;
}

Rect Rect::Add(Vec2 vec) {
    return Rect(x + vec.x, y + vec.y, w, h);
}

Vec2 Rect::Center() {
    return Vec2(x + (w / 2), y + (h / 2));
}

float Rect::CenterDistance(Rect rect) {
    return Center().Distance(rect.Center());
}

bool Rect::Contains(Vec2 vec) {
    return (vec.x >= x && vec.x <= (x + w) && vec.y >= y && vec.y <= (y + h));
}
