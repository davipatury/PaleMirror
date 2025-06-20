#include "physics/IsoCollider.h"

#define DEBUG_COLLIDER
#ifdef DEBUG_COLLIDER
#include <math.h>
#include "SDL_image.h"
#include "components/Camera.h"
#include "core/Game.h"
#endif

IsoCollider::IsoCollider(GameObject& associated, Vec2 scale, Vec2 offset, bool passable) : Component(associated) {
    this->scale = scale;
    this->offset = offset;
    this->passable = passable;
}

void IsoCollider::Update(float dt) {
    Vec2 bli = (associated.box.BottomLeft() + offset).ToIso();
    Vec2 tl = bli.Add({0, -associated.box.w});

    prevBox = box;
    box = Rect(tl.x, tl.y, associated.box.w * scale.x, associated.box.w * scale.y);
}

void IsoCollider::Render() {
#ifdef DEBUG_COLLIDER
    SDL_Point points[5];

    Vec2 point = box.TopLeft().ToCart() - Camera::pos;
    points[0] = {(int) point.x, (int) point.y};
    points[4] = {(int) point.x, (int) point.y};

    point = box.TopRight().ToCart() - Camera::pos;
    points[1] = {(int) point.x, (int) point.y};

    point = box.BottomRight().ToCart() - Camera::pos;
    points[2] = {(int) point.x, (int) point.y};

    // Left
    point = box.BottomLeft().ToCart() - Camera::pos;
    points[3] = {(int) point.x, (int) point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoints(Game::GetInstance().GetRenderer(), points, 5);

    // Center
    point = box.Center() - Camera::pos;
    SDL_RenderDrawPoint(Game::GetInstance().GetRenderer(), point.x, point.y);

    Vec2 pointleft = box.BottomLeft().ToCart() - Camera::pos;
    Vec2 pointright = box.TopRight().ToCart() - Camera::pos;

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), pointleft.x, pointleft.y, pointright.x, pointright.y);

    
#endif // DEBUG
}

void IsoCollider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void IsoCollider::SetOffset(Vec2 offset) {
    this->offset = offset;
}
bool IsoCollider::Is(std::string type) {
    return type == "IsoCollider";
}

