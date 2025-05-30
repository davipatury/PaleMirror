#include "math/Ray.h"

Ray::Intersection Ray::GetIntersection(Line ray, Line line) {
    float r_px = ray.src.x;
    float r_py = ray.src.y;
    float r_dx = ray.dst.x - r_px;
    float r_dy = ray.dst.y - r_py;

    float s_px = line.src.x;
    float s_py = line.src.y;
    float s_dx = line.dst.x - s_px;
    float s_dy = line.dst.y - s_py;

    // Both lines parallel
    if (r_dx * s_dy == r_dy * s_dx) {
        return Intersection(Vec2(0, 0), 0, false);
    }

    float T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
    float T1 = (s_dx * (r_py - s_py) + s_dy * (s_px - r_px)) / (r_dx * s_dy - r_dy * s_dx);

    if (T1 < 0) {
        return Intersection(Vec2(0, 0), 1, false);
    }
    if (T2 < 0 || T2 > 1) {
        return Intersection(Vec2(0, 0), 2, false);
    }

    return Intersection({r_px + r_dx * T1, r_py + r_dy * T1}, T1, true);
}

Ray::Intersection Ray::ClosestIntersection(Line ray, std::vector<Line> lines) {
    float closest = -1;
    Intersection closestInter = Intersection(Vec2(0, 0), 0, false);
    for (int i = 0; i < lines.size(); i++) {
        Line line = lines[i];
        Intersection inter = GetIntersection(ray, line);
        if (inter.exists && (closest == -1 || inter.param < closest)) {
            closest = inter.param;
            closestInter = inter;
        }
    }
    return closestInter;
}

std::vector<Ray::Intersection> Ray::AllIntersections(Line ray, std::vector<Line> lines) {
    std::vector<Intersection> intersections;
    for (int i = 0; i < lines.size(); i++) {
        Line line = lines[i];
        Intersection inter = GetIntersection(ray, line);
        if (inter.exists) {
            intersections.push_back(inter);
        }
    }
    return intersections;
}

Ray::Intersection::Intersection(Vec2 pos, float param, bool exists) {
    this->pos = pos;
    this->param = param;
    this->exists = exists;
}
