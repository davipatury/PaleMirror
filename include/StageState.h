#ifndef STAGESTATE_H
#define STAGESTATE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include "Camera.h"
#include "Collider.h"
#include "Collision.h"
#include "Character.h"
#include "EndState.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "IsoCollider.h"
#include "Line.h"
#include "Music.h"
#include "PlayerController.h"
#include "Ray.h"
#include "ShadowCaster.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "State.h"
#include "HealthHUD.h"
#include "Text.h"
#include "TileMap.h"
#include "Vec2.h"
#include "WaveSpawner.h"
#include "Zombie.h"

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
private:
    Music backgroundMusic;
};

#endif // STAGESTATE_H
