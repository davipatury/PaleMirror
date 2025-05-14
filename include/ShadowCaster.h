#ifndef SHADOWCASTER_H
#define SHADOWCASTER_H

#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Character.h"
#include "Game.h"
#include "GameObject.h"
#include "IsoRect.h"
#include "Line.h"
#include "Ray.h"
#include "Vec2.h"

class ShadowCaster : public Component
{
public:
    ShadowCaster(GameObject& associated, Vec2 offset = {0, 0});
    ShadowCaster(GameObject& associated, Vec2 offset, std::vector<Vec2> vertices);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    std::vector<Vec2> vertices;
    Vec2 offset;
private:
    std::vector<Line> GenerateEdges(std::vector<Vec2> vertices);
    Vec2 GetVerticesCenter();
};

#endif // SHADOWCASTER_H
