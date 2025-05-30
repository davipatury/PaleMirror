#include "components/Animator.h"

Animator::Animator(GameObject& associated) : Component(associated) {
    frameStart = 0;
    frameEnd = 0;
    frameTime = 0;
    currentFrame = 0;
    timeElapsed = 0;
}

void Animator::Update(float dt) {
    if (frameTime != 0) {
        timeElapsed += dt;
        if (timeElapsed > frameTime) {
            currentFrame++;
            timeElapsed -= frameTime;
            if (currentFrame > frameEnd) {
                currentFrame = frameStart;
            }
            SpriteRenderer* sprite = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
            Animation anim = animations.at(current);
            sprite->SetFrame(currentFrame, anim.flip);
        }
    }
}

void Animator::Render() { }

void Animator::SetAnimation(std::string name) {
    if (name == current) return;
    if (animations.find(name) != animations.end()) {
        Animation anim = animations.at(name);
        frameStart = anim.frameStart;
        frameEnd = anim.frameEnd;
        frameTime = anim.frameTime;
        currentFrame = frameStart;
        current = name;
        timeElapsed = 0;
        SpriteRenderer* sprite = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
        sprite->SetFrame(currentFrame, anim.flip);
    }
}

void Animator::AddAnimation(std::string name, Animation anim) {
    if (animations.find(name) == animations.end()) {
        animations.insert({name, anim});
    }
}

bool Animator::Is(std::string type) {
    return type == "Animator";
}
