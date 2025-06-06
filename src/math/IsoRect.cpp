#include "math/IsoRect.h"

#define TAN30 0.57735026918962576450914878050196

IsoRect::IsoRect() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
    h = 0;
    catOp = 0;
}

IsoRect::IsoRect(float x, float y, float w, float h) {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = w;
    this->h = h;
    catOp = TAN30 * w * 0.5;
}

IsoRect::IsoRect(Rect rect) {
    float halfWidth = rect.w * 0.5;
    x = rect.x + halfWidth;
    y = rect.y + rect.h;
    z = rect.z;
    w = rect.w;
    h = rect.h;
    catOp = TAN30 * halfWidth;
}

Vec2 IsoRect::Center() {
    return Vec2(x, y);
}

Vec2 IsoRect::Right() {
    return Vec2(x + w * 0.5, y);
}

Vec2 IsoRect::Left() {
    return Vec2(x - w * 0.5, y);
}

Vec2 IsoRect::Top() {
    return Vec2(x, y - catOp);
}

Vec2 IsoRect::Bottom() {
    return Vec2(x, y + catOp);
}

IsoRect IsoRect::Add(Vec2 vec) {
    return IsoRect(x + vec.x, y + vec.y, w, h);
}

void IsoRect::SetWidth(float w) {
    this->w = w;
    catOp = TAN30 * w * 0.5;
}

float IsoRect::CenterDistance(IsoRect rect) {
    return Center().Distance(rect.Center());
}

bool IsoRect::Contains(Vec2 vec) {
    return false;
}
