#include "IsoCollider.h"

#define DEBUG_COLLIDER
#ifdef DEBUG_COLLIDER
#include <math.h>
#include "SDL_image.h"
#include "Camera.h"
#include "Game.h"
#endif

IsoCollider::IsoCollider(GameObject& associated, Vec2 scale, Vec2 offset, bool opaque) : Component(associated) {
    this->scale = scale;
    this->offset = offset;
    this->opaque = opaque;
}

void IsoCollider::Update(float dt) {
    Vec2 bli = (associated.box.BottomLeft() + offset).ToIso();
    Vec2 tl = bli.Add({0, -associated.box.w});

    box = Rect(tl.x, tl.y, associated.box.w, associated.box.w);
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

/*
    Vec2 bli = to_iso(associated.box.BottomLeft());

    Vec2 tl = to_cart(bli.Add({0, -associated.box.w}));
    Vec2 tr = to_cart(bli.Add({associated.box.w, -associated.box.w}));
    Vec2 br = to_cart(bli.Add({associated.box.w, 0}));
    Vec2 bl = to_cart(bli);

    SDL_Point points[5];
    Vec2 point = tl - Camera::pos;
    points[0] = {(int) point.x, (int) point.y};
    points[4] = {(int) point.x, (int) point.y};

    point = tr - Camera::pos;
    points[1] = {(int) point.x, (int) point.y};

    point = br - Camera::pos;
    points[2] = {(int) point.x, (int) point.y};

    point = bl - Camera::pos;
    points[3] = {(int) point.x, (int) point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoints(Game::GetInstance().GetRenderer(), points, 5);
*/
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
