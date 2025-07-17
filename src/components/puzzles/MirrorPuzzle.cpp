#include "components/puzzles/MirrorPuzzle.h"
#include "core/Game.h"

//#define DEBUG_MIRROR_PUZZLE

#define MIRROR_PUZZLE_RECT_X 475
#define MIRROR_PUZZLE_RECT_Y 100
#define DISTANCE_THRESHOLD 20

#define CONTROLLER_PIECE_SPEED 300

MirrorPuzzle::MirrorPuzzle(GameObject& associated, std::vector<Piece> pieces) : Component(associated), bg("Recursos/img/mirror_puzzle/Azulejos.png", 1, 1, true) {
    this->pieces = pieces;
    selectedPiece = -1;
}

int MirrorPuzzle::ClosestPiece(Vec2 joystick) {
    Vec2 hoverCenter = pieces[hoverPiece].Center();
    float joystickAngle = joystick.Angle();
    auto angle = [joystick, joystickAngle](Vec2 vec) {
        if (joystickAngle < 3*M_PI/4 && M_PI/4 < joystickAngle) return vec.AngleN();
        else if (3*M_PI/4 < joystickAngle || joystickAngle < -3*M_PI/4) return vec.AngleE();
        else if (-3*M_PI/4 < joystickAngle && joystickAngle < -M_PI/4) return vec.AngleS();
        else return vec.AngleW();
        /*if (joystick.x <= 0 && joystick.y <= 0) return vec.AngleS();
        if (joystick.x <= 0 && 0 <= joystick.y) return vec.AngleE();
        if (0 <= joystick.x && joystick.y <= 0) return vec.AngleW();
        if (0 <= joystick.x && 0 <= joystick.y) return vec.AngleN();
        return 0.0f;*/
    };
    joystickAngle = angle(joystick);
    int lowestI = -1;
    float lowestValue = 0;
    for (int i = 0; i < pieces.size(); i++) {
        if (hoverPiece != i) {
            Vec2 diff = pieces[i].Center() - hoverCenter;
            float angleDiff = abs(abs(angle(diff)) - abs(joystickAngle));
            float distance = diff.Magnitude();

            // Ajuste de peso da distancia, angulo importa mais
            float comparisonNumber = angleDiff + distance * 0.001;
            if ((comparisonNumber < lowestValue || lowestI == -1)) {
                lowestI = i;
                lowestValue = comparisonNumber;
            }
        }
    }
    return lowestI;
}

void MirrorPuzzle::Update(float dt) {
    if (solved) return;

    if (INPUT_MANAGER.HasController()) {
        // Controller
        if (selectedPiece == -1) {
            // Select piece
            int i = -1;
            Vec2 joystick = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK);
            if (LEFT_CHECK) i = ClosestPiece(joystick.x == 0 ? Vec2{-1, 0} : joystick);
            if (RIGHT_CHECK) i = ClosestPiece(joystick.x == 0 ? Vec2{1, 0} : joystick);
            if (UP_CHECK) i = ClosestPiece(joystick.x == 0 ? Vec2{0, -1} : joystick);
            if (DOWN_CHECK) i = ClosestPiece(joystick.x == 0 ? Vec2{0, 1} : joystick);

            if (i != -1) hoverPiece = i;
            if (CONFIRM_CHECK) selectedPiece = hoverPiece;
        } else {
            // Move piece
            Vec2 joystick = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK);
            pieces[selectedPiece].pos = pieces[selectedPiece].pos + (joystick * CONTROLLER_PIECE_SPEED * dt);
            if (CONFIRM_CHECK || BACK_CHECK) selectedPiece = -1;
        }
    } else {
        // KBM
        hoverPiece = -1;
        bool mousePressed = INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON);
        if (selectedPiece == -1) {
            Vec2 mousePos = INPUT_MANAGER.GetMousePos();
            for (int i = 0; i < pieces.size(); i++) {
                Rect pieceRect = pieces[i].GetRect().Add(Vec2{MIRROR_PUZZLE_RECT_X, MIRROR_PUZZLE_RECT_Y});
                if (pieceRect.Contains(mousePos)) {
                    hoverPiece = i;
                    break;
                }
            }
            if (mousePressed) selectedPiece = hoverPiece;
        } else {
            pieces[selectedPiece].pos.x = INPUT_MANAGER.GetMouseX() - pieces[selectedPiece].GetWidth() / 2 - MIRROR_PUZZLE_RECT_X;
            pieces[selectedPiece].pos.y = INPUT_MANAGER.GetMouseY() - pieces[selectedPiece].GetHeight() / 2 - MIRROR_PUZZLE_RECT_Y;
            if (mousePressed) selectedPiece = -1;
        }
    }

    if (ESCAPE_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }

    if (selectedPiece == -1 && IsSolved()) {
        hoverPiece = -1;
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
    bg.Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Background rectangle
    SDL_Rect bgRect = {MIRROR_PUZZLE_RECT_X, MIRROR_PUZZLE_RECT_Y, 250, 500};
    SDL_SetRenderDrawColor(GAME_RENDERER, 136, 136, 136, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(GAME_RENDERER, &bgRect);

    // Pieces
    for (int i = 0; i < pieces.size(); i++) {
        if (i != selectedPiece && i != hoverPiece) RenderPiece(i);
    }

    // Render hover and selected above all
    if (hoverPiece != -1) RenderPiece(hoverPiece);
    if (selectedPiece != -1) RenderPiece(selectedPiece);
}

void MirrorPuzzle::RenderPiece(int i) {
    float x = pieces[i].pos.x + MIRROR_PUZZLE_RECT_X;
    float y = pieces[i].pos.y + MIRROR_PUZZLE_RECT_Y;
    if (selectedPiece == i) {
        SDL_SetTextureColorMod(pieces[i].sprite.texture, 150, 150, 150);
#ifdef DEBUG_MIRROR_PUZZLE
        if (pieces[i].pos.Distance(pieces[i].posCerta) <= DISTANCE_THRESHOLD) {
            SDL_SetRenderDrawColor(GAME_RENDERER, 0, 255, 0, SDL_ALPHA_OPAQUE);
        } else {
            SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderDrawLine(GAME_RENDERER, x, y, pieces[i].posCerta.x + bgRect.x, pieces[i].posCerta.y + bgRect.y);
#endif
    } else if (hoverPiece == i) {
        SDL_SetTextureColorMod(pieces[i].sprite.texture, 200, 200, 200);
    } else {
        SDL_SetTextureColorMod(pieces[i].sprite.texture, 255, 255, 255);
    }
    pieces[i].sprite.Render(x, y, pieces[i].GetWidth(), pieces[i].GetHeight());
}

void MirrorPuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI = false;

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
