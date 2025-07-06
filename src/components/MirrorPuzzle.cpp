#include "components/MirrorPuzzle.h"
#include "core/Game.h"

#define MIRROR_PUZZLE_RECT_X 475
#define MIRROR_PUZZLE_RECT_Y 100
#define DISTANCE_THRESHOLD 20

MirrorPuzzle::MirrorPuzzle(GameObject& associated, std::vector<Piece> pieces) : Component(associated), bg("Recursos/img/mirror_puzzle/Azulejos.png") {
    this->pieces = pieces;
    bg.SetCameraFollower(true);
    selectedPiece = -1;
}

void MirrorPuzzle::Update(float dt) {
    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        if (selectedPiece == -1) {
            Vec2 mousePos = {(float) INPUT_MANAGER.GetMouseX(), (float) INPUT_MANAGER.GetMouseY()};
            for (int i = 0; i < pieces.size(); i++) {
                Rect pieceRect = pieces[i].GetRect().Add(Vec2{MIRROR_PUZZLE_RECT_X, MIRROR_PUZZLE_RECT_Y});
                if (pieceRect.Contains(mousePos)) {
                    selectedPiece = i;
                    break;
                }
            }
            std::cout << "GRAB = " << selectedPiece << std::endl;
        } else {
            selectedPiece = -1;
            std::cout << "RELEASE = -1" << std::endl;
        }
    }

    if (INPUT_MANAGER.IsKeyDown(SDLK_ESCAPE)) {
        CURRENT_STATE.openUI = false;
        associated.pauseOnOpenUI=true;
        associated.RequestDelete();
        std::cout << "MirrorPuzzle closed" << std::endl;
        INPUT_MANAGER.ReleaseKey(SDLK_ESCAPE);
    }

    if (selectedPiece != -1) {
        pieces[selectedPiece].pos.x = INPUT_MANAGER.GetMouseX() - pieces[selectedPiece].GetWidth() / 2 - MIRROR_PUZZLE_RECT_X;
        pieces[selectedPiece].pos.y = INPUT_MANAGER.GetMouseY() - pieces[selectedPiece].GetHeight() / 2 - MIRROR_PUZZLE_RECT_Y;
    } else if (IsSolved()) {
        std::cout << "!!! SOLVED !!!" << std::endl;
        CURRENT_STATE.openUI = false;
        associated.pauseOnOpenUI=true;
        associated.RequestDelete();
    }
}

void MirrorPuzzle::Render() {
    bg.Render(0, 0, 1200, 900);

    SDL_Rect bgRect;
    bgRect.x = MIRROR_PUZZLE_RECT_X;
    bgRect.y = MIRROR_PUZZLE_RECT_Y;
    bgRect.w = 250;
    bgRect.h = 500;

    //std::cout<< "Renderizando MirrorPuzzle" << std::endl;

    // Background rectangle
    SDL_SetRenderDrawColor(GAME_RENDERER, 136, 136, 136, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(GAME_RENDERER, &bgRect);

    // Pieces
    for (int i = 0; i < pieces.size(); i++) {
        float x = pieces[i].pos.x + bgRect.x;
        float y = pieces[i].pos.y + bgRect.y;
        if (selectedPiece == i) {
            if (pieces[i].pos.Distance(pieces[i].posCerta) <= DISTANCE_THRESHOLD) {
                SDL_SetRenderDrawColor(GAME_RENDERER, 0, 255, 0, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
            }
            SDL_RenderDrawLine(GAME_RENDERER, x, y, pieces[i].posCerta.x + bgRect.x, pieces[i].posCerta.y + bgRect.y);
        }
        pieces[i].sprite.Render(x, y, pieces[i].GetWidth(), pieces[i].GetHeight());
    }
}

void MirrorPuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI=false;

    for (int i = 0; i < pieces.size(); i++) {
        // Min x = 150
        int randomOffset = std::rand() % 900;
        std::cout << "rand " << randomOffset << std::endl;
        pieces[i].pos = {(float) ((randomOffset) - 325), 525};
    }
}

bool MirrorPuzzle::IsSolved() {
    for (int i = 0; i < pieces.size(); i++) {
        if (pieces[i].pos.Distance(pieces[i].posCerta) > DISTANCE_THRESHOLD) {
            return false;
        }
    }
    return true;
}

bool MirrorPuzzle::Is(std::string type) {
    return type == "MirrorPuzzle";
}
