#include "core/Component.h"
#include "core/GameObject.h"
#include "core/Sprite.h"

class DocumentHUD : public Component {
public:
    DocumentHUD(GameObject& associated, const std::string& imagePath);
    ~DocumentHUD();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) override;

    Sprite bg;
};
