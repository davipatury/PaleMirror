#include "components/puzzles/FusePuzzle.h"

#define FUSE_PUZZLE_RECT_X 260
#define FUSE_PUZZLE_RECT_Y 210

FusePuzzle::FusePuzzle(GameObject& associated) : Component(associated), bg("Recursos/img/fuse_puzzle/fuseboxbg.png", 1, 1, true){
    for(int i=0; i<9; i++) fuses.push_back(Fuse());
    fusePressed = new Sound("Recursos/audio/sounds/puzzle/click.wav");
}

void FusePuzzle::FusePressed(int idx){
    fusePressed->Play();

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
    if (solved) return;

    if (INPUT_MANAGER.HasController()) {
        int mod3 = selectedPiece % 3;
        if (UP_CHECK && selectedPiece > 2) selectedPiece -= 3;
        if (DOWN_CHECK && selectedPiece < 6) selectedPiece += 3;
        if (LEFT_CHECK && mod3) selectedPiece--;
        if (RIGHT_CHECK && mod3 < 2) selectedPiece++;

        if (CONFIRM_CHECK) FusePressed(selectedPiece);
    } else {
        selectedPiece = -1;
        Vec2 mousePos = {(float) INPUT_MANAGER.GetMouseX(), (float) INPUT_MANAGER.GetMouseY()};
        for (int i = 0; i < fuses.size(); i++) {
            Rect fuseRect = fuses[i].GetRect().Add(Vec2{FUSE_PUZZLE_RECT_X, FUSE_PUZZLE_RECT_Y});
            if (fuseRect.Contains(mousePos)) {
                selectedPiece = i;
                break;
            }
        }

        if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON) && selectedPiece != -1) FusePressed(selectedPiece);
    }

    if (ESCAPE_CHECK || BACK_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }

    if (IsSolved()) {
        DialogueHUD::RequestDialogue("fusePuzzle_solved", [this]() {
            GameData::fusePuzzleSolved = true;
            associated.RequestDelete();
        });
        solved = true;
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

        // Selected fuse highlight
        if (selectedPiece == i) SDL_SetTextureColorMod(fuses[i].sprite.texture, 150, 150, 150);
        else                    SDL_SetTextureColorMod(fuses[i].sprite.texture, 255, 255, 255);

        fuses[i].sprite.Render(x, y, fuses[i].GetWidth(), fuses[i].GetHeight());
    }
}

void FusePuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI=false;
    bool always_on = true;

    for (int i = 0; i < fuses.size(); i++) {
        fuses[i].pos = {(float) (376 + (i%3 * 96)), (float) (90 + (i/3 * 126))};
        bool on = 0;
        //bool on = (rand() % 2);
        //always_on &= on;
        //if(i == fuses.size()-1 && always_on) on = 0;
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

// FusePuzzle initiator
FusePuzzle::Initiator::Initiator(GameObject& associated) : Component(associated) {
    openSound = new Sound("Recursos/audio/sounds/objetos/caixa_energia.wav");
}

void FusePuzzle::Initiator::Update(float dt) {
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    LightEmitter* light = (LightEmitter*) associated.GetComponent("LightEmitter");
    if (!sr || !intr || !light) return;
    if (puzzleClosed != nullptr && !(*puzzleClosed)) return;
    puzzleClosed = nullptr;

    if (GameData::fusePuzzleSolved) {
        sr->SetFrame(1, SDL_FLIP_HORIZONTAL);
        intr->SetActivationDistance(0.0f);
        light->SetEnabledAll(true);
    } else {
        sr->SetFrame(0, SDL_FLIP_HORIZONTAL);
        intr->SetActivationDistance(100.0f);
        intr->SetType(InteractableHUD::INTERACT);
        intr->SetHUDText("Interagir");
        intr->SetAction([intr, this](State* state, GameObject* go) {
            openSound->Play();

            GameObject* fp = new GameObject();
            fp->AddComponent(new FusePuzzle(*fp));
            fp->box.z = PUZZLE_LAYER;
            fp->lazyRender = false;
            fp->pauseOnOpenUI = false;
            state->AddObject(fp);

            puzzleClosed = &fp->isDead;
            intr->SetActivationDistance(0.0f);
        });
        light->SetEnabledAll(false);
    }
}

void FusePuzzle::Initiator::Render() {}

void FusePuzzle::Initiator::Start() {}

bool FusePuzzle::Initiator::Is(std::string type) {
    return type == "FusePuzzleInitiator";
}
