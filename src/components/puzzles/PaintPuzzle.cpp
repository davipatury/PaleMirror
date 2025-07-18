#include "components/puzzles/PaintPuzzle.h"
#include "core/Game.h"

#include "states/StageState.h"

#define PAINT_BG_OFFSET_X 233
#define PAINT_BG_OFFSET_Y 65

#define PAINT_TINTA_OFFSET_X 533
#define PAINT_TINTA_OFFSET_Y 304

#define RECT_VERMELHO Rect(236, 520, 101, 63)
#define RECT_AZUL Rect(343, 515, 101, 69)
#define RECT_AMARELO Rect(449, 506, 108, 79)
#define RECT_LIMPAR Rect(773, 183, 47, 148)

PaintPuzzle::PaintColor PaintPuzzle::possibleSolutions[] = {
    PaintPuzzle::VERMELHO_ARROXEADO, PaintPuzzle::VERMELHO_ALARANJADO, PaintPuzzle::AMARELO_ESVERDEADO, PaintPuzzle::AMARELO_ALARANJADO, PaintPuzzle::AZUL_ROXEADO, PaintPuzzle::AZUL_ESVERDEADO
};

PaintPuzzle::PaintColor PaintPuzzle::solution;

PaintPuzzle::PaintPuzzle(GameObject& associated) : Component(associated),
    bg("Recursos/img/paint_puzzle/cavalete.png", 1, 1, true),
    tinta("Recursos/img/paint_puzzle/paint.png", 1, 1, true),
    tintaVermelha("Recursos/img/paint_puzzle/tinta_vermelha.png", 1, 1, true),
    tintaAzul("Recursos/img/paint_puzzle/tinta_azul.png", 1, 1, true),
    tintaAmarela("Recursos/img/paint_puzzle/tinta_amarela.png", 1, 1, true),
    pano("Recursos/img/paint_puzzle/pano.png", 1, 1, true)
{
    splash = new Sound("Recursos/audio/sounds/puzzle/splash-tinta-2.wav");
    clearSound = new Sound("Recursos/audio/sounds/puzzle/limpar_tinta.wav");
    corAtual = COR_VAZIA;
    estadoAtual = QUADRO_VAZIO;
    selectedRect = RECT_VERMELHO;

    SDL_SetTextureColorMod(tintaVermelha.texture, 150, 150, 150);
    SDL_SetTextureColorMod(tintaAzul.texture, 150, 150, 150);
    SDL_SetTextureColorMod(tintaAmarela.texture, 150, 150, 150);
    SDL_SetTextureColorMod(pano.texture, 150, 150, 150);
}

void PaintPuzzle::Update(float dt) {
    if (estadoAtual == QUADRO_SOLVED) return;
    if (!dicaInicio) {
        associated.pauseOnOpenUI = true;
        DialogueHUD::RequestDialogue("paintPuzzle_start", [this]() {
            associated.pauseOnOpenUI = false;
            CURRENT_STATE.openUI = true;
        });
        dicaInicio = true;
    }
    if (IsSolved()) {
        // Solved
        DialogueHUD::RequestDialogue("paintPuzzle_solved", [this]() {
            INVENTORY->Collect(ITEM_BALDE_TINTA);
            GameData::paintPuzzleSolved = true;
            associated.RequestDelete();
        });
        estadoAtual = QUADRO_SOLVED;
    }

    if (corAtual == COR_DESCONHECIDA && estadoAtual != QUADRO_ERRADO) {
        DialogueHUD::RequestDialogue("paintPuzzle_corErrada", [this]() {
            associated.pauseOnOpenUI = false;
            CURRENT_STATE.openUI = true;
        });
        associated.pauseOnOpenUI = true;
        estadoAtual = QUADRO_ERRADO;
    }

    // Selection
    if (INPUT_MANAGER.HasController()) {
        if (LEFT_CHECK) {
            if (selectedRect == RECT_AZUL) selectedRect = RECT_VERMELHO;
            else if (selectedRect == RECT_AMARELO) selectedRect = RECT_AZUL;
            else if (selectedRect == RECT_LIMPAR) selectedRect = RECT_AMARELO;
        }
        if (RIGHT_CHECK) {
            if (selectedRect == RECT_VERMELHO) selectedRect = RECT_AZUL;
            else if (selectedRect == RECT_AZUL) selectedRect = RECT_AMARELO;
            else if (selectedRect == RECT_AMARELO) selectedRect = RECT_LIMPAR;
        }
        if (BACK_CHECK) selectedRect = RECT_LIMPAR;
        if (CONFIRM_CHECK) {
            if (selectedRect == RECT_VERMELHO) {
                splash->Play();
                Pintar(VERMELHO);
            } else if (selectedRect == RECT_AZUL) {
                splash->Play();
                Pintar(AZUL);
            } else if (selectedRect == RECT_AMARELO) {
                splash->Play();
                Pintar(AMARELO);
            } else if (selectedRect == RECT_LIMPAR) {
                clearSound->Play();
                corAtual = COR_VAZIA;
                estadoAtual = QUADRO_VAZIO;
            }
        }
    } else {
        selectedRect = EMPTY_RECT;
        Vec2 mousePos = INPUT_MANAGER.GetMousePos();
        if (estadoAtual != QUADRO_ERRADO) {
            if (RECT_VERMELHO.Contains(mousePos) && corAtual != VERMELHO) {
                selectedRect = RECT_VERMELHO;
            } else if (RECT_AZUL.Contains(mousePos) && corAtual != AZUL) {
                selectedRect = RECT_AZUL;
            } else if (RECT_AMARELO.Contains(mousePos) && corAtual != AMARELO) {
                selectedRect = RECT_AMARELO;
            }
        }
        if (RECT_LIMPAR.Contains(mousePos)) {
            selectedRect = RECT_LIMPAR;
        }

        // Check if mouse is inside rects
        if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON) && selectedRect != EMPTY_RECT) {
            if (selectedRect == RECT_VERMELHO) {
                splash->Play();
                Pintar(VERMELHO);
            } else if (selectedRect == RECT_AZUL) {
                splash->Play();
                Pintar(AZUL);
            } else if (selectedRect == RECT_AMARELO) {
                splash->Play();
                Pintar(AMARELO);
            } else if (selectedRect == RECT_LIMPAR) {
                clearSound->Play();
                corAtual = COR_VAZIA;
                estadoAtual = QUADRO_VAZIO;
            }
        }
    }

    if (ESCAPE_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }
}

