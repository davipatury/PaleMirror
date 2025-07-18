#ifndef PROFESSORESROOM_H
#define PROFESSORESROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class ProfessoresRoom : public Room
{
public:
    ProfessoresRoom(State* state);

    void Build();
};

#endif // PROFESSORESROOM_H
