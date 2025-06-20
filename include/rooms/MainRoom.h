#ifndef MAINROOM_H
#define MAINROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class MainRoom : public Room
{
public:
    MainRoom(State* state);

    void Build();
};

#endif // MAINROOM_H
