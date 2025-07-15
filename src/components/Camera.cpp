#include "components/Camera.h"

#include "core/Game.h"

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
        pos.x = focusCenter.x - (WINDOW_WIDTH * 0.5);
        pos.y = focusCenter.y - (WINDOW_HEIGHT * 0.5);
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
        if (pos.x > br.x - WINDOW_WIDTH) pos.x = br.x - WINDOW_WIDTH;
        if (pos.y > br.y - WINDOW_HEIGHT) pos.y = br.y - WINDOW_HEIGHT;
    }
}

Rect Camera::PosRect() {
    return Rect(pos.x, pos.y, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Camera::SetCameraLimits(Rect limits) {
    cameraLimits = limits;
}
