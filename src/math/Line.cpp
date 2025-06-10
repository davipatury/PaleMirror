#include "math/Line.h"
#include "math/Vec2.h"

Line::Line(Vec2 source, Vec2 destination) {
    this->source = source;
    this->destination = destination;
}

Vec2 Line::Dir() {
    return destination.Sub(source).Normalized();
}

float Line::Angle() {
    return source.Angle(destination);
}

Vec2 Line::MidPoint(){
    return Vec2((source.x + destination.x)/2,(source.y + destination.y)/2);
}
