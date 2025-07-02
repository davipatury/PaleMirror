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

void Interactable::Update(float dt) {
    if (Character::player == nullptr) return;

    IsoCollider* playerCol = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
    Vec2 playercoord = playerCol->box.Center().ToCart();
    IsoCollider* col = (IsoCollider*) associated.GetComponent("IsoCollider");

    Vec2 objectcoord;
    float dist2;
    if(col){
        objectcoord = col->box.Center().ToCart();
        IsoCollider* colplay = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
        dist2 = colplay->box.Center().ToCart().Distance(objectcoord);
    }else{
        objectcoord = associated.box.Center();
        dist2 = playercoord.Distance(objectcoord);
    }

    canInteract = (dist2 <= activationDistance);

    bool mouseOverOK = false;
    if (requireMouseOver) {
        Collider* col = (Collider*) associated.GetComponent("Collider");
        if (col) {
            /*
            auto mousePos = InputManager::GetInstance().GetMousePosition();
            mouseOverOK = col->IsPointInside(mousePos.x, mousePos.y);
            */
        } else {
            mouseOverOK = false;
        }
    }

    bool can = (canInteract || mouseOverOK);
    /* Highlight
    if (can && !isHighlighted) {
        SpriteRenderer* spr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
        if (spr) spr->SetColor(1, 1, 0.5, 1);
        isHighlighted = true;
    } else if (!can && isHighlighted) {
        SpriteRenderer* spr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
        if (spr) spr->SetColor(1, 1, 1, 1);
        isHighlighted = false;
    }
    */

    if (highlightSr != nullptr) {
        highlightSr->SetVisible(can);
    }
    if (can && InputManager::GetInstance().KeyPress(interactionKey)) {
        if(action){
            // Cria uma cópia do Action e executa
            //std::unique_ptr<Action> actionCopy = action->Clone();
            action->Execute();
        }
    }
}

void Interactable::Render() {
    // Mostrar alguma coisa na tela
    static GameObject* textGO     = nullptr;
    static Text* promptText = nullptr;

    if (!canInteract) {
        if (textGO) {
            delete textGO;
            textGO = nullptr;
            promptText = nullptr;
        }
        return;
    }

    if (!textGO) {
        textGO = new GameObject();

        textGO->box = Rect(0, 0, 0, 0);
        const char* fontFile = "Recursos/font/PixelifySans-Regular.ttf";
        int fontSize = 45;
        Text::TextStyle style = Text::BLENDED;
        std::string key = {toupper(interactionKey)};
        std::string msg = "Pressione \"" + key + "\" para Interagir";
        SDL_Color color = { 255, 255, 255, 255 };
        float flash = 0.0f;
        promptText = new Text(*textGO, fontFile, fontSize, style, msg, color, flash);
        textGO->AddComponent(promptText);
    }
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    int screenW = 0, screenH = 0;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
    int marginX = 10;
    int marginY = 10;

    int textW = textGO->box.w;
    int textH = textGO->box.h;

    textGO->box.x = static_cast<int>(Camera::pos.x) + (screenW - textW) - marginX;
    textGO->box.y = static_cast<int>(Camera::pos.y) + (screenH - textH) - marginY;
    promptText->Render();

    Vec2 objectcoord;
    IsoCollider* col = (IsoCollider*) associated.GetComponent("IsoCollider");
    if (col) objectcoord = col->box.Center().ToCart();
    else objectcoord = associated.box.Center();

    Vec2 cam = Camera::pos;
    int cx = static_cast<int>(objectcoord.x - cam.x);
    int cy = static_cast<int>(objectcoord.y - cam.y);
    int radius = static_cast<int>(activationDistance);

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 0, 128);
    DrawCircle(Game::GetInstance().GetRenderer(), cx, cy, radius);
}


bool Interactable::Is(std::string type) {
    return (type == "Interactable");
}
