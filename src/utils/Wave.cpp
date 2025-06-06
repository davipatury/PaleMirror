#include "utils/Wave.h"

Wave::Wave(int zombies, float cooldown, int npcs, float npcCooldown) {
    this->zombies = zombies;
    this->cooldown = cooldown;
    this->npcs = npcs;
    this->npcCooldown = npcCooldown;
}
