#ifndef WAVESPAWNER_H
#define WAVESPAWNER_H

#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <vector>

#include "AIController.h"
#include "Character.h"
#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include "Wave.h"
#include "Zombie.h"

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
