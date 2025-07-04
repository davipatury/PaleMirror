#include "components/HealthHUD.h"

HealthHUD::HealthHUD(GameObject& associated) : Component(associated), dmg1("Recursos/img/hud/dmg1.png"), dmg2("Recursos/img/hud/dmg2.png"), dmg3("Recursos/img/hud/dmg3.png"){
    // HP text
    hpText = new Text(associated, "Recursos/font/neodgm.ttf", 30, Text::TextStyle::SOLID, "100/100", {255, 255, 255, 255});
    associated.AddComponent(hpText);

    dmg1.SetCameraFollower(true);
    dmg2.SetCameraFollower(true);
    dmg3.SetCameraFollower(true);
}

HealthHUD::~HealthHUD() {}

void HealthHUD::Update(float dt) {
    if (Character::player == nullptr) return;

    associated.box.x = Camera::pos.x + 100;
    associated.box.y = Camera::pos.y + 800;

    std::ostringstream ss;
    ss << "HP: " << std::to_string(Character::player->GetHP()) << "/100";
    hpText->SetText(ss.str());

    int hp = Character::player->GetHP();
    if (hp >= 80) {
        hpBorder = nullptr;
    } else if (hp >= 65) {
        hpBorder = &dmg1;
    } else if (hp >= 40) {
        hpBorder = &dmg2;
    } else {
        hpBorder = &dmg3;
    }
}

void HealthHUD::Render() {
    if (hpBorder) {
        hpBorder->Render(0, 0, 1200, 900);
    }
}

bool HealthHUD::Is(std::string type) {
    return type == "HealthHUD";
}
