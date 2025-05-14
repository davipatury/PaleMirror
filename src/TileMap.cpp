#include "TileMap.h"

TileMap::TileMap(GameObject& associated, const char* file, TileSet* tileSet) : Component(associated) {
    Load(file);
    SetTileSet(tileSet);
}

void TileMap::Load(const char* file) {
    // Read map .txt
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs) {
        std::cout << "Erro ao carregar TileMap: " << file << std::endl;
        return;
    }

    char comma;
    ifs >> mapWidth >> comma;
    ifs >> mapHeight >> comma;
    ifs >> mapDepth >> comma;

    int value;
    for (int i = 0; i < (mapWidth * mapHeight * mapDepth); i++) {
        ifs >> value >> comma;
        tileMatrix.push_back(value);
    }
    ifs.close();
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet = std::unique_ptr<TileSet>(tileSet);
}

int& TileMap::At(int x, int y, int z) {
    return tileMatrix[x + (y * mapWidth) + (z * mapWidth * mapHeight)];
}

void TileMap::Update(float dt) { }

void TileMap::Render() {
    for (int i = 0; i < mapDepth; i++) {
        RenderLayer(i);
    }
}

void TileMap::RenderLayer(int layer) {
    // Efeito Parallax na renderização das camadas do TileMap
    // Quanto mais "profunda" a camada, mais lenta ela deve se mover
    int layerDepth = mapDepth - layer - 1;
    // Multiplicador de velocidade (cada camada fica 80% mais lenta que a anterior)
    float speedMul = 0.5;
    float extraX = Camera::pos.x * speedMul * layerDepth;
    float extraY = Camera::pos.y * speedMul * layerDepth;
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            tileSet->RenderTile(At(x, y, layer),
                                associated.box.x + (x * tileSet->GetTileWidth()) + extraX,
                                associated.box.y + (y * tileSet->GetTileHeight()) + extraY);
        }
    }
}

int TileMap::GetWidth() {
    return mapWidth;
}

int TileMap::GetHeight() {
    return mapHeight;
}

int TileMap::GetDepth() {
    return mapDepth;
}

bool TileMap::Is(std::string type) {
    return type == "TileMap";
}
