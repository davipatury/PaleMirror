#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"

class TileMap : public Component
{
public:
    TileMap(GameObject& associated, const char* file, TileSet* tileSet);

    void Load(const char* file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y, int z = 0);
    void Update(float dt);
    void Render();
    void RenderLayer(int layer);

    int GetWidth();
    int GetHeight();
    int GetDepth();

    bool Is(std::string type);
private:
    std::vector<int> tileMatrix;
    std::unique_ptr<TileSet> tileSet;

    int mapWidth;
    int mapHeight;
    int mapDepth;
};

#endif // TILEMAP_H
