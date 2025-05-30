#ifndef TILESET_H
#define TILESET_H

#include <iostream>

#include "core/Sprite.h"

class TileSet
{
public:
    TileSet(int tileWidth, int tileHeight, const char* file);

    void RenderTile(unsigned index, float x, float y);
    int GetTileWidth();
    int GetTileHeight();
private:
    Sprite tileSet;
    int tileWidth;
    int tileHeight;
    int tileCount;
};

#endif // TILESET_H
