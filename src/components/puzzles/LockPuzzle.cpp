#include "components/puzzles/LockPuzzle.h"
#include "components/SpriteRenderer.h"
#include "core/Game.h"
#include "utils/InputManager.h"
#include "hud/DialogueHUD.h"
#include "actions/Actions.h"

#include "components/Interactable.h"
#include "core/GameData.h"

class Interactable;

#define ARROW_UP_1 Rect(355, 307, 30, 30)
#define ARROW_UP_2 Rect(511, 307, 30, 30)
#define ARROW_UP_3 Rect(661, 307, 30, 30)
#define ARROW_UP_4 Rect(813, 307, 30, 30)

#define ARROW_DOWN_1 Rect(355, 631, 30, 30)
#define ARROW_DOWN_2 Rect(511, 631, 30, 30)
#define ARROW_DOWN_3 Rect(661, 631, 30, 30)
#define ARROW_DOWN_4 Rect(813, 631, 30, 30)

#define PAGE_ICON Rect(1000, 235, 80, 80)
#define PAGE_SPEED 300
#define PAGE_INDEX 4

LockPuzzle::LockPuzzle(GameObject& associated, std::string expectedPassword): Component(associated), 
    expected(expectedPassword), selectedIndex(0), bg1("Recursos/img/lock_puzzle/lock1.png", 1, 1, true),
    bg2("Recursos/img/lock_puzzle/lock2.png", 1, 1, true), bg3("Recursos/img/lock_puzzle/lock3.png", 1, 1, true),
    bgunlocked("Recursos/img/lock_puzzle/unlock.png", 1, 1, true), pageIcon("Recursos/img/hud/icon-bilhete.png", 1, 1, true),
    page("Recursos/img/hud/page.png", 1, 1, true)
{
    rolling = new Sound("Recursos/audio/sounds/puzzle/rolling.wav");
    openLock = new Sound("Recursos/audio/sounds/puzzle/open-lock.wav");
    selectedRect = EMPTY_RECT;
    current = {1, 6, 3, 7};

    digit0 = digit1 = digit2 = digit3 = nullptr;
    for(int i = 0; i < 4; i++){
        animating[i] = false;
        animOffsetY[i] = 0.0f;
    }

    pageRect.x = WINDOW_WIDTH - 100;
    pageRect.y = 50;
    pageRect.w = page.GetWidth();
    pageRect.h = page.GetHeight();
}   

void LockPuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI = false;

    digit0 = new TextHUD({322, 400}, "Recursos/font/canterbury.regular.ttf", 200, TextHUD::BLENDED, std::to_string(current[0]), {0, 0, 0, 0});
    digit1 = new TextHUD({475, 400}, "Recursos/font/canterbury.regular.ttf", 200, TextHUD::BLENDED, std::to_string(current[1]), {0, 0, 0, 0});
    digit2 = new TextHUD({623, 400}, "Recursos/font/canterbury.regular.ttf", 200, TextHUD::BLENDED, std::to_string(current[2]), {0, 0, 0, 0});
    digit3 = new TextHUD({783, 400}, "Recursos/font/canterbury.regular.ttf", 200, TextHUD::BLENDED, std::to_string(current[3]), {0, 0, 0, 0});
    pageText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 30, TextHUD::BLENDED, "Senha cadeado \n \
\n\
É tão triste que isso tenha acontecido com você… Você não sente raiva? Ressentimento? Não dói o que fizeram com você? Em quantos pedaços eles te quebraram? Quantas vezes elas não te deixaram em paz? Dezesseis anos jogados fora. \
\n\
\nVocê precisa reagir.", {0, 0, 0, 0}, 450);
}

