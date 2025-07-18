#ifndef MIRRORPUZZLE_H
#define MIRRORPUZZLE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <functional>

#include "core/Game.h"
#include "utils/InputManager.h"
#include "math/Rect.h"
#include "core/Sprite.h"
#include "math/Vec2.h"
#include "hud/DialogueHUD.h"

class MirrorPuzzle : public Component
{
public:
    class Piece {
    public:
        Piece(const char* file, Vec2 posCerta) {
            this->posCerta = posCerta;
            sprite.SetCameraFollower(true);
            sprite.Open(file);
        }

        Vec2 pos;
        Vec2 posCerta;
        Sprite sprite;

        Rect GetRect() {
            return Rect(pos.x, pos.y, sprite.GetWidth(), sprite.GetHeight());
        }

        Vec2 Center() {
            return GetRect().Center();
        }

        int GetWidth() {
            return sprite.GetWidth();
        }

        int GetHeight() {
            return sprite.GetHeight();
        }
    };

    MirrorPuzzle(GameObject& associated, std::vector<Piece> pieces);

    void Update(float dt);
    void Render();
    void Start();
    bool Is(std::string type);

    bool IsSolved();
    int ClosestPiece(Vec2 joystick);
    void RenderPiece(int i);

    bool solved = false;
    std::vector<Piece> pieces;
    int selectedPiece = -1;
    int hoverPiece = 0;
    Sprite bg;
    Sprite espelhoCompleto;
    bool background = false;

    // Initiator component
    class Initiator : public Component {
    public:
        Initiator(GameObject& associated);
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);

        bool* puzzleClosed = nullptr;
        Sound* openSound;
    };
};

#endif // MIRRORPUZZLE_H
