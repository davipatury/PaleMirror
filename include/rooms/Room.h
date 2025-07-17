#ifndef ROOM_H
#define ROOM_H

#include "components/Camera.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "core/Music.h"
#include "core/Sprite.h"
#include "entities/characters/Character.h"

class Room
{
public:
    State* state;
    const char* bgFile;
    const char* bgMusicFile = nullptr;

    GameObject* bgGO;
    Music bgMusic;
    double lastMusicPosition = 0.0d;
    bool lockCameraOnRoom;

    Vec2 basePos;
    std::vector<Vec2> entryPos;

    Rect cameraLimits;

    Room(State* state);

    void Build();
    void Enter();
    void Leave();
    GameObject* createGO(std::string name, float x, float y);
};

#endif // ROOM_H
