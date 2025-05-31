#ifndef STAGESTATE_H
#define STAGESTATE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

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
#include "core/Music.h"
#include "components/PlayerController.h"
#include "math/Ray.h"
#include "physics/ShadowCaster.h"
#include "core/Sprite.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "components/HealthHUD.h"
#include "utils/Text.h"
#include "components/TileMap.h"
#include "math/Vec2.h"
#include "components/WaveSpawner.h"
#include "entities/characters/Zombie.h"

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

    float pauseX;
    float pauseY;
private:
    Music backgroundMusic;
};

#endif // STAGESTATE_H
