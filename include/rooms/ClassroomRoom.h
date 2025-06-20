#ifndef CLASSROOMROOM_H
#define CLASSROOMROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class ClassroomRoom : public Room
{
public:
    ClassroomRoom(State* state);

    void Build();
};

#endif // CLASSROOMROOM_H
