#include "actions/Actions.h"

std::function<void (State*)> Actions::ChangeRoom(std::string targetRoom, int entryPosIndex) {
    return [targetRoom, entryPosIndex](State* state) {
        StageState* st = (StageState*) state;
        st->ChangeRoom(targetRoom);
        Room* room = st->GetRoom(targetRoom);
        Vec2 destination = room->entryPos[entryPosIndex];
        Vec2 dest = {destination.x, destination.y};
        Character::player->Issue(Character::Command(Character::Command::TELEPORT, dest.x, dest.y));
    };
}

std::function<void (State*)> Actions::Document(const std::string& path) {
    return [path](State* state) {
        // TODO: Create and call DocumentUI
    };
}

std::function<void (State*)> Actions::CollectItem() {
    return [](State* state) {
        // Create inventory system
    };
}
