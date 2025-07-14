#ifndef HISTORYCLASSROOM_H
#define HISTORYCLASSROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class HistoryClassRoom : public Room
{
public:
    HistoryClassRoom(State* state);

    void Build();
};

#endif // HISTORYCLASSROOM_H
