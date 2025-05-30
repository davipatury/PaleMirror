#include "math/Line.h"

Line::Line(Vec2 src, Vec2 dst) {
    this->src = src;
    this->dst = dst;
}

Vec2 Line::Dir() {
    return dst.Sub(src);
}

float Line::Angle() {
    return src.Angle(dst);
}
