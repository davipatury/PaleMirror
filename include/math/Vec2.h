#ifndef VEC2_H
#define VEC2_H

#include <math.h>
#include <string>
#include <SDL_image.h>

const long double EPS = 1e-9;

class Line; // Forward declaration

class Vec2
{
public:
    Vec2();
    Vec2(float x, float y);

    Vec2 Add(Vec2 vec);
    Vec2 operator+(Vec2 const& vec);
    bool operator==(Vec2 const& o);
    bool operator!=(Vec2 const& o);
    Vec2 Sub(Vec2 vec) const;
    Vec2 operator-(Vec2 const& vec);
    Vec2 MulScalar(float scalar);
    Vec2 operator*(float const scalar);

    float Magnitude();
    Vec2 Normalized();
    float Distance(Vec2 vec) const;
    float Angle() const;
    float Angle(Vec2 vec) const;
    Vec2 Rotate(float angle);

    float operator^(Vec2 const& vec);
    int PointLine(Line l);

    Vec2 ToIso();
    Vec2 ToCart();

    float Dot(Vec2 b) const;
    float Cross(Vec2 b) const;

    std::string ToStr();
    SDL_Vertex ToSDLVertex(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255, float tex_x = 0, float tex_y = 0);

    float x;
    float y;
};

#endif // VEC2_H
