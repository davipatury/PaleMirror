#include "components/puzzles/PipePuzzle.h"

#include "states/StageState.h"

#define PIPE_PUZZLE_RECT_X 0
#define PIPE_PUZZLE_RECT_Y 0

PipePuzzle::PipePuzzle(GameObject& associated) : Component(associated), bg("Recursos/img/pipe_puzzle/pipebg.png", 1, 1, true) {
    for(int i=0; i<16; i++) pipes.push_back(Pipe()), rotated.push_back(0), pipeAngles.push_back(0.0f), pipeTargetAngles.push_back(0.0f);
    pipePressed = new Sound("Recursos/audio/sounds/puzzle/cano-mexendo-3.wav");
}

void PipePuzzle::PipePressed(int idx){
    pipePressed->Play();
    rotated[idx] = (rotated[idx] + 1) % 4;
    pipeTargetAngles[idx] += 90.0f;
    std::cout << idx << ' ' << rotated[idx] << std::endl;
}

void PipePuzzle::Update(float dt) {
    if (solved) return;

    const float ROTATION_SPEED = 360.0f;

    for(int i = 0; i < pipes.size(); i++) {
        float diff = pipeTargetAngles[i] - pipeAngles[i];
        if (std::fabs(diff) > 0.1f) {
            float delta = ROTATION_SPEED * dt;
            if (std::fabs(diff) < delta) {
                pipeAngles[i] = pipeTargetAngles[i];
            } else {
                pipeAngles[i] += delta;
            }
        }
    }

    if (INPUT_MANAGER.HasController()) {
        int mod4 = selectedPiece % 4;
        if (LEFT_CHECK && mod4 > 0) selectedPiece--;
        if (RIGHT_CHECK && mod4 < 3) selectedPiece++;
        if (UP_CHECK && selectedPiece > 3) selectedPiece -= 4;
        if (DOWN_CHECK && selectedPiece < 12) selectedPiece += 4;
        if (CONFIRM_CHECK) PipePressed(selectedPiece);
    } else {
        selectedPiece = 0;
        Vec2 mousePos = {(float) INPUT_MANAGER.GetMouseX(), (float) INPUT_MANAGER.GetMouseY()};
        for (int i = 0; i < pipes.size(); i++) {
            Rect PipeRect = pipes[i].GetRect().Add(Vec2{PIPE_PUZZLE_RECT_X, PIPE_PUZZLE_RECT_Y});
            if (PipeRect.Contains(mousePos)) {
                selectedPiece = i;
                break;
            }
        }

        if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) PipePressed(selectedPiece);
    }

    if (ESCAPE_CHECK || BACK_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }

    if (IsSolved()) {
        DialogueHUD::RequestDialogue("pipePuzzle_solved", [this]() {
            INVENTORY->Collect(ITEM_CANO);
            GameData::pipePuzzleSolved = true;
            associated.RequestDelete();
        });
        solved = true;
    }
}

void PipePuzzle::Render() {
    SDL_Rect bgRect = WINDOW_RECT;

    // Background rectangle
    bg.Render(bgRect.x, bgRect.y, bgRect.w, bgRect.h);

    // Pieces
    for (int i = 0; i < pipes.size(); i++) {
        float x = pipes[i].pos.x + bgRect.x;
        float y = pipes[i].pos.y + bgRect.y;
        if (selectedPiece == i) SDL_SetTextureColorMod(pipes[i].sprite.texture, 150, 150, 150);
        else                    SDL_SetTextureColorMod(pipes[i].sprite.texture, 255, 255, 255);
        pipes[i].sprite.Render(x, y, pipes[i].GetWidth(), pipes[i].GetHeight(), fmod(pipeAngles[i], 360.0f));
    }
}

void PipePuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI=false;

    for (int i = 0; i < pipes.size(); i++) {
        pipes[i].pos = {(float) ((182 + i%4 * 210)), (float) ((20 + i/4 * 210))};

        if(i == 2 || i == 3 || i == 4 || i == 6 || i == 10 || i == 12 || i == 14) pipes[i].sprite.SetFrame(3);
        if(i == 0 || i == 7 || i == 8 || i == 9 || i == 13 || i == 15) pipes[i].sprite.SetFrame(2);
        if(i == 1 || i == 11) pipes[i].sprite.SetFrame(1);
        if(i == 5) pipes[i].sprite.SetFrame(0);
    }

    rotated[3] = 2, rotated[4] = 1, rotated[6] = 3, rotated[8] = 3, rotated[12] = 2, rotated[14] = 1;
}

bool PipePuzzle::IsSolved() {
    return ((rotated[0] == 0 || rotated[0] == 2) && (rotated[1] == 2 || rotated[1] == 3) && rotated[6] == 0 && rotated[2] == 3 && 
    rotated[3] == 2 && (rotated[7] == 1 || rotated[7] == 3) && rotated[11] != 2 && 
    rotated[10] == 3 && rotated[14] == 3 && (rotated[15] == 0 || rotated[15] == 2));
}

bool PipePuzzle::Is(std::string type) {
    return type == "PipePuzzle";
}

// PipePuzzle initiator
PipePuzzle::Initiator::Initiator(GameObject& associated) : Component(associated) {
    openSound = new Sound("Recursos/audio/sounds/objetos/cano.wav");
}

void PipePuzzle::Initiator::Update(float dt) {
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    if (!intr) return;
    if (puzzleClosed != nullptr && !(*puzzleClosed)) return;
    puzzleClosed = nullptr;

    if(!GameData::fusePuzzleSolved) {
        intr->SetActivationDistance(70);
        intr->SetHUDText("Interagir");
        intr->SetAction([this, intr](State* state, GameObject* go) {
            
            // Chama o Diálogo.
        });
        return;
    } 
    if (GameData::pipePuzzleSolved) {
        intr->SetActivationDistance(0);
    } else {
        intr->SetActivationDistance(70);
        intr->SetHUDText("Interagir");

        intr->SetAction([this, intr](State* state, GameObject* go) {
            openSound->Play();

            GameObject* pip = new GameObject();
            pip->AddComponent(new PipePuzzle(*pip));
            pip->box.z = PUZZLE_LAYER;
            pip->lazyRender = false;
            pip->pauseOnOpenUI = false;
            state->AddObject(pip);

            puzzleClosed = &pip->isDead;
            intr->SetActivationDistance(0);
        });
    }
}

void PipePuzzle::Initiator::Render() {}

void PipePuzzle::Initiator::Start() {}

bool PipePuzzle::Initiator::Is(std::string type) {
    return type == "PipePuzzleInitiator";
}
