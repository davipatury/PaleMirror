#ifndef VEC2_H
#define VEC2_H

#include <math.h>

const long double EPS = 1e-9;

class Line; // Forward declaration

class Vec2
{
public:
    Vec2();
    Vec2(float x, float y);

    Vec2 Add(Vec2 vec);
    Vec2 operator+(Vec2 const& vec);
    Vec2 Sub(Vec2 vec);
    Vec2 operator-(Vec2 const& vec);
    Vec2 MulScalar(float scalar);
    Vec2 operator*(float const scalar);

    float Magnitude();
    Vec2 Normalized();
    float Distance(Vec2 vec);
    float Angle();
    float Angle(Vec2 vec);
    Vec2 Rotate(float angle);

    float operator^(Vec2 const& vec);
    int PointLine(Line l);

    Vec2 ToIso();
    Vec2 ToCart();

    float x;
    float y;
};

#endif // VEC2_H
