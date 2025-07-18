#ifndef PAINTPUZZLE_H
#define PAINTPUZZLE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#include "core/Game.h"
#include "utils/InputManager.h"
#include "math/Rect.h"
#include "core/Sprite.h"
#include "math/Vec2.h"
#include "hud/DialogueHUD.h"
#include "hud/InventoryHUD.h"

class PaintPuzzle : public Component
{
public:
    enum PaintState { QUADRO_VAZIO, QUADRO_PRIMARIO, QUADRO_SECUNDARIO, QUADRO_TERCIARIO, QUADRO_ERRADO, QUADRO_SOLVED };
    enum PaintColor {
        COR_VAZIA,
        // Primarias
        VERMELHO, AZUL, AMARELO,
        // Secundarias
        VIOLETA, LARANJA, VERDE,
        // Terciarias
        VERMELHO_ARROXEADO, VERMELHO_ALARANJADO, AMARELO_ESVERDEADO, AMARELO_ALARANJADO, AZUL_ROXEADO, AZUL_ESVERDEADO,
        // Erros
        COR_DESCONHECIDA
    };

    PaintPuzzle(GameObject& associated);

    void Update(float dt);
    void Render();
    void Start();
    bool Is(std::string type);

    static SDL_Color GetColor(PaintColor cor);
    static SDL_Color GetSolutionColor();
    static void GenerateRandomSolution();
    void Pintar(PaintColor cor);
    bool IsSolved();

    Rect selectedRect;
    PaintState estadoAtual;
    PaintColor corCerta;
    PaintColor corAtual;
    Sprite bg, tintaVermelha, tintaAzul, tintaAmarela, pano;
    Sprite tinta;
    Sound* splash;
    Sound* clearSound;

    static PaintColor solution;
    static PaintColor possibleSolutions[];

    // Initiator component
    class Initiator : public Component {
    public:
        Initiator(GameObject& associated);
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);
        bool* puzzleClosed = nullptr;
    };
};

#endif // PAINTPUZZLE_H
