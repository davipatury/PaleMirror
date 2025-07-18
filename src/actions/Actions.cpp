#include "actions/Actions.h"
#include "hud/DocumentHUD.h"

std::function<void (State*, GameObject*)> Actions::ChangeRoom(std::string targetRoom, int entryPosIndex) {
    return [targetRoom, entryPosIndex](State* state, GameObject* associated) {
        StageState* st = (StageState*) state;
        st->ChangeRoom(targetRoom);
        Room* room = st->GetRoom(targetRoom);
        Vec2 destination = room->entryPos[entryPosIndex];
        Vec2 dest = {destination.x, destination.y};
        Character::player->Issue(Character::Command(Character::Command::TELEPORT, dest.x, dest.y));
    };
}

void Actions::Document(const std::string& path) {
    GameObject* uiGO = new GameObject();
    uiGO->AddComponent(new DocumentHUD(*uiGO, path));
    CURRENT_STATE.AddObject(uiGO);
}

std::function<void (State*, GameObject*)> Actions::CollectItem(std::string itemName, int amount, bool deleteSelf) {
    return [itemName, amount, deleteSelf](State* state, GameObject* associated) {
        // Create inventory system
        INVENTORY->Collect(itemName, amount);
        if (deleteSelf) associated->RequestDelete();
    };
}

std::function<void (State*, GameObject*)> Actions::OpenDialog(std::string dialogueKey, std::function<void ()> dialogueEndFunc) {
    return [dialogueKey, dialogueEndFunc](State* state, GameObject* associated) {
        DialogueHUD::RequestDialogue(dialogueKey, dialogueEndFunc);
    };
}
