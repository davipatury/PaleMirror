#include "core/Component.h"
#include "core/Sprite.h"
#include <array>
#include <string>
#include "utils/TextHUD.h"
#include "math/Rect.h"

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

private:
    std::string expected;
    std::array<int,4> current;
    Sprite bg1, bg2, bg3, bgunlocked;
    int selectedIndex;

    bool animating[4];
    float animOffsetY[4];
    int animFrom[4];
    int animTo[4];
    static constexpr float animSpeed = 300.0f;
    static constexpr float digitHeight = 200.0f;

    void RenderTriangle(Rect rect);
};
