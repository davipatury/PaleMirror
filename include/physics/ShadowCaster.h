#ifndef SHADOWCASTER_H
#define SHADOWCASTER_H

#include <iostream>
#include <string>
#include <vector>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Game.h"
#include "core/GameObject.h"
#include "math/IsoRect.h"
#include "math/Line.h"
#include "math/Ray.h"
#include "math/Vec2.h"

class ShadowCaster : public Component
{
public:
    ShadowCaster(GameObject& associated, Vec2 offset = {0, 0}, bool fixedDir = false);
    ShadowCaster(GameObject& associated, std::vector<Vec2> offsetVectors);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void RenderShadow(Vec2 origin);

    std::vector<Vec2> vertices;
    Vec2 offset;

    class Controller : public Component {
    public:
        Controller(GameObject& associated);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
    };

private:
    std::vector<Line> GenerateEdges(std::vector<Vec2> vertices);
    bool fixedVertices;
    std::vector<Vec2> offsetVectors;
    SDL_Texture* shadow;
    bool fixedDir;
};

#endif // SHADOWCASTER_H
