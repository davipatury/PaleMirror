#include "components/SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(GameObject& associated) : Component(associated), sprite() {}

SpriteRenderer::SpriteRenderer(GameObject& associated, const char* file, int frameCountW, int frameCountH) : Component(associated), sprite(file, frameCountW, frameCountH) {
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Open(const char* file) {
    sprite.Open(file);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH) {
    sprite.SetFrameCount(frameCountW, frameCountH);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Update(float dt) { }

void SpriteRenderer::Render() {
    if (!visible) return;
    sprite.Render(associated.box.x, associated.box.y, associated.box.w, associated.box.h, associated.angleDeg);
}

void SpriteRenderer::SetFrame(int frame, SDL_RendererFlip flip) {
    sprite.SetFrame(frame);
    sprite.SetFlip(flip);
}

void SpriteRenderer::SetCameraFollower(bool newValue) {
    sprite.SetCameraFollower(newValue);
}

void SpriteRenderer::SetScale(float scaleX, float scaleY) {
    sprite.SetScale(scaleX, scaleY);
    Vec2 oldCenter = associated.box.Center();
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();

    associated.box = associated.box.Add(oldCenter.Sub(associated.box.Center()));
}

void SpriteRenderer::SetVisible(bool visible) {
    this->visible = visible;
}

bool SpriteRenderer::Is(std::string type) {
    return type == "SpriteRenderer";
}
