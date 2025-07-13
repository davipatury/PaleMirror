#include "core/Sprite.h"
#include "core/Game.h"

Sprite::Sprite() {
    texture = nullptr;
    frameCountW = 1;
    frameCountH = 1;
    cameraFollower = false;
    SetScale(1, 1);
    flip = SDL_FLIP_NONE;
}

Sprite::Sprite(const char* file, int frameCountW, int frameCountH, bool cameraFollower) {
    texture = nullptr;
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
    this->cameraFollower = cameraFollower;
    SetScale(1, 1);
    flip = SDL_FLIP_NONE;
    Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(const char* file) {
    texture = Resources::GetImage(file);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SetFrame(0);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::Render(int x, int y, int w, int h, float angle) {
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    if (!cameraFollower) {
        dstrect.x -= Camera::pos.x;
        dstrect.y -= Camera::pos.y;
    }
    dstrect.w = w;
    dstrect.h = h;
    SDL_RenderCopyEx(GAME_RENDERER, texture, &clipRect, &dstrect, angle, nullptr, flip);
}

void Sprite::SetFrame(int frame) {
    int row    = frame / frameCountW;
    int column = frame % frameCountW;
    int frameW = width  / frameCountW;
    int frameH = height / frameCountH;
    int x = column * frameW;
    int y = row * frameH;
    SetClip(x, y, frameW, frameH);
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
}

void Sprite::SetCameraFollower(bool newValue) {
    cameraFollower = newValue;
}

void Sprite::SetFlip(SDL_RendererFlip flip) {
    this->flip = flip;
}

void Sprite::SetScale(float scaleX, float scaleY) {
    if (scaleX != 0) scale.x = scaleX;
    if (scaleY != 0) scale.y = scaleY;
}

Vec2 Sprite::GetScale() {
    return scale;
}

int Sprite::GetWidth() {
    return (width * scale.x) / frameCountW;
}

int Sprite::GetHeight() {
    return (height * scale.y) / frameCountH;
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}
