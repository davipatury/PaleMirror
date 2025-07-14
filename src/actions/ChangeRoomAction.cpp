#include "actions/ChangeRoomAction.h"
#include "rooms/Room.h"

ChangeRoomAction::ChangeRoomAction(State* state, std::string targetRoom, int entryPosIndex) : state(state), targetRoom(targetRoom), entryPosIndex(entryPosIndex) {}

void ChangeRoomAction::Execute() {

    StageState* st = (StageState*) state;
    st->ChangeRoom(targetRoom);

    Room* room = st->GetRoom(targetRoom);
    Vec2 destination = room->entryPos[entryPosIndex];
    Vec2 dest = {destination.x, destination.y};
    Character::player->Issue(Character::Command(Character::Command::TELEPORT, dest.x, dest.y));
}
