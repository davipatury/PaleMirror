#ifndef BANHEIROMASCULINOROOM_H
#define BANHEIROMASCULINOROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroMasculinoRoom : public Room
{
public:
    BanheiroMasculinoRoom(State* state);

    void Build();
};

#endif // BANHEIROMASCULINOROOM_H
