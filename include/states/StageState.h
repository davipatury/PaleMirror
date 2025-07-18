#ifndef STAGESTATE_H
#define STAGESTATE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "components/Camera.h"
#include "physics/Collider.h"
#include "physics/Collision.h"
#include "entities/characters/Character.h"
#include "states/EndState.h"
#include "core/Game.h"
#include "core/GameData.h"
#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "physics/IsoCollider.h"
#include "math/Line.h"
#include "components/puzzles/MirrorPuzzle.h"
#include "components/puzzles/FusePuzzle.h"
#include "components/puzzles/PaintPuzzle.h"
#include "components/puzzles/PipePuzzle.h"
#include "components/puzzles/LockPuzzle.h"
#include "core/Music.h"
#include "components/PlayerController.h"
#include "math/Ray.h"
#include "physics/ShadowCaster.h"
#include "core/Sprite.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "hud/HealthHUD.h"
#include "hud/DialogueHUD.h"
#include "hud/InteractableHUD.h"
#include "hud/FlashlightHUD.h"
#include "hud/InventoryHUD.h"
#include "hud/PauseHUD.h"
#include "hud/DebugHUD.h"
#include "utils/Text.h"
#include "math/Vec2.h"
#include "components/WaveSpawner.h"
#include "entities/characters/Zombie.h"
#include "entities/characters/ZombieFast.h"
#include "entities/characters/Boss.h"
#include "entities/projectiles/HitAttack.h"
#include "rooms/MainRoom.h"
#include "rooms/HistoryClassRoom.h"
#include "rooms/ArtsClassRoom.h"
#include "rooms/ScienceClassRoom.h"
#include "rooms/PortugueseClassRoom.h"
#include "rooms/BanheiroFemininoRoom.h"
#include "rooms/BanheiroMasculinoRoom.h"
#include "rooms/BanheiroIntroRoom.h"

class StageState : public State
{
public:
    StageState();
    ~StageState();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();

    void ChangeRoom(std::string room);
    Room* GetRoom(std::string room);
    Room* GetCurrentRoom();

    float pauseX;
    float pauseY;

    enum scenes{
        RITUAL_NONE,
        RITUAL_BEFORE,
        RITUAL_DURING,
        RITUAL_AFTER,
        RITUAL_DONE,
        BOSS_BATTLE
    };

    scenes scene = RITUAL_NONE;

    bool zombieFarAway = false;
    Timer zombieFarAwayTimer;
    Sound* zombieFarAwaySound;
private:
    Music backgroundMusic;

    std::unordered_map<std::string, Room*> rooms;
    Room* currentRoom;
};

#endif // STAGESTATE_H
