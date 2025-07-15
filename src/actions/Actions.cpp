#include "actions/Actions.h"

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

std::function<void (State*, GameObject*)> Actions::Document(const std::string& path) {
    return [path](State* state, GameObject* associated) {
        // TODO: Create and call DocumentUI
    };
}

std::function<void (State*, GameObject*)> Actions::CollectItem(std::string itemName, int amount, bool deleteSelf) {
    return [itemName, amount, deleteSelf](State* state, GameObject* associated) {
        // Create inventory system
        INVENTORY->Collect(itemName, amount);
        if (deleteSelf) associated->RequestDelete();
    };
}