void PaintPuzzle::Render() {
    // Semi transparent black background
    SDL_Rect screenRect = WINDOW_RECT;
    SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    // Cavalete
    bg.Render(PAINT_BG_OFFSET_X, PAINT_BG_OFFSET_Y, bg.GetWidth(), bg.GetHeight());

    // Selection
    if (selectedRect == RECT_VERMELHO)      tintaVermelha.Render(PAINT_BG_OFFSET_X, PAINT_BG_OFFSET_Y + 455, tintaVermelha.GetWidth(), tintaVermelha.GetHeight());
    else if (selectedRect == RECT_AZUL)     tintaAzul.Render(PAINT_BG_OFFSET_X + 108, PAINT_BG_OFFSET_Y + 449, tintaAzul.GetWidth(), tintaAzul.GetHeight());
    else if (selectedRect == RECT_AMARELO)  tintaAmarela.Render(PAINT_BG_OFFSET_X + 216, PAINT_BG_OFFSET_Y + 439, tintaAmarela.GetWidth(), tintaAmarela.GetHeight());
    else if (selectedRect == RECT_LIMPAR)   pano.Render(PAINT_BG_OFFSET_X + 541, PAINT_BG_OFFSET_Y + 119, pano.GetWidth(), pano.GetHeight());

    // Splash de tinta
    if (corAtual == COR_VAZIA) return;

    SDL_Color cor = GetColor(corAtual);
    SDL_SetTextureColorMod(tinta.texture, cor.r, cor.g, cor.b);
    tinta.Render(PAINT_TINTA_OFFSET_X, PAINT_TINTA_OFFSET_Y, tinta.GetWidth(), tinta.GetHeight());
}

void PaintPuzzle::Start() {
    CURRENT_STATE.openUI = true;
    associated.pauseOnOpenUI = false;
}

void PaintPuzzle::Pintar(PaintColor cor) {
    if (estadoAtual == QUADRO_ERRADO) return;
    if (estadoAtual == QUADRO_VAZIO) {
        corAtual = cor;
        estadoAtual = QUADRO_PRIMARIO;
    } else if (estadoAtual == QUADRO_PRIMARIO) {
        if (corAtual == cor) return;
        if (corAtual == VERMELHO && cor == AZUL)    corAtual = VIOLETA;
        if (corAtual == VERMELHO && cor == AMARELO) corAtual = LARANJA;
        if (corAtual == AZUL && cor == VERMELHO)    corAtual = VIOLETA;
        if (corAtual == AZUL && cor == AMARELO)     corAtual = VERDE;
        if (corAtual == AMARELO && cor == VERMELHO) corAtual = LARANJA;
        if (corAtual == AMARELO && cor == AZUL)     corAtual = VERDE;
        estadoAtual = QUADRO_SECUNDARIO;
    } else if (estadoAtual == QUADRO_SECUNDARIO) {
        if (corAtual == VIOLETA && cor == VERMELHO) corAtual = VERMELHO_ARROXEADO;
        if (corAtual == VIOLETA && cor == AZUL)     corAtual = AZUL_ROXEADO;
        if (corAtual == VIOLETA && cor == AMARELO)  corAtual = COR_DESCONHECIDA;
        if (corAtual == LARANJA && cor == VERMELHO) corAtual = VERMELHO_ALARANJADO;
        if (corAtual == LARANJA && cor == AZUL)     corAtual = COR_DESCONHECIDA;
        if (corAtual == LARANJA && cor == AMARELO)  corAtual = AMARELO_ALARANJADO;
        if (corAtual == VERDE && cor == VERMELHO)   corAtual = COR_DESCONHECIDA;
        if (corAtual == VERDE && cor == AZUL)       corAtual = AZUL_ESVERDEADO;
        if (corAtual == VERDE && cor == AMARELO)    corAtual = AMARELO_ESVERDEADO;
        estadoAtual = QUADRO_TERCIARIO;
    } else {
        corAtual = COR_DESCONHECIDA;
    }
}

