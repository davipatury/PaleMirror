#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, const char* file) {
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    // Abrir Sprite
    tileSet.Open(file);
    if (tileSet.IsOpen()) {
        int cols = tileSet.GetWidth() / tileWidth;
        int rows = tileSet.GetHeight() / tileHeight;
        tileCount = rows * cols;
        tileSet.SetFrameCount(cols, rows);
    } else {
        std::cout << "Erro ao carregar TileSet: ";
        std::cout << SDL_GetError()  << std::endl;
    }
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    if (index >= 0 && index < tileCount) {
        tileSet.SetFrame(index);
        tileSet.Render(x, y, tileWidth, tileHeight);
    }
}

int TileSet::GetTileWidth() {
    return tileWidth;
}

int TileSet::GetTileHeight() {
    return tileHeight;
}
