#include "Camera.h"

Vec2 Camera::pos;
Vec2 Camera::speed;
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
        if (InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY))  xSpeed = -250;
        if (InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY)) xSpeed =  250;
        if (InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY))    ySpeed = -250;
        if (InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY))  ySpeed =  250;
        pos.x += xSpeed * dt;
        pos.y += ySpeed * dt;
    }
}
