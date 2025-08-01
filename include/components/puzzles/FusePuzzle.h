#ifndef FUSEPUZZLE_H
#define FUSEPUZZLE_H

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
#include "utils/Timer.h"
#include "components/Interactable.h"
#include "components/LightEmitter.h"
#include "core/GameData.h"

class FusePuzzle : public Component
{
public:
    class Fuse {
    public:
        Fuse() {
            sprite.Open("Recursos/img/fuse_puzzle/fuse.png");
            sprite.SetFrameCount(2, 1);
            sprite.SetFrame(0);
            sprite.SetCameraFollower(true);
            on = false;
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

    FusePuzzle(GameObject& associated);

    void Update(float dt);
    void Render();
    void Start();
    void FusePressed(int idx);
    bool Is(std::string type);

    bool IsSolved();

    std::vector<Fuse> fuses;
    int selectedPiece = 0;
    bool solved = false;

    Sprite bg;
    Sound* fusePressed;

    // Initiator component
    class Initiator : public Component {
    public:
        Initiator(GameObject& associated);
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);

        Sound* openSound;
        bool* puzzleClosed = nullptr;
    };
};

#endif // FUSEPUZZLE_H
