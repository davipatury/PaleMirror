#include "components/HealthHUD.h"

HealthHUD::HealthHUD(GameObject& associated) : Component(associated) {
    // HP text
    hpText = new Text(associated, "Recursos/font/neodgm.ttf", 30, Text::TextStyle::SOLID, "100/100", {255, 255, 255, 255});
    associated.AddComponent(hpText);
}

HealthHUD::~HealthHUD() {}

void HealthHUD::Update(float dt) {
    if (Character::player == nullptr) return;

    associated.box.x = Camera::pos.x + 100;
    associated.box.y = Camera::pos.y + 800;

    std::ostringstream ss;
    ss << "HP: " << std::to_string(Character::player->GetHP()) << "/100";
    hpText->SetText(ss.str());
}

void HealthHUD::Render() {}

bool HealthHUD::Is(std::string type) {
    return type == "HealthHUD";
}
