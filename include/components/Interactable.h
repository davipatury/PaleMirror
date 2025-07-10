#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "core/Component.h"
#include "components/SpriteRenderer.h"
#include "actions/Action.h"
#include <memory>
#include <string>

#define DOOR_INTERACTION_DISTANCE 40

class GameObject;
class Collider;

class Interactable : public Component {
    public:
        float activationDistance = 100.0f;
        bool canInteract = false;
    private:
        std::unique_ptr<Action> action;
        SpriteRenderer* highlightSr = nullptr;

    public:
        Interactable(GameObject& associated, std::unique_ptr<Action> a, SpriteRenderer* sr = nullptr): Component(associated), action(std::move(a)), highlightSr(sr) {}

        void Update(float dt) override;
        void Render() override;
        bool Is(std::string type) override;

        void SetActivationDistance(float d) { activationDistance = d; }
    };

#endif