void LockPuzzle::Update(float dt) {
    if(solved) return;
    
    if (IsSolved()) {
        // Solved
        openLock->Play();
        DialogueHUD::RequestDialogue("lockPuzzle_solved", [this]() {
            GameData::lockPuzzleSolved = true;
            associated.RequestDelete();
        });
        solved = true;
    }

    if (ESCAPE_CHECK || BACK_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }

    if (INPUT_MANAGER.HasController()) {
        if (LEFT_CHECK && selectedIndex > 0) selectedIndex--;
        if (RIGHT_CHECK && selectedIndex < PAGE_INDEX) selectedIndex++;

        if (selectedIndex != PAGE_INDEX) {
            if (UP_CHECK && !animating[selectedIndex]) {
                animating[selectedIndex]   = true;
                animOffsetY[selectedIndex] = 0.0f;
                animFrom[selectedIndex]    = current[selectedIndex];
                animTo[selectedIndex] = (animFrom[selectedIndex] + 1 + 10) % 10;
                rolling->Play();
            }

            if (DOWN_CHECK && !animating[selectedIndex]) {
                animating[selectedIndex]   = true;
                animOffsetY[selectedIndex] = 0.0f;
                animFrom[selectedIndex]    = current[selectedIndex];
                animTo[selectedIndex] = (animFrom[selectedIndex] - 1 + 10) % 10;
                rolling->Play();
            }
        }
    } else {
        selectedRect = EMPTY_RECT;
        selectedIndex = -1;
        Vec2 mousePos = INPUT_MANAGER.GetMousePos();
        if      (ARROW_UP_1.Contains(mousePos))    selectedRect = ARROW_UP_1;
        else if (ARROW_UP_2.Contains(mousePos))    selectedRect = ARROW_UP_2;
        else if (ARROW_UP_3.Contains(mousePos))    selectedRect = ARROW_UP_3;
        else if (ARROW_UP_4.Contains(mousePos))    selectedRect = ARROW_UP_4;
        else if (ARROW_DOWN_1.Contains(mousePos))  selectedRect = ARROW_DOWN_1;
        else if (ARROW_DOWN_2.Contains(mousePos))  selectedRect = ARROW_DOWN_2;
        else if (ARROW_DOWN_3.Contains(mousePos))  selectedRect = ARROW_DOWN_3;
        else if (ARROW_DOWN_4.Contains(mousePos))  selectedRect = ARROW_DOWN_4;
        else if (pageRect.Contains(mousePos))      selectedIndex = PAGE_INDEX;

        if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON) && selectedRect != EMPTY_RECT && selectedIndex != PAGE_INDEX) {
            int idx = -1; bool isUp = false;
            if (selectedRect == ARROW_UP_1)         { idx=0; isUp=true; }
            else if (selectedRect == ARROW_UP_2)    { idx=1; isUp=true; }
            else if (selectedRect == ARROW_UP_3)    { idx=2; isUp=true; }
            else if (selectedRect == ARROW_UP_4)    { idx=3; isUp=true; }
            else if (selectedRect == ARROW_DOWN_1)  { idx=0; isUp=false; }
            else if (selectedRect == ARROW_DOWN_2)  { idx=1; isUp=false; }
            else if (selectedRect == ARROW_DOWN_3)  { idx=2; isUp=false; }
            else if (selectedRect == ARROW_DOWN_4)  { idx=3; isUp=false; }

            if (idx >= 0 && !animating[idx]) {
                animating[idx]   = true;
                animOffsetY[idx] = 0.0f;
                animFrom[idx]    = current[idx];
                animTo[idx] = (animFrom[idx] + (isUp ? +1 : -1) + 10) % 10;

                rolling->Play();
            }
        }
    }

    // Page move
    if (selectedIndex == PAGE_INDEX) {
        pageRect.x = std::max(pageRect.x - PAGE_SPEED * dt, WINDOW_WIDTH - pageRect.w - 50);
    } else {
        pageRect.x = std::min(pageRect.x + PAGE_SPEED * dt, WINDOW_WIDTH - 100);
    }
    pageText->SetPos({pageRect.x + 75, pageRect.y + 50});

    for (int i = 0; i < 4; i++) {
        if (!animating[i]) continue;
        int dir = (animTo[i] == (animFrom[i]+1)%10) ? -1 : +1;
        animOffsetY[i] += dir * animSpeed * dt;

        // terminou a “roleta”?
        if (std::abs(animOffsetY[i]) >= digitHeight) {
            animating[i]   = false;
            current[i]     = animTo[i];
            animOffsetY[i] = 0.0f;
            TextHUD* d = (i==0?digit0:i==1?digit1:i==2?digit2:digit3);
            d->SetText(std::to_string(current[i]));
        }
    }
}

