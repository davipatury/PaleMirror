#ifndef BANHEIROFEMININOROOM_H
#define BANHEIROFEMININOROOM_H

#include "rooms/Room.h"
#include "actions/Action.h"
#include "actions/ChangeRoomAction.h"
#include "components/Interactable.h"
#include "core/State.h"

class BanheiroFemininoRoom : public Room
{
public:
    BanheiroFemininoRoom(State* state);

    void Build();
};

#endif // BANHEIROFEMININOROOM_H
