#include "math/Vec2.h"

Vec2::Vec2() {
    x = 0;
    y = 0;
}

Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec2 Vec2::Add(Vec2 vec) {
    return Vec2(x + vec.x, y + vec.y);
}

Vec2 Vec2::operator+(Vec2 const& vec) {
    return Add(vec);
}

Vec2 Vec2::Sub(Vec2 vec) {
    return Vec2(x - vec.x, y - vec.y);
}

Vec2 Vec2::operator-(Vec2 const& vec) {
    return Sub(vec);
}

Vec2 Vec2::MulScalar(float scalar) {
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator*(float const scalar) {
    return MulScalar(scalar);
}

float Vec2::Magnitude() {
    return sqrt((x * x) + (y * y));
}

Vec2 Vec2::Normalized() {
    float mag = Magnitude();
    return MulScalar(1 / mag);
}

float Vec2::Distance(Vec2 vec) {
    Vec2 sub = Sub(vec);
    return sub.Magnitude();
}

float Vec2::Angle() {
    return atan2(y, x);
}

float Vec2::Angle(Vec2 vec) {
    Vec2 sub = Sub(vec);
    return sub.Angle();
}

Vec2 Vec2::Rotate(float angle) {
    float coss = cos(angle);
    float seno = sin(angle);
    return Vec2(x * coss - y * seno, y * coss + x * seno);
}

Vec2 Vec2::ToCart() {
    return {(x - y) / 2.0f, (x + y) / 4.0f};
}

Vec2 Vec2::ToIso() {
    return {x + 2.0f * y, -x + 2.0f * y};
}
