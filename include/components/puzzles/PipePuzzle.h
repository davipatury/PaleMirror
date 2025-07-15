#ifndef PIPEPUZZLE_H
#define PIPEPUZZLE_H

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

class PipePuzzle : public Component
{
public:
    class Pipe {
    public:
        Pipe() {
            sprite.Open("Recursos/img/pipe_puzzle/pipe.png");
            sprite.SetFrameCount(4, 1);
            sprite.SetFrame(0);
            sprite.SetCameraFollower(true);
        }

        bool on;
        Vec2 pos;
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

    PipePuzzle(GameObject& associated);

    void Update(float dt);
    void Render();
    void Start();
    void PipePressed(int idx);
    bool Is(std::string type);

    bool IsSolved();

    std::vector<Pipe> pipes;
    std::vector<int> rotated;
    std::vector<float> pipeAngles;
    std::vector<float> pipeTargetAngles;

    Sound* pipePressed;
    int selectedPiece;
    bool solved = false;

    Sprite bg;
};

#endif // PIPEPUZZLE_H
