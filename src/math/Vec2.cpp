#include "math/Vec2.h"
#include "math/Line.h"
#include <iostream>

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

bool Vec2::operator==(Vec2 const& o){
    return x == o.x && y == o.y;
}
bool Vec2::operator!=(Vec2 const& o){
    return !(*this == o);
}

bool Vec2::operator<(Vec2 const& o){
    if(o.x == x){
        return y < o.y;
    }else{
        return x < o.x;
    }
}

Vec2 Vec2::Sub(Vec2 vec) const {
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

float Vec2::Distance(Vec2 vec) const {
    Vec2 sub = Sub(vec);
    return sub.Magnitude();
}

float Vec2::Angle() const {
    return atan2(y, x);
}

float Vec2::AngleW() const {
    return atan2(y, x);
}

float Vec2::AngleN() const {
    return atan2(x, y);
}

float Vec2::AngleE() const {
    return atan2(-y, -x);
}

float Vec2::AngleS() const {
    return atan2(-x, -y);
}

float Vec2::Angle(Vec2 vec) const {
    Vec2 sub = Sub(vec);
    return sub.Angle();
}

Vec2 Vec2::Rotate(float angle) {
    float coss = cos(angle);
    float seno = sin(angle);
    return Vec2(x * coss - y * seno, y * coss + x * seno);
}

// Iso magic number = 0.578125
Vec2 Vec2::ToCart() {
    //return {(x - y) / 2.0f, (x + y) / 4.0f}; old 26.6º
    return {(float) (x * 0.578125 - y * 0.578125), (float) (x * 0.334228 + y * 0.334228)};
}

Vec2 Vec2::ToIso() {
    //return {x + 2.0f * y, -x + 2.0f * y}; old 26.6º
    return {(float) (x * 0.864864 + y * 1.4959824), (float) (x * -0.864864 + y * 1.4959824)};
}

float Vec2::operator^(Vec2 const& o){ 
    return x * o.y - y * o.x; 
}
// -1 = esq, 0 = coll, 1 = dir
int Vec2::PointLine(Line l){
    Vec2 a = l.destination - l.source;
    Vec2 b = Sub(l.destination);
    float tmp = a ^ b;
    return (tmp > EPS) - (tmp < EPS);
}

std::string Vec2::ToStr() {
    return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
}

SDL_Vertex Vec2::ToSDLVertex(Uint8 r, Uint8 g , Uint8 b, Uint8 a, float tex_x, float tex_y) {
    SDL_FPoint pos;
    pos.x = x;
    pos.y = y;
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    SDL_FPoint tex_coord;
    tex_coord.x = tex_x;
    tex_coord.y = tex_y;
    SDL_Vertex vertex;
    vertex.position = pos;
    vertex.color = color;
    vertex.tex_coord = tex_coord;
    return vertex;
}

float Vec2::Dot(Vec2 b) const {
    return x * b.x + y * b.y;
}

float Vec2::Cross(Vec2 b) const {
    return x * b.x - y * b.y;
}
