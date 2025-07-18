#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "math/Line.h"
#include "math/Vec2.h"

class Ray
{
public:
    class Intersection {
    public:
        Intersection(Vec2 pos, float param, bool exists);
        bool operator< (const Intersection &other) const {
            return param < other.param;
        }
        Vec2 pos;
        float param;
        bool exists;
    };

    static Intersection ClosestIntersection(Line ray, std::vector<Line> lines);
    static std::vector<Ray::Intersection> AllIntersections(Line ray, std::vector<Line> lines);
    static Intersection GetIntersection(Line ray, Line line);
};

#endif // RAY_H
