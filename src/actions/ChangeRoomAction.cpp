#include "actions/ChangeRoomAction.h"
#include "rooms/Room.h"

ChangeRoomAction::ChangeRoomAction(State* state, std::string targetRoom, int entryPosIndex) : state(state), targetRoom(targetRoom), entryPosIndex(entryPosIndex) {}

void ChangeRoomAction::Execute() {

    StageState* st = (StageState*) state;
    st->ChangeRoom(targetRoom);
    GameObject& playerGO = Character::player->associated;

    float halfW = playerGO.box.w / 2.0f;
    float halfH = playerGO.box.h / 2.0f;

    Room* room = st->GetRoom(targetRoom);
    Vec2 destination = room->entryPos[entryPosIndex];
    Vec2 dest = {destination.x - halfW, destination.y - halfH};
    Character::player->Issue(Character::Command(Character::Command::TELEPORT, dest.x, dest.y));
}

std::unique_ptr<Action> ChangeRoomAction::Clone() const {
    return std::unique_ptr<Action>(new ChangeRoomAction(state, targetRoom));
}
