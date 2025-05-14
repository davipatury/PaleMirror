#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Component.h"
#include "Animation.h"
#include "SpriteRenderer.h"

class Animator : public Component
{
public:
    Animator(GameObject& associated);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetAnimation(std::string name);
    void AddAnimation(std::string name, Animation anim);
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
