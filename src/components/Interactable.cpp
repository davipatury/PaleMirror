#include "components/Interactable.h"
#include "core/GameObject.h"
#include "core/Game.h"
#include "utils/InputManager.h"
#include "physics/Collider.h"
#include "components/Camera.h"
#include "math/Vec2.h"
#include "entities/characters/Character.h"
#include "utils/Text.h"
#include "utils/DrawCircle.h"
#include <string>

// #define DEBUG_INTERACT_RANGE

Interactable::Interactable(GameObject& associated, std::function<void (State*, GameObject*)> a, float ad, SpriteRenderer* sr, Vec2 offset, std::string text, InteractableHUD::InteractableType type): Component(associated),
    action(a), highlightSr(sr), activationDistance(ad), hudOffset(offset), hudText(text), type(type)
{

}

void Interactable::Update(float dt) {
    if (Character::player == nullptr ||
        action == nullptr ||
        associated.IsDead() ||
        activationDistance <= 0.0f
    ) return;

    // Player center coordinates
    IsoCollider* playerCol = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
    Vec2 playerCoord = playerCol->box.Center().ToCart();

    // Object center coordinates
    IsoCollider* objCol = (IsoCollider*) associated.GetComponent("IsoCollider");
    Vec2 objectCoord;
    if (objCol) objectCoord = objCol->box.Center().ToCart();
    else objectCoord = associated.box.Center();

    // Set canInteract
    canInteract = (playerCoord.Distance(objectCoord) <= activationDistance);

    // Highlight sprite renderer
    if (highlightSr != nullptr) highlightSr->SetVisible(canInteract);

    // Execute action
    if (canInteract) {
        if((type == InteractableHUD::INTERACT && INTERACT_CHECK) || (type == InteractableHUD::USE_ITEM && USE_CHECK)) {
            INPUT_MANAGER.ReleaseCButton(SDL_CONTROLLER_BUTTON_A);
            action(&CURRENT_STATE, &associated);
        }
        if (!associated.IsDead()) INTERACTABLE->RecordInteractable(&associated, hudOffset, type, hudText);
    }
}

void Interactable::Render() {
#ifdef DEBUG_INTERACT_RANGE
    if (activationDistance <= 0.0f) return;

    IsoCollider* objCol = (IsoCollider*) associated.GetComponent("IsoCollider");
    Vec2 objectCoord;
    if (objCol) objectCoord = objCol->box.Center().ToCart();
    else objectCoord = associated.box.Center();

    Vec2 cam = Camera::pos;
    int cx = static_cast<int>(objectCoord.x - cam.x);
    int cy = static_cast<int>(objectCoord.y - cam.y);
    int radius = static_cast<int>(activationDistance);

    SDL_SetRenderDrawColor(GAME_RENDERER, 255, 255, 0, 128);
    DrawCircle(GAME_RENDERER, cx, cy, radius);
#endif
}

bool Interactable::Is(std::string type) {
    return (type == "Interactable");
}

void Interactable::SetHUDText(std::string text) {
    this->hudText = text;
}

void Interactable::SetType(InteractableHUD::InteractableType type) {
    this->type = type;
}

void Interactable::SetHUDOffset(Vec2 offset) {
    this->hudOffset = offset;
}

void Interactable::SetAction(std::function<void (State*, GameObject*)> action) {
    this->action = action;
}
