#ifndef BANHEIRODEMROOM_H
#define BANHEIRODEMROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroDemRoom : public Room
{
public:
    BanheiroDemRoom(State* state);

    void Build();
};

#endif // BANHEIRODEM_H
