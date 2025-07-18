#ifndef BANHEIRODEM_H
#define BANHEIRODEM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroDem : public Room
{
public:
    BanheiroDem(State* state);

    void Build();
};

#endif // BANHEIRODEM_H
