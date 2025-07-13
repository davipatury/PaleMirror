#ifndef STAGESTATE_H
#define STAGESTATE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <unordered_map>

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
#include "components/MirrorPuzzle.h"
#include "components/FusePuzzle.h"
#include "components/PaintPuzzle.h"
#include "components/PipePuzzle.h"
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
#include "utils/Text.h"
#include "components/TileMap.h"
#include "math/Vec2.h"
#include "components/WaveSpawner.h"
#include "entities/characters/Zombie.h"
#include "rooms/MainRoom.h"
#include "rooms/HistoryClassRoom.h"
#include "rooms/ArtsClassRoom.h"
#include "rooms/ScienceClassRoom.h"
#include "rooms/PortugueseClassRoom.h"

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

    float pauseX;
    float pauseY;
private:
    Music backgroundMusic;

    std::unordered_map<std::string, Room*> rooms;
    Room* currentRoom;
};

#endif // STAGESTATE_H
