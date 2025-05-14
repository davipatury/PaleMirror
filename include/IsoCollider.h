#ifndef ISOCOLLIDER_H
#define ISOCOLLIDER_H

#include <iostream>
#include <string>
#include <vector>

#include "GameObject.h"
#include "IsoRect.h"
#include "Line.h"
#include "Rect.h"
#include "Vec2.h"

class IsoCollider : public Component
{
public:
    IsoCollider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0}, bool opaque = true);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    std::vector<Vec2> GetPoints();
    std::vector<Vec2> GetExtendedPoints();
    std::vector<Line> GetLines();

    IsoRect box;
    bool opaque;
private:
    Vec2 scale;
    Vec2 offset;
};

#endif // ISOCOLLIDER_H
