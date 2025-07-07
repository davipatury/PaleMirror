#include "physics/Collider.h"

//#define DEBUG_COLLIDER

#include <math.h>
#include "SDL_image.h"
#include "components/Camera.h"
#include "core/Game.h"

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) : Component(associated) {
    this->scale = scale;
    this->offset = offset;
}

void Collider::Update(float dt) {
    box = associated.box;
    box.w = box.w * scale.x;
    box.h = box.h * scale.y;
    box = box.Add(associated.box.Center().Add(offset).Sub(box.Center()));
}

void Collider::Render() {
#ifdef DEBUG_COLLIDER
    Vec2 center( box.Center() );
    SDL_Point points[5];

    Vec2 point = (Vec2(box.x, box.y) - center).Rotate( associated.angleDeg/(180/M_PI) )
                 + center - Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y) - center).Rotate( associated.angleDeg/(180/M_PI) )
            + center - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y + box.h) - center).Rotate( associated.angleDeg/(180/M_PI) )
            + center - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x, box.y + box.h) - center).Rotate( associated.angleDeg/(180/M_PI) )
            + center - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(GAME_RENDERER, points, 5);
#endif // DEBUG
}

bool Collider::Is(std::string type) {
    return type == "Collider";
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}