SDL_Color PaintPuzzle::GetColor(PaintColor cor) {
    // Primarias
    if (cor == VERMELHO)               return SDL_Color{255, 0, 0, SDL_ALPHA_OPAQUE};
    if (cor == AZUL)                   return SDL_Color{0, 95, 255, SDL_ALPHA_OPAQUE};
    if (cor == AMARELO)                return SDL_Color{255, 242, 0, SDL_ALPHA_OPAQUE};
    // Secundarias
    if (cor == VIOLETA)                return SDL_Color{143, 25, 164, SDL_ALPHA_OPAQUE};
    if (cor == LARANJA)                return SDL_Color{255, 143, 0, SDL_ALPHA_OPAQUE};
    if (cor == VERDE)                  return SDL_Color{0, 182, 66, SDL_ALPHA_OPAQUE};
    // Terciarias
    if (cor == VERMELHO_ARROXEADO)     return SDL_Color{196, 45, 174, SDL_ALPHA_OPAQUE};
    if (cor == VERMELHO_ALARANJADO)    return SDL_Color{255, 82, 0, SDL_ALPHA_OPAQUE};
    if (cor == AMARELO_ESVERDEADO)     return SDL_Color{64, 227, 23, SDL_ALPHA_OPAQUE};
    if (cor == AMARELO_ALARANJADO)     return SDL_Color{255, 205, 0, SDL_ALPHA_OPAQUE};
    if (cor == AZUL_ROXEADO)           return SDL_Color{132, 64, 255, SDL_ALPHA_OPAQUE};
    if (cor == AZUL_ESVERDEADO)        return SDL_Color{0, 143, 123, SDL_ALPHA_OPAQUE};

    if (cor == COR_DESCONHECIDA)       return SDL_Color{67, 41, 41, SDL_ALPHA_OPAQUE};
    // Nao eh pra chegar aqui
    return SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE};
}

std::string PaintPuzzle::GetColorName(PaintColor cor) {
    // Terciarias
    if (cor == VERMELHO_ARROXEADO)     return "vermelho arroxeado";
    if (cor == VERMELHO_ALARANJADO)    return "vermelho alaranjado";
    if (cor == AMARELO_ESVERDEADO)     return "amarelo esverdeado";
    if (cor == AMARELO_ALARANJADO)     return "amarelo alaranjado";
    if (cor == AZUL_ROXEADO)           return "azul roxeado";
    if (cor == AZUL_ESVERDEADO)        return "azul esverdeado";
    // Nao eh pra chegar aqui
    return "SEI LA";
}

std::string PaintPuzzle::GetSolutionName() {
    return GetColorName(solution);
}

SDL_Color PaintPuzzle::GetSolutionColor() {
    return GetColor(solution);
}

bool PaintPuzzle::IsSolved() {
    return corAtual == solution;
}

bool PaintPuzzle::Is(std::string type) {
    return type == "PaintPuzzle";
}

void PaintPuzzle::GenerateRandomSolution() {
    solution = possibleSolutions[std::rand() % (sizeof(possibleSolutions) / sizeof(*possibleSolutions))];
}

// MirrorPuzzle initiator
PaintPuzzle::Initiator::Initiator(GameObject& associated) : Component(associated) {}

void PaintPuzzle::Initiator::Update(float dt) {
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    if (!intr) return;
    StageState* stage = (StageState*) &CURRENT_STATE;
    if (stage->openUI) return;

    intr->SetActivationDistance(50);
    if (GameData::paintPuzzleSolved) {
        intr->SetHUDText("Inspecionar");
        intr->SetAction([this](State* state, GameObject* go) {
            //std::cout << "Dialogo interagir com o cavalete depois de completado" << std::endl;
            DialogueHUD::RequestDialogue("paintPuzzle_solved");
            // TODO: Dialogo interagir com o cavalete depois de completado
        });
    } else {
        intr->SetHUDText("Interagir");
        intr->SetAction([this, intr](State* state, GameObject* go) {
            if (GameData::paintPuzzleSolved) return;
            GameObject* pp = new GameObject();
            pp->AddComponent(new PaintPuzzle(*pp));
            pp->box.z = PUZZLE_LAYER;
            pp->lazyRender = false;
            pp->pauseOnOpenUI = false;
            state->AddObject(pp);

            intr->SetActivationDistance(0);
        });
    }
}

void PaintPuzzle::Initiator::Render() {}

void PaintPuzzle::Initiator::Start() {}

bool PaintPuzzle::Initiator::Is(std::string type) {
    return type == "PaintPuzzleInitiator";
}
