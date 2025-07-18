#ifndef LOCKPUZZLE_H
#define LOCKPUZZLE_H

#include "core/Component.h"
#include "core/Sprite.h"
#include <array>
#include <string>
#include "utils/TextHUD.h"
#include "math/Rect.h"
#include "components/Sound.h"

class LockPuzzle : public Component {
public:
    LockPuzzle(GameObject& associated, std::string expectedPassword);

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    bool IsSolved();

    bool solved = false;

    TextHUD* digit0;
    TextHUD* digit1;
    TextHUD* digit2;
    TextHUD* digit3;

    Rect selectedRect;
    Sound* rolling;
    Sound* openLock;

    // Initiator component
    class Initiator : public Component {
    public:
        Initiator(GameObject& associated, std::string password, std::string targetRoom, int entryIndex = 0);
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);

        std::string password;
        std::string targetRoom;
        int entryIndex;
        Sound* openSound;
        bool* puzzleClosed = nullptr;
    };
private:
    std::string expected;
    std::array<int,4> current;
    Sprite bg1, bg2, bg3, bgunlocked, pageIcon;
    Sprite page;
    TextHUD* pageText;
    Rect pageRect;
    int selectedIndex;

    bool animating[4];
    float animOffsetY[4];
    int animFrom[4];
    int animTo[4];
    static constexpr float animSpeed = 300.0f;
    static constexpr float digitHeight = 200.0f;

    void RenderTriangle(Rect rect);
};

#endif
