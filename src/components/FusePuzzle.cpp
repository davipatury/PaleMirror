#include "components/FusePuzzle.h"

#define FUSE_PUZZLE_RECT_X 260
#define FUSE_PUZZLE_RECT_Y 210

FusePuzzle::FusePuzzle(GameObject& associated) : Component(associated), bg("Recursos/img/fuse_puzzle/fuseboxbg.png"){
    for(int i=0; i<9; i++) fuses.push_back(Fuse());
    bg.SetCameraFollower(true);
    std::cout << "FusePuzzle()\n";
}

void FusePuzzle::FusePressed(int idx){
    std::vector<int> adjacentFuses = {idx};
    if(idx%3 > 0) adjacentFuses.push_back(idx - 1);
    if(idx%3 < 2) adjacentFuses.push_back(idx + 1);
    if(idx/3 > 0) adjacentFuses.push_back(idx - 3);
    if(idx/3 < 2) adjacentFuses.push_back(idx + 3);

    for(auto i : adjacentFuses){
        fuses[i].on = !fuses[i].on;
        fuses[i].sprite.SetFrame(fuses[i].on);
    }
}

void FusePuzzle::Update(float dt) {
    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        Vec2 mousePos = {INPUT_MANAGER.GetMouseX(), INPUT_MANAGER.GetMouseY()};
        for (int i = 0; i < fuses.size(); i++) {
            Rect fuseRect = fuses[i].GetRect().Add(Vec2{FUSE_PUZZLE_RECT_X, FUSE_PUZZLE_RECT_Y});
            if (fuseRect.Contains(mousePos)) {
                FusePressed(i);
            }
        }
    }

    if (INPUT_MANAGER.IsKeyDown(SDLK_ESCAPE)) {
        Game::GetInstance().GetCurrentState().openUI = false;
        associated.pauseOnOpenUI=true;
        associated.RequestDelete();
        std::cout << "FuzePuzzle closed" << std::endl;
        INPUT_MANAGER.ReleaseKey(SDLK_ESCAPE);
    }

    if (IsSolved()) {
        std::cout << "!!! SOLVED !!!" << std::endl;
        Game::GetInstance().GetCurrentState().openUI = false;
        associated.pauseOnOpenUI=true;
        associated.RequestDelete();
    }
}

void FusePuzzle::Render() {
    SDL_Rect bgRect;
    bgRect.x = FUSE_PUZZLE_RECT_X;
    bgRect.y = FUSE_PUZZLE_RECT_Y;
    bgRect.w = 678;
    bgRect.h = 480;

    // Background rectangle
    bg.Render(bgRect.x, bgRect.y, bgRect.w, bgRect.h);

    // Pieces
    for (int i = 0; i < fuses.size(); i++) {
        float x = fuses[i].pos.x + bgRect.x;
        float y = fuses[i].pos.y + bgRect.y;
        fuses[i].sprite.Render(x, y, fuses[i].GetWidth(), fuses[i].GetHeight());
    }
}

void FusePuzzle::Start() {
    Game::GetInstance().GetCurrentState().openUI = true;
    associated.pauseOnOpenUI=false;
    bool always_on = true;

    for (int i = 0; i < fuses.size(); i++) {
        fuses[i].pos = {376 + (i%3 * 96), 90 + (i/3 * 126)};
        bool on = (rand() % 2);
        always_on &= on;
        if(i == fuses.size()-1 && always_on) on = 0;
        fuses[i].on = on;
        fuses[i].sprite.SetFrame(on);
    }
}

bool FusePuzzle::IsSolved() {
    for (int i = 0; i < fuses.size(); i++) {
        if (!fuses[i].on) {
            return false;
        }
    }
    return true;
}

bool FusePuzzle::Is(std::string type) {
    return type == "FusePuzzle";
}
