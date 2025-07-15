#ifndef BANHEIROINTROROOM_H
#define BANHEIROINTROROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroIntroRoom : public Room
{
public:
    BanheiroIntroRoom(State* state);

    void Build();
};

#endif // BANHEIROINTROROOM_H
