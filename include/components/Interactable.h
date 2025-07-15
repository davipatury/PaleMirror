#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "core/Component.h"
#include "components/SpriteRenderer.h"
#include "actions/Actions.h"

#include <string>
#include <functional>

#define DOOR_BACK_INTERACT_DIST 30
#define DOOR_INTERACT_DIST 40

class GameObject;
class Collider;

class Interactable : public Component {
    public:
        float activationDistance = 100.0f;
        bool canInteract = false;
    private:
        std::function<void (State*, GameObject*)> action;
        SpriteRenderer* highlightSr = nullptr;

    public:
        Interactable(GameObject& associated, std::function<void (State*, GameObject*)> a, float ad = 100.0f, SpriteRenderer* sr = nullptr): Component(associated), action(a), highlightSr(sr), activationDistance(ad) {}

        void Update(float dt) override;
        void Render() override;
        bool Is(std::string type) override;

        void SetActivationDistance(float d) { activationDistance = d; }
    };

#endif
