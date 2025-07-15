#include "hud/HealthHUD.h"

HealthHUD::HealthHUD(GameObject& associated) : Component(associated),
    dmg1("Recursos/img/hud/dmg1.png", 1, 1, true),
    dmg2("Recursos/img/hud/dmg2.png", 1, 1, true),
    dmg3("Recursos/img/hud/dmg3.png", 1, 1, true)
{
}

HealthHUD::~HealthHUD() {}

void HealthHUD::Update(float dt) {}

void HealthHUD::Render() {
    if (Character::player == nullptr) return;
    int hp = Character::player->GetHP();
    if (hp < 40) {
        dmg3.Render(0, 0, 1200, 900);
    } else if (hp < 65) {
        dmg2.Render(0, 0, 1200, 900);
    } else if (hp < 80) {
        dmg1.Render(0, 0, 1200, 900);
    }
}

bool HealthHUD::Is(std::string type) {
    return type == "HealthHUD";
}