void LockPuzzle::Render() {
    SDL_Rect screenRect = WINDOW_RECT;
    SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    if(solved) {
        bgunlocked.Render(0, 0, bgunlocked.GetWidth(), bgunlocked.GetHeight());
        return;
    }

    if(!solved) bg2.Render(0, bg1.GetHeight(), bg2.GetWidth(), bg2.GetHeight());

    // Animação de cada dígito
    TextHUD* digits[4] = {digit0, digit1, digit2, digit3};
    SDL_Point bases[4] = {{322,400},{475,400},{623,400},{783,400}};

    for (int i = 0; i < 4; i++) {
        TextHUD* d = digits[i];
        if (animating[i]) {
            d->SetText(std::to_string(animFrom[i]));
            d->SetPos({ (float) bases[i].x, (float) (bases[i].y + animOffsetY[i]) });
            d->Render();

            bool up = (animTo[i] == (animFrom[i]+1)%10);
            int extraY = up ? +digitHeight : -digitHeight;
            d->SetText(std::to_string(animTo[i]));
            d->SetPos({ (float) bases[i].x, (float) (bases[i].y + animOffsetY[i] + extraY) });
            d->Render();

            d->SetText(std::to_string(current[i]));
        } else {
            d->SetPos({ (float) bases[i].x, (float) bases[i].y });
            d->Render();
        }
    }

    if(!solved) bg1.Render(0, 0, bg1.GetWidth(), bg1.GetHeight());
    if(!solved) bg3.Render(0, bg1.GetHeight()+bg2.GetHeight(), bg3.GetWidth(), bg3.GetHeight());

    // Selection
    if (INPUT_MANAGER.HasController()) {
        if (selectedIndex == 0) {
            RenderTriangle(ARROW_UP_1);
            RenderTriangle(ARROW_DOWN_1);
        }
        if (selectedIndex == 1) {
            RenderTriangle(ARROW_UP_2);
            RenderTriangle(ARROW_DOWN_2);
        }
        if (selectedIndex == 2) {
            RenderTriangle(ARROW_UP_3);
            RenderTriangle(ARROW_DOWN_3);
        }
        if (selectedIndex == 3) {
            RenderTriangle(ARROW_UP_4);
            RenderTriangle(ARROW_DOWN_4);
        }
    } else if (selectedRect != EMPTY_RECT and selectedIndex != PAGE_INDEX and !solved) {
        RenderTriangle(selectedRect);
    }

    // Page
    page.Render(pageRect.x, pageRect.y, pageRect.w, pageRect.h);
    pageText->Render();
}

void LockPuzzle::RenderTriangle(Rect rect) {
    SDL_Rect sel = rect.ToSDLRect();
    int cx = sel.x + sel.w/2;
    int cy = sel.y + sel.h/2;
    int halfW = sel.w/2;
    int halfH = sel.h/2;
    // ARROW_UP
    SDL_Point triUp[4] = {
        { cx, sel.y}, { sel.x, sel.y + sel.h }, { sel.x + sel.w, sel.y + sel.h }, { cx, sel.y}
    };
    // ARROW_DOWN
    SDL_Point triDown[4] = {
        { sel.x, sel.y }, { sel.x + sel.w, sel.y}, { cx, sel.y + sel.h }, { sel.x, sel.y }
    };
    SDL_SetRenderDrawColor(GAME_RENDERER, 127, 127, 127, 255);
    if (rect == ARROW_UP_1 || rect == ARROW_UP_2|| rect == ARROW_UP_3 || rect == ARROW_UP_4) {
        SDL_RenderDrawLines(GAME_RENDERER, triUp, 4);
    } else {
        SDL_RenderDrawLines(GAME_RENDERER, triDown, 4);
    }
}

bool LockPuzzle::Is(std::string type) {
    return type == "LockPuzzle";
}

bool LockPuzzle::IsSolved() {
    std::string now = "";
    for(int i=0; i<4; i++){
        now += std::to_string(current[i]);
    }
    return expected == now;
}


// LockPuzzle initiator
LockPuzzle::Initiator::Initiator(GameObject& associated, std::string password, std::string targetRoom, int entryIndex) : Component(associated) {
    openSound = new Sound("Recursos/audio/sounds/objetos/cadeado_trancado.wav");
    this->password = password;
    this->targetRoom = targetRoom;
    this->entryIndex = entryIndex;
}

void LockPuzzle::Initiator::Update(float dt) {
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    if (!intr) return;
    StageState* stage = (StageState*) &CURRENT_STATE;
    if (stage->openUI) return;

    if(GameData::runeState != GameData::RUNA_SUMMONADA) {
        intr->SetHUDText("Interagir");
        intr->SetAction([this, intr](State* state, GameObject* go) {
            DialogueHUD::RequestDialogue("lockPuzzle_agoraNao");
        });
        return;
    } 

    intr->SetActivationDistance(40.0f);
    if (GameData::lockPuzzleSolved) {
        intr->SetHUDText("Entrar");
        intr->SetAction(Actions::ChangeRoom(targetRoom, entryIndex));
    } else {
        intr->SetHUDText("Abrir");
        intr->SetAction([this, intr](State* state, GameObject* go) {
            if (GameData::lockPuzzleSolved) return;
            openSound->Play();

            GameObject* lp = new GameObject();
            lp->AddComponent(new LockPuzzle(*lp, this->password));
            lp->box.z = PUZZLE_LAYER;
            lp->lazyRender = false;
            lp->pauseOnOpenUI = false;
            state->AddObject(lp);

            intr->SetActivationDistance(0);
        });
    }
}

void LockPuzzle::Initiator::Render() {}

void LockPuzzle::Initiator::Start() {}

bool LockPuzzle::Initiator::Is(std::string type) {
    return type == "LockPuzzleInitiator";
}


