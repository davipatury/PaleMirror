#include "components/Interactable.h"
#include "core/GameObject.h"
#include "core/Game.h"
#include "utils/InputManager.h"
#include "physics/Collider.h"
#include "components/Camera.h"
#include "hud/InteractableHUD.h"
#include "math/Vec2.h"
#include "entities/characters/Character.h"
#include "utils/Text.h"
#include "utils/DrawCircle.h"
#include <string>

#define DEBUG_INTERACT_RANGE

void Interactable::Update(float dt) {
    if (Character::player == nullptr) return;

    IsoCollider* playerCol = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
    Vec2 playerCoord = playerCol->box.Center().ToCart();

    IsoCollider* objCol = (IsoCollider*) associated.GetComponent("IsoCollider");
    Vec2 objectCoord;
    if (objCol) objectCoord = objCol->box.Center().ToCart();
    else objectCoord = associated.box.Center();
    float dist2 = playerCoord.Distance(objectCoord);

    canInteract = (dist2 <= activationDistance);

    if (requireMouseOver) {
        /*
        if (!mouseInsideObject) {
            canInteract = false;
        }
        */
    }

    if (highlightSr != nullptr) {
        highlightSr->SetVisible(canInteract);
    }

    if (canInteract) {
        InteractableHUD::RecordInteractable();
        if(action && (INPUT_MANAGER.KeyPress(interactionKey) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_A))){
            // Cria uma cópia do Action e executa
            //std::unique_ptr<Action> actionCopy = action->Clone();
            action->Execute();
        }
    }
}

void Interactable::Render() {
#ifdef DEBUG_INTERACT_RANGE
    if (!canInteract) return;

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
