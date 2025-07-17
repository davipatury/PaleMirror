#include "components/LightEmitter.h"

#include "core/Game.h"

LightEmitter::LightEmitter(GameObject &associated, std::vector<LightPoint> lightPoints) : Component(associated) {
    this->lightPoints = lightPoints;
}

void LightEmitter::Update(float dt) { }

void LightEmitter::Start() { }

void LightEmitter::Render() { }

bool LightEmitter::IsEnabled(int pointIndex) {
    return lightPoints.size() > pointIndex ? lightPoints[pointIndex].enabled : false;
}

bool LightEmitter::HasEnabledPoint() {
    for (int i = 0; i < lightPoints.size(); i++) {
        if (lightPoints[i].enabled) return true;
    }
    return false;
}

void LightEmitter::SetEnabled(int pointIndex, bool enabled) {
    if (lightPoints.size() <= pointIndex) return;
    lightPoints[pointIndex].enabled = enabled;
}

void LightEmitter::SetEnabledAll(bool enabled) {
    for (int i = 0; i < lightPoints.size(); i++) {
        lightPoints[i].enabled = enabled;
    }
}

void LightEmitter::SetScale(int pointIndex, float scaleX, float scaleY) {
    if (lightPoints.size() <= pointIndex) return;
    lightPoints[pointIndex].scale = {scaleX, scaleY};
}

void LightEmitter::SetScaleAll(float scaleX, float scaleY) {
    for (int i = 0; i < lightPoints.size(); i++) {
        lightPoints[i].scale = {scaleX, scaleY};
    }
}

void LightEmitter::SetOffset(int pointIndex, Vec2 offset) {
    if (lightPoints.size() <= pointIndex) return;
    lightPoints[pointIndex].offset = offset;
}

void LightEmitter::SetOffsetAll(Vec2 offset) {
    for (int i = 0; i < lightPoints.size(); i++) {
        lightPoints[i].offset = offset;
    }
}

Vec2 LightEmitter::GetPos(int pointIndex, bool minusCamera) {
    Vec2 pos = associated.box.TopLeft() + lightPoints[pointIndex].offset;
    return minusCamera ? pos - Camera::pos : pos;
}

Vec2 LightEmitter::GetOffset(int pointIndex) {
    return lightPoints[pointIndex].offset;
}

Vec2 LightEmitter::GetScale(int pointIndex) {
    return lightPoints[pointIndex].scale;
}

Sprite LightEmitter::GetSprite(int pointIndex) {
    return lightPoints[pointIndex].sprite;
}

bool LightEmitter::Is(std::string type) {
    return type == "LightEmitter";
}

// LightPoint
LightEmitter::LightPoint::LightPoint(Vec2 o, Vec2 s, bool e, const char* f) : offset(o), scale(s), enabled(e), sprite(f, 1, 1, true) {
    SDL_SetTextureBlendMode(sprite.texture, SDL_BLENDMODE_BLEND);
    sprite.SetScale(s.x, s.y);
}
