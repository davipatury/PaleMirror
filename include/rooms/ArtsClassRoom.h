#ifndef ARTSCLASSROOM_H
#define ARTSCLASSROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class ArtsClassRoom : public Room
{
public:
    ArtsClassRoom(State* state);

    void Build();
};

#endif // ARTSCLASSROOM_H
