#include "core/Game.h"
#include "states/TitleState.h"
#include "utils/Resources.h"

int main (int argc, char** argv) {
    Game& game = Game::GetInstance();
    game.Push(new TitleState());
    game.Run();
    return 0;
}
