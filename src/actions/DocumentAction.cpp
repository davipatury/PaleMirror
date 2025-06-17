#include "actions/DocumentAction.h"
#include "actions/Action.h"
#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "components/SpriteRenderer.h"
#include "core/Game.h"


DocumentAction::DocumentAction(const std::string& path): documentPath(path), imageVisible(false) {}

void DocumentAction::Execute() {
    imageVisible = !imageVisible;
    if (imageVisible) {
        imageObject = new GameObject();
        SpriteRenderer* imgSprite = new SpriteRenderer(*imageObject, documentPath.c_str());
        imageObject->AddComponent(imgSprite);
        imageObject->box.x = Camera::pos.x + (Camera::PosRect().w/2) - (imgSprite->associated.box.w/2);
        imageObject->box.y = Camera::pos.y + (Camera::PosRect().h/2) - (imgSprite->associated.box.h/2);
        imageObject->box.z = 5;

        Game::GetInstance().GetCurrentState().AddObject(imageObject);
    } else {
        if (imageObject) {
            imageObject->RequestDelete();
            imageObject = nullptr;
        }
    }
}

std::unique_ptr<Action> DocumentAction::Clone() const {
    return std::unique_ptr<Action>(new DocumentAction(documentPath));
}
