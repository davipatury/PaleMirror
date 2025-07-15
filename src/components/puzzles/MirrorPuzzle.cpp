#include "components/puzzles/MirrorPuzzle.h"
#include "core/Game.h"

//#define DEBUG_MIRROR_PUZZLE

#define MIRROR_PUZZLE_RECT_X 475
#define MIRROR_PUZZLE_RECT_Y 100
#define DISTANCE_THRESHOLD 20

MirrorPuzzle::MirrorPuzzle(GameObject& associated, std::vector<Piece> pieces) : Component(associated), bg("Recursos/img/mirror_puzzle/Azulejos.png", 1, 1, true) {
    this->pieces = pieces;
    selectedPiece = -1;
}

void MirrorPuzzle::Update(float dt) {
    if (solved) return;

    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        if (selectedPiece == -1) {
            Vec2 mousePos = INPUT_MANAGER.GetMousePos();
            for (int i = 0; i < pieces.size(); i++) {
                Rect pieceRect = pieces[i].GetRect().Add(Vec2{MIRROR_PUZZLE_RECT_X, MIRROR_PUZZLE_RECT_Y});
                if (pieceRect.Contains(mousePos)) {
                    selectedPiece = i;
                    break;
                }
            }
#ifdef DEBUG_MIRROR_PUZZLE
            std::cout << "[MirrorPuzzle] Grab = " << selectedPiece << std::endl;
#endif
        } else {
            selectedPiece = -1;
#ifdef DEBUG_MIRROR_PUZZLE
            std::cout << "[MirrorPuzzle] Release = -1" << std::endl;
#endif
        }
    }

    if (ESCAPE_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }

    if (selectedPiece != -1) {
        pieces[selectedPiece].pos.x = INPUT_MANAGER.GetMouseX() - pieces[selectedPiece].GetWidth() / 2 - MIRROR_PUZZLE_RECT_X;
        pieces[selectedPiece].pos.y = INPUT_MANAGER.GetMouseY() - pieces[selectedPiece].GetHeight() / 2 - MIRROR_PUZZLE_RECT_Y;
    } else if (IsSolved()) {
        for (int i = 0; i < pieces.size(); i++) {
            pieces[i].pos = pieces[i].posCerta;
        }
        DialogueHUD::RequestDialogue("mirrorPuzzle_solved", [this]() {
            associated.RequestDelete();
        });
        solved = true;
    }
}

void MirrorPuzzle::Render() {
    bg.Render(0, 0, 1200, 900);

    // Background rectangle
    SDL_Rect bgRect = {MIRROR_PUZZLE_RECT_X, MIRROR_PUZZLE_RECT_Y, 250, 500};
    SDL_SetRenderDrawColor(GAME_RENDERER, 136, 136, 136, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(GAME_RENDERER, &bgRect);

    // Pieces
    for (int i = 0; i < pieces.size(); i++) {
        float x = pieces[i].pos.x + bgRect.x;
        float y = pieces[i].pos.y + bgRect.y;
        if (selectedPiece == i) {
#ifdef DEBUG_MIRROR_PUZZLE
            if (pieces[i].pos.Distance(pieces[i].posCerta) <= DISTANCE_THRESHOLD) {
                SDL_SetRenderDrawColor(GAME_RENDERER, 0, 255, 0, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
            }
            SDL_RenderDrawLine(GAME_RENDERER, x, y, pieces[i].posCerta.x + bgRect.x, pieces[i].posCerta.y + bgRect.y);
#endif
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
        pieces[i].pos = {(float) ((randomOffset) - 325), 525};
    }
}

bool MirrorPuzzle::IsSolved() {
    for (int i = 0; i < pieces.size(); i++) {
        if (pieces[i].pos.Distance(pieces[i].posCerta) > DISTANCE_THRESHOLD) return false;
    }
    return true;
}

bool MirrorPuzzle::Is(std::string type) {
    return type == "MirrorPuzzle";
}
