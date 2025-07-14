#ifndef MAINROOM_H
#define MAINROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class MainRoom : public Room
{
public:
    MainRoom(State* state);

    void Build();
};

#endif // MAINROOM_H
