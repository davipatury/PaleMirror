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
    box = IsoRect(associated.box);
    box.w = box.w * scale.x;
    box.h = box.h * scale.y;
    box = box.Add(offset);
}

std::vector<Vec2> IsoCollider::GetPoints() {
    return std::vector<Vec2>{
        box.Top(),
        box.Right(),
        box.Bottom(),
        box.Left(),
    };
}

std::vector<Vec2> IsoCollider::GetExtendedPoints() {
    return std::vector<Vec2>{
        box.Top(),
        box.Top().Rotate(0.00001),
        box.Top().Rotate(-0.00001),
        box.Right(),
        box.Right().Rotate(0.00001),
        box.Right().Rotate(-0.00001),
        box.Bottom(),
        box.Bottom().Rotate(0.00001),
        box.Bottom().Rotate(-0.00001),
        box.Left(),
        box.Left().Rotate(0.00001),
        box.Left().Rotate(-0.00001),
    };
}

std::vector<Line> IsoCollider::GetLines() {
    return std::vector<Line>{
        Line(box.Top(), box.Right()),
        Line(box.Right(), box.Bottom()),
        Line(box.Bottom(), box.Left()),
        Line(box.Left(), box.Top())
    };
}

void IsoCollider::Render() {
#ifdef DEBUG_COLLIDER
    SDL_Point points[5];

    // Bottom
    Vec2 point = box.Bottom() - Camera::pos;
    points[0] = {(int) point.x, (int) point.y};
    points[4] = {(int) point.x, (int) point.y};

    // Right
    point = box.Right() - Camera::pos;
    points[1] = {(int) point.x, (int) point.y};

    // Top
    point = box.Top() - Camera::pos;
    points[2] = {(int) point.x, (int) point.y};

    // Left
    point = box.Left() - Camera::pos;
    points[3] = {(int) point.x, (int) point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoints(Game::GetInstance().GetRenderer(), points, 5);

    // Center
    point = box.Center() - Camera::pos;
    SDL_RenderDrawPoint(Game::GetInstance().GetRenderer(), point.x, point.y);
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
