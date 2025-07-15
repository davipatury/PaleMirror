#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "core/Component.h"
#include "components/SpriteRenderer.h"
#include "actions/Actions.h"
#include "hud/InteractableHUD.h"

#include <string>
#include <functional>

#define DOOR_BACK_INTERACT_DIST 30
#define DOOR_INTERACT_DIST 40
#define ITEM_COLLECT_DIST 30

class GameObject;
class Collider;

class Interactable : public Component {
    public:
        float activationDistance = 100.0f;
        bool canInteract = false;
    private:
        std::function<void (State*, GameObject*)> action;
        SpriteRenderer* highlightSr = nullptr;
        Vec2 hudOffset;
        std::string hudText;
        InteractableHUD::InteractableType type;
    public:
        Interactable(
            GameObject& associated,
            std::function<void (State*, GameObject*)> a,
            float ad = 100.0f,
            SpriteRenderer* sr = nullptr,
            Vec2 offset = {-20, -15},
            std::string text = "Entrar",
            InteractableHUD::InteractableType type = InteractableHUD::INTERACT
        );

        void Update(float dt) override;
        void Render() override;
        bool Is(std::string type) override;

        void SetHUDText(std::string text);
        void SetType(InteractableHUD::InteractableType type);
        void SetHUDOffset(Vec2 offset);

        void SetActivationDistance(float d) { activationDistance = d; }
    };

#endif
