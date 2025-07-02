#ifndef PORTUGUESECLASSROOM_H
#define PORTUGUESECLASSROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class PortugueseClassRoom : public Room
{
public:
    PortugueseClassRoom(State* state);

    void Build();
};

#endif // PORTUGUESECLASSROOM_H
