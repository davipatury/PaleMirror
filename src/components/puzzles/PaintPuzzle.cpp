#include "components/puzzles/PaintPuzzle.h"
#include "core/Game.h"

#define PAINT_BG_OFFSET_X 233
#define PAINT_BG_OFFSET_Y 65

#define PAINT_TINTA_OFFSET_X 533
#define PAINT_TINTA_OFFSET_Y 304

#define RECT_VERMELHO Rect(236, 520, 101, 63)
#define RECT_AZUL Rect(343, 515, 101, 69)
#define RECT_AMARELO Rect(449, 506, 108, 79)
#define RECT_LIMPAR Rect(773, 183, 47, 148)

PaintPuzzle::PaintPuzzle(GameObject& associated, PaintColor corCerta) : Component(associated),
    bg("Recursos/img/paint_puzzle/cavalete.png", 1, 1, true),
    tinta("Recursos/img/paint_puzzle/paint.png", 1, 1, true)
{
    this->corCerta = corCerta;
    corAtual = COR_VAZIA;
    estadoAtual = QUADRO_VAZIO;
    selectedRect = EMPTY_RECT;

    bg.SetCameraFollower(true);
    tinta.SetCameraFollower(true);
}

void PaintPuzzle::Update(float dt) {
    if (estadoAtual == QUADRO_SOLVED) return;
    if (IsSolved()) {
        // Solved
        DialogueHUD::RequestDialogue("paintPuzzle_solved", [this]() {
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
    Vec2 mousePos = INPUT_MANAGER.GetMousePos();
    selectedRect = EMPTY_RECT;
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

    if (INPUT_MANAGER.MousePress(LEFT_MOUSE_BUTTON)) {
        // Check if mouse is inside rects
        if (selectedRect == RECT_VERMELHO) {
            Pintar(VERMELHO);
        } else if (selectedRect == RECT_AZUL) {
            Pintar(AZUL);
        } else if (selectedRect == RECT_AMARELO) {
            Pintar(AMARELO);
        } else if (selectedRect == RECT_LIMPAR) {
            corAtual = COR_VAZIA;
            estadoAtual = QUADRO_VAZIO;
        }
    }

    if (ESCAPE_CHECK) {
        CURRENT_STATE.openUI = false;
        associated.RequestDelete();
    }
}

void PaintPuzzle::Render() {
    // Semi transparent black background
    SDL_Rect screenRect = {0, 0, 1200, 900};
    SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    // Cavalete
    bg.Render(PAINT_BG_OFFSET_X, PAINT_BG_OFFSET_Y, bg.GetWidth(), bg.GetHeight());

    // Selection
    if (selectedRect != EMPTY_RECT) {
        SDL_Rect selRect = selectedRect.ToSDLRect();
        SDL_SetRenderDrawColor(GAME_RENDERER, 127, 127, 127, 255);
        SDL_RenderDrawRect(GAME_RENDERER, &selRect);
    }

    // Splash de tinta
    if (corAtual == COR_VAZIA) return;

    SDL_Color cor = GetCurrentColor();
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

SDL_Color PaintPuzzle::GetCurrentColor() {
    // Primarias
    if (corAtual == VERMELHO)               return SDL_Color{255, 0, 0, SDL_ALPHA_OPAQUE};
    if (corAtual == AZUL)                   return SDL_Color{0, 95, 255, SDL_ALPHA_OPAQUE};
    if (corAtual == AMARELO)                return SDL_Color{255, 242, 0, SDL_ALPHA_OPAQUE};
    // Secundarias
    if (corAtual == VIOLETA)                return SDL_Color{143, 25, 164, SDL_ALPHA_OPAQUE};
    if (corAtual == LARANJA)                return SDL_Color{255, 143, 0, SDL_ALPHA_OPAQUE};
    if (corAtual == VERDE)                  return SDL_Color{0, 182, 66, SDL_ALPHA_OPAQUE};
    // Terciarias
    if (corAtual == VERMELHO_ARROXEADO)     return SDL_Color{196, 45, 174, SDL_ALPHA_OPAQUE};
    if (corAtual == VERMELHO_ALARANJADO)    return SDL_Color{255, 82, 0, SDL_ALPHA_OPAQUE};
    if (corAtual == AMARELO_ESVERDEADO)     return SDL_Color{64, 227, 23, SDL_ALPHA_OPAQUE};
    if (corAtual == AMARELO_ALARANJADO)     return SDL_Color{255, 205, 0, SDL_ALPHA_OPAQUE};
    if (corAtual == AZUL_ROXEADO)           return SDL_Color{132, 64, 255, SDL_ALPHA_OPAQUE};
    if (corAtual == AZUL_ESVERDEADO)        return SDL_Color{0, 143, 123, SDL_ALPHA_OPAQUE};

    if (corAtual == COR_DESCONHECIDA)       return SDL_Color{67, 41, 41, SDL_ALPHA_OPAQUE};
    // Nao eh pra chegar aqui
    return SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE};
}

bool PaintPuzzle::IsSolved() {
    return corAtual == corCerta;
}

bool PaintPuzzle::Is(std::string type) {
    return type == "PaintPuzzle";
}
