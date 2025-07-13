#include "components/PipePuzzle.h"

#define PIPE_PUZZLE_RECT_X 50
#define PIPE_PUZZLE_RECT_Y 50

PipePuzzle::PipePuzzle(GameObject& associated) : Component(associated), bg("Recursos/img/pipe_puzzle/pipebg.png"){
    for(int i=0; i<16; i++) pipes.push_back(Pipe()), rotated.push_back(0);
    bg.SetCameraFollower(true);
}

void PipePuzzle::PipePressed(int idx){
    rotated[idx] = (rotated[idx] + 1) % 4;
    std::cout << "idx: " << idx << " rotated: " << rotated[idx] << std::endl;
    // GIRA 90 GRAUS
}

void PipePuzzle::Update(float dt) {
    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        Vec2 mousePos = {(float) INPUT_MANAGER.GetMouseX(), (float) INPUT_MANAGER.GetMouseY()};
        for (int i = 0; i < pipes.size(); i++) {
            Rect PipeRect = pipes[i].GetRect().Add(Vec2{PIPE_PUZZLE_RECT_X, PIPE_PUZZLE_RECT_Y});
            if (PipeRect.Contains(mousePos)) {
                PipePressed(i);
            }
        }
    }

    if (INPUT_MANAGER.IsKeyDown(SDLK_ESCAPE)) {
        CURRENT_STATE.openUI = false;
        associated.pauseOnOpenUI=true;
        associated.RequestDelete();
        INPUT_MANAGER.ReleaseKey(SDLK_ESCAPE);
    }

    if (IsSolved() && !solvedDialogue) {
        DialogueHUD::RequestDialogue("pipePuzzle_solved", [this]() {
            associated.RequestDelete();
            CURRENT_STATE.openUI = false;
        });
        solvedDialogue = true;
    }
}

void PipePuzzle::Render() {
    SDL_Rect bgRect;
    bgRect.x = PIPE_PUZZLE_RECT_X;
    bgRect.y = PIPE_PUZZLE_RECT_Y;
    bgRect.w = 800;
    bgRect.h = 800;

    // Background rectangle
    bg.Render(bgRect.x, bgRect.y, bgRect.w, bgRect.h);

    // Pieces
    for (int i = 0; i < pipes.size(); i++) {
        float x = pipes[i].pos.x + bgRect.x;
        float y = pipes[i].pos.y + bgRect.y;
        pipes[i].sprite.Render(x, y, pipes[i].GetWidth(), pipes[i].GetHeight(), rotated[i] * 90);
    }
}

void PipePuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI=false;

    for (int i = 0; i < pipes.size(); i++) {
        pipes[i].pos = {(float) ((i%4 * 200)), (float) ((i/4 * 200))};

        if(i == 7 || i == 8 || i == 9 || i == 13 || i == 15) pipes[i].sprite.SetFrame(0);
        if(i == 0 || i == 2 || i == 3 || i == 4 || i == 6 || i == 10 || i == 12 || i == 14) pipes[i].sprite.SetFrame(1);
        if(i == 1 || i == 11) pipes[i].sprite.SetFrame(2);
        if(i == 5) pipes[i].sprite.SetFrame(3);

    }
}

bool PipePuzzle::IsSolved() {
    return (rotated[0] == 0 && rotated[1] != 1 && rotated[6] == 3 && rotated[2] == 1 && 
    rotated[3] == 2 && (rotated[7] == 1 || rotated[7] == 3) && rotated[11] != 2 && 
    rotated[10] == 1 && rotated[14] == 0 && (rotated[15] == 0 || rotated[15] == 2));
}

bool PipePuzzle::Is(std::string type) {
    return type == "PipePuzzle";
}
