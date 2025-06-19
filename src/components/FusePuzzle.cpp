#include "components/FusePuzzle.h"

#define FUSE_PUZZLE_RECT_X 475
#define FUSE_PUZZLE_RECT_Y 100
#define DISTANCE_THRESHOLD 20

FusePuzzle::FusePuzzle(GameObject& associated, std::vector<Fuse> fuses) : Component(associated), bg("Recursos/img/fuse_puzzle/fuseboxbg.png"), fuses(fuses) {
}

void FusePuzzle::Update(float dt) {
    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        Vec2 mousePos = {INPUT_MANAGER.GetMouseX(), INPUT_MANAGER.GetMouseY()};
        for (int i = 0; i < fuses.size(); i++) {
            Rect fuseRect = fuses[i].GetRect().Add(Vec2{FUSE_PUZZLE_RECT_X, FUSE_PUZZLE_RECT_Y});
            if (fuseRect.Contains(mousePos)) {
                fuses[i].on = !fuses[i].on;
                fuses[i].sprite.SetFrame(fuses[i].on);
            }
        }
    }

    if (IsSolved()) {
        std::cout << "!!! SOLVED !!!" << std::endl;
    }
}

void FusePuzzle::Render() {
    SDL_Rect bgRect;
    bgRect.x = FUSE_PUZZLE_RECT_X;
    bgRect.y = FUSE_PUZZLE_RECT_Y;
    bgRect.w = 250;
    bgRect.h = 500;

    // Background rectangle
    bg.Render(bgRect.x, bgRect.y, bgRect.w, bgRect.w, bgRect.h);

    // Pieces
    for (int i = 0; i < fuses.size(); i++) {
        float x = fuses[i].pos.x + bgRect.x;
        float y = fuses[i].pos.y + bgRect.y;
        fuses[i].sprite.Render(x, y, fuses[i].GetWidth(), fuses[i].GetHeight());
    }
}

void FusePuzzle::Start() {
    for (int i = 0; i < fuses.size(); i++) {
        fuses[i].pos = {i%3 * 50 + 10, i/3 * 50 + 10};
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
