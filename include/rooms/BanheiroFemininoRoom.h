#ifndef BANHEIROFEMININOROOM_H
#define BANHEIROFEMININOROOM_H

#include "rooms/Room.h"
#include "actions/Actions.h"
#include "components/Interactable.h"
#include "core/State.h"
#include "components/RunaRitual.h"
#include "components/LightEmitter.h"

class BanheiroFemininoRoom : public Room
{
public:
    BanheiroFemininoRoom(State* state);

    void Build();
};

#endif // BANHEIROFEMININOROOM_H
