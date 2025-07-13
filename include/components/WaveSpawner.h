#ifndef WAVESPAWNER_H
#define WAVESPAWNER_H

#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <vector>

#include "entities/characters/Character.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "utils/Timer.h"
#include "entities/characters/Zombie.h"
#include "utils/Wave.h"

class WaveSpawner : public Component
{
public:
    WaveSpawner(GameObject& associated);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    Vec2 GenerateSpawnPos();
    static bool allWavesEnded;
private:
    std::vector<Wave> waves;
    int currentWave;

    int zombieCounter;
    Timer zombieCooldownTimer;

    int npcCounter;
    Timer npcCooldownTimer;
};

#endif // WAVESPAWNER_H
