#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "core/Component.h"
#include "actions/Action.h"
#include <memory>
#include <string>

class GameObject;
class Collider;

class Interactable : public Component {
    public:
        float activationDistance = 100.0f;
        char interactionKey = 'e';
        bool requireMouseOver = false;
        bool canInteract = false;
    private:
        std::unique_ptr<Action> action;
        bool isHighlighted = false;

    public:
        Interactable(GameObject& associated, std::unique_ptr<Action> a): Component(associated), action(std::move(a)){}

        void Update(float dt) override;
        void Render() override;
        bool Is(std::string type) override;

        void SetActivationDistance(float d) { activationDistance = d; }
        void SetInteractionKey(char key) { interactionKey = key; }
        void SetRequireMouseOver(bool r) { requireMouseOver = r; }
    };

#endif
