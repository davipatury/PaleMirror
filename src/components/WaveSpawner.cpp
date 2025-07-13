#include "components/WaveSpawner.h"

#include "core/Game.h"

bool WaveSpawner::allWavesEnded;

WaveSpawner::WaveSpawner(GameObject& associated) : Component(associated) {
    zombieCounter = 0;
    npcCounter = 0;
    currentWave = 0;
    allWavesEnded = false;
    waves = { Wave(5, 2), Wave(10, 1, 1, 3), Wave(15, 1, 3, 3) };
}

void WaveSpawner::Update(float dt) {
    if (Character::player == nullptr) return;

    zombieCooldownTimer.Update(dt);
    npcCooldownTimer.Update(dt);

    if (zombieCounter < waves[currentWave].zombies) {
        if (zombieCooldownTimer.Get() >= waves[currentWave].cooldown) {
            Vec2 outOfScreenPos = GenerateSpawnPos();
            zombieCounter++;
            zombieCooldownTimer.Restart();

            GameObject* zombie = new GameObject();
            Zombie* zombieCmp = new Zombie((*zombie));
            zombie->AddComponent(zombieCmp);
            zombie->box.x = outOfScreenPos.x;
            zombie->box.y = outOfScreenPos.y;
            zombie->box.z = 0;
            CURRENT_STATE.AddObject(zombie);
        }
    }

    if (npcCounter < waves[currentWave].npcs) {
        if (npcCooldownTimer.Get() >= waves[currentWave].npcCooldown) {
            Vec2 outOfScreenPos = GenerateSpawnPos();
            npcCounter++;
            npcCooldownTimer.Restart();

        }
    }


}

Vec2 WaveSpawner::GenerateSpawnPos() {
    Vec2 offset = Vec2(700, 0).Rotate((std::rand() % 360) * 180 / M_PI);
    // Screen center + offset
    return Vec2(Character::player->Pos().x, Character::player->Pos().y).Add(offset);
}

void WaveSpawner::Render() { }

bool WaveSpawner::Is(std::string type) {
    return type == "WaveSpawner";
}
