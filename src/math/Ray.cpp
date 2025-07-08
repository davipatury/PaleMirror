#include "math/Ray.h"

Ray::Intersection Ray::GetIntersection(Line ray, Line line) {
    float r_px = ray.source.x;
    float r_py = ray.source.y;
    float r_dx = ray.destination.x - r_px;
    float r_dy = ray.destination.y - r_py;

    float s_px = line.source.x;
    float s_py = line.source.y;
    float s_dx = line.destination.x - s_px;
    float s_dy = line.destination.y - s_py;

    // Both lines parallel
    if (r_dx * s_dy == r_dy * s_dx) {
        return Intersection(Vec2(0, 0), 0, false);
    }

    float T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
    float T1 = (s_dx * (r_py - s_py) + s_dy * (s_px - r_px)) / (r_dx * s_dy - r_dy * s_dx);

    if (T1 < 0) {
        return Intersection(Vec2(0, 0), 1, false);
    }
    if (T2 < 0 || 1 < T2) {
        return Intersection(Vec2(0, 0), 2, false);
    }

    return Intersection({r_px + r_dx * T1, r_py + r_dy * T1}, T1, true);
}

Ray::Intersection Ray::ClosestIntersection(Line ray, std::vector<Line> lines) {
    std::vector<Intersection> allInters = AllIntersections(ray, lines);
    if (allInters.size() > 0) return allInters[0];
    return Intersection(Vec2(0, 0), 0, false);
}

std::vector<Ray::Intersection> Ray::AllIntersections(Line ray, std::vector<Line> lines) {
    std::vector<Intersection> inters;
    for (int i = 0; i < lines.size(); i++) {
        Line line = lines[i];
        Intersection inter = GetIntersection(ray, line);
        if (inter.exists) {
            inters.insert(std::upper_bound(inters.begin(), inters.end(), inter), inter);
        }
    }
    return inters;
}

Ray::Intersection::Intersection(Vec2 pos, float param, bool exists) {
    this->pos = pos;
    this->param = param;
    this->exists = exists;
}
