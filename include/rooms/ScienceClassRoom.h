#ifndef SCIENCECLASSROOM_H
#define SCIENCECLASSROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class ScienceClassRoom : public Room
{
public:
    ScienceClassRoom(State* state);

    void Build();
};

#endif // SCIENCECLASSROOM_H
