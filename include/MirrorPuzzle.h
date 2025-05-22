#ifndef MIRRORPUZZLE_H
#define MIRRORPUZZLE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#include "Game.h"
#include "InputManager.h"
#include "Rect.h"
#include "Sprite.h"
#include "Vec2.h"

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

    std::vector<Piece> pieces;
    int selectedPiece;
};

#endif // MIRRORPUZZLE_H
