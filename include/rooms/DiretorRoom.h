#ifndef DIRETORROOM_H
#define DIRETORROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class DiretorRoom : public Room
{
public:
    DiretorRoom(State* state);

    void Build();
};

#endif // DIRETORROOM_H
