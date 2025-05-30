#ifndef WAVE_H
#define WAVE_H

class Wave
{
public:
    Wave(int zombies, float cooldown, int npcs = 0, float npcCooldown = 0.0f);
    int zombies;
    float cooldown;
    int npcs;
    float npcCooldown;
};

#endif // WAVE_H
