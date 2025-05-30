#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <iostream>
#include <string>

#include "utils/Animation.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "core/Sprite.h"

class SpriteRenderer : public Component
{
public:
    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, const char* file, int frameCountW = 1, int frameCountH = 1);

    void Open(const char* file);
    void SetFrameCount(int frameCountW, int frameCountH);
    void Update(float dt);
    void Render();
    void SetFrame(int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void SetCameraFollower(bool newValue);

    void SetScale(float scaleX, float scaleY);

    bool Is(std::string type);

    Sprite sprite;
};

#endif // SPRITERENDERER_H
