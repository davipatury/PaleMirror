#include "hud/DebugHUD.h"

#include "states/StageState.h"
#include "core/Game.h"

DebugHUD::DebugHUD(GameObject& associated) : Component(associated)
{
    selectedObject = nullptr;
    text = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 50, TextHUD::BLENDED, "Debug", {255, 255, 255, 255});
    supportText = new TextHUD({0, 50}, "Recursos/font/neodgm.ttf", 40, TextHUD::BLENDED, "DebugInfo", {255, 255, 255, 255});
}

DebugHUD::~DebugHUD() {}

void DebugHUD::Update(float dt) {
    if (INPUT_MANAGER.KeyPress(SDLK_F1)) { debugMode = DEBUG_POS; text->SetText("Pos"); }
    if (INPUT_MANAGER.KeyPress(SDLK_F2)) { debugMode = DEBUG_SCALE; text->SetText("Scale"); }
    if (INPUT_MANAGER.KeyPress(SDLK_F3)) { debugMode = DEBUG_ISO_POS; text->SetText("IsoPos"); }
    if (INPUT_MANAGER.KeyPress(SDLK_F4)) { debugMode = DEBUG_ISO_SCALE; text->SetText("IsoScale"); }
    if (INPUT_MANAGER.KeyPress(SDLK_F5)) { debugMode = DEBUG_PLAYER_POS; text->SetText("PlayerPos"); }
    if (INPUT_MANAGER.KeyPress(SDLK_F6)) { debugMode = DEBUG_DISABLE; selectedObject = nullptr; }

    if (debugMode == DEBUG_DISABLE) return;

    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        Vec2 mousePos = INPUT_MANAGER.GetMousePos();
        selectedObject = nullptr;
        for (int i = 0; i < CURRENT_STATE.objectArray.size(); i++) {
            GameObject* go = CURRENT_STATE.objectArray[i].get();
            Rect screenRect = Rect(go->box.x - Camera::pos.x, go->box.y - Camera::pos.y, go->box.w, go->box.h);
            if (go->box.z == 0 && screenRect.Contains(mousePos)) {
                selectedObject = go;
            }
        }
    }

    if (selectedObject) {
        // Pos debug
        if (debugMode == DEBUG_POS) {
            if (LEFT_CHECK || INPUT_MANAGER.IsKeyDown(LEFT_ARROW_KEY)) selectedObject->box.x--;
            if (RIGHT_CHECK || INPUT_MANAGER.IsKeyDown(RIGHT_ARROW_KEY)) selectedObject->box.x++;
            if (UP_CHECK || INPUT_MANAGER.IsKeyDown(UP_ARROW_KEY)) selectedObject->box.y--;
            if (DOWN_CHECK || INPUT_MANAGER.IsKeyDown(DOWN_ARROW_KEY)) selectedObject->box.y++;
            StageState* state = (StageState*) &CURRENT_STATE;
            support = selectedObject->box.TopLeft().Sub(state->GetCurrentRoom()->basePos).ToStr();
        }

        else if (debugMode == DEBUG_SCALE) {
            SpriteRenderer* sr = (SpriteRenderer*) selectedObject->GetComponent("SpriteRenderer");
            if (sr) {
                Vec2 scale = sr->sprite.GetScale();
                if (LEFT_CHECK || INPUT_MANAGER.IsKeyDown(LEFT_ARROW_KEY)) sr->SetScale(scale.x - 0.05, scale.y);
                if (RIGHT_CHECK || INPUT_MANAGER.IsKeyDown(RIGHT_ARROW_KEY)) sr->SetScale(scale.x + 0.05, scale.y);
                if (UP_CHECK || INPUT_MANAGER.IsKeyDown(UP_ARROW_KEY)) sr->SetScale(scale.x, scale.y - 0.05);
                if (DOWN_CHECK || INPUT_MANAGER.IsKeyDown(DOWN_ARROW_KEY)) sr->SetScale(scale.x, scale.y + 0.05);
                if (INPUT_MANAGER.IsKeyDown(PLUS_KEY)) sr->SetScale(scale.x + 0.05, scale.y + 0.05);
                if (INPUT_MANAGER.IsKeyDown(MINUS_KEY)) sr->SetScale(scale.x - 0.05, scale.y - 0.05);
                support = sr->sprite.GetScale().ToStr();
            }
        }

        else if (debugMode == DEBUG_ISO_POS) {
            IsoCollider* iso = (IsoCollider*) selectedObject->GetComponent("IsoCollider");
            if (iso) {
                Vec2 offset = iso->GetOffset();
                if (LEFT_CHECK || INPUT_MANAGER.IsKeyDown(LEFT_ARROW_KEY)) iso->SetOffset(offset.Add({-1, 0}));
                if (RIGHT_CHECK || INPUT_MANAGER.IsKeyDown(RIGHT_ARROW_KEY)) iso->SetOffset(offset.Add({1, 0}));
                if (UP_CHECK || INPUT_MANAGER.IsKeyDown(UP_ARROW_KEY)) iso->SetOffset(offset.Add({0, -1}));
                if (DOWN_CHECK || INPUT_MANAGER.IsKeyDown(DOWN_ARROW_KEY)) iso->SetOffset(offset.Add({0, 1}));
                support = iso->GetOffset().ToStr();
            }
        }

        else if (debugMode == DEBUG_ISO_SCALE) {
            IsoCollider* iso = (IsoCollider*) selectedObject->GetComponent("IsoCollider");
            if (iso) {
                Vec2 scale = iso->GetScale();
                if (LEFT_CHECK || INPUT_MANAGER.IsKeyDown(LEFT_ARROW_KEY)) iso->SetScale(scale.Add({-0.05, 0}));
                if (RIGHT_CHECK || INPUT_MANAGER.IsKeyDown(RIGHT_ARROW_KEY)) iso->SetScale(scale.Add({0.05, 0}));
                if (UP_CHECK || INPUT_MANAGER.IsKeyDown(UP_ARROW_KEY)) iso->SetScale(scale.Add({0, -0.05}));
                if (DOWN_CHECK || INPUT_MANAGER.IsKeyDown(DOWN_ARROW_KEY)) iso->SetScale(scale.Add({0, 0.05}));
                if (INPUT_MANAGER.IsKeyDown(PLUS_KEY)) iso->SetScale(scale.Add({0.05, 0.05}));
                if (INPUT_MANAGER.IsKeyDown(MINUS_KEY)) iso->SetScale(scale.Add({-0.05, -0.05}));
                support = iso->GetScale().ToStr();
            }
        }
    }

    if (debugMode == DEBUG_PLAYER_POS) {
        Vec2 playerPos = PLAYER ? PLAYER->associated.box.TopLeft() : Vec2{0, 0};
        support = playerPos.ToStr();
    }

    supportText->SetText(support);
}

void DebugHUD::Render() {
    if (debugMode == DEBUG_DISABLE) return;
    text->Render();

    if (selectedObject) {
        // Highlight rect
        SDL_Rect screenRect = Rect(selectedObject->box.x - Camera::pos.x, selectedObject->box.y - Camera::pos.y, selectedObject->box.w, selectedObject->box.h).ToSDLRect();
        SDL_SetRenderDrawColor(GAME_RENDERER, 0, 255, 0, 255);
        SDL_RenderDrawRect(GAME_RENDERER, &screenRect);
    }

    if (selectedObject || debugMode == DEBUG_PLAYER_POS) {
        // Extra info
        supportText->Render();
    }
}

bool DebugHUD::Is(std::string type) {
    return type == "DebugHUD";
}
