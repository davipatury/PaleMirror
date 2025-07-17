#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "core/GameObject.h"
#include "core/Component.h"
#include "utils/Animation.h"
#include "components/SpriteRenderer.h"

class Animator : public Component
{
public:
    Animator(GameObject& associated);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetAnimation(std::string name);
    void AddAnimation(std::string name, Animation anim);
    std::string GetAnimation();
private:
    std::unordered_map<std::string, Animation> animations;
    int frameStart;
    int frameEnd;
    float frameTime;
    int currentFrame;
    float timeElapsed;
    std::string current;
};

#endif // ANIMATOR_H
