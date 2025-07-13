#ifndef BANHEIROMASCULINOROOM_H
#define BANHEIROMASCULINOROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroMasculinoRoom : public Room
{
public:
    BanheiroMasculinoRoom(State* state);

    void Build();
};

#endif // BANHEIROMASCULINOROOM_H
