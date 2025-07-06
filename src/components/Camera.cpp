#include "components/Camera.h"

Vec2 Camera::pos;
Vec2 Camera::speed;
Rect Camera::cameraLimits;
GameObject* Camera::focus;

void Camera::Follow(GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus != nullptr) {
        Vec2 focusCenter = focus->box.Center();
        pos.x = focusCenter.x - (1200 / 2);
        pos.y = focusCenter.y - (900 / 2);
    } else {
        float xSpeed = 0;
        float ySpeed = 0;
        if (INPUT_MANAGER.IsKeyDown(LEFT_ARROW_KEY))  xSpeed = -250;
        if (INPUT_MANAGER.IsKeyDown(RIGHT_ARROW_KEY)) xSpeed =  250;
        if (INPUT_MANAGER.IsKeyDown(UP_ARROW_KEY))    ySpeed = -250;
        if (INPUT_MANAGER.IsKeyDown(DOWN_ARROW_KEY))  ySpeed =  250;
        pos.x += xSpeed * dt;
        pos.y += ySpeed * dt;
    }

    // Camera limits
    Vec2 tl = cameraLimits.TopLeft();
    Vec2 br = cameraLimits.BottomRight();
    if (cameraLimits.w > 0) {
        if (pos.x < tl.x) pos.x = tl.x;
        if (pos.y < tl.y) pos.y = tl.y;
        if (pos.x > br.x - 1200) pos.x = br.x - 1200;
        if (pos.y > br.y - 900) pos.y = br.y - 900;
    }
}

Rect Camera::PosRect() {
    return Rect(pos.x, pos.y, 1200, 900);
}

void Camera::SetCameraLimits(Rect limits) {
    cameraLimits = limits;
}
