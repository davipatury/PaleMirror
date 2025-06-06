#include "utils/Resources.h"
#include "core/Game.h"

std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
std::unordered_map<std::string, TTF_Font*> Resources::fontTable;

SDL_Texture* Resources::GetImage(const char* file) {
    if (Resources::imageTable.find(file) == Resources::imageTable.end()) {
        SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file);
        if (texture == nullptr) {
            std::cout << "Erro ao carregar textura: ";
            std::cout << IMG_GetError() << std::endl;
        }
        Resources::imageTable.insert({ file, texture });
        return texture;
    }
    return Resources::imageTable.at(file);
}

Mix_Music* Resources::GetMusic(const char* file) {
    if (Resources::musicTable.find(file) == Resources::musicTable.end()) {
        Mix_Music* music = Mix_LoadMUS(file);
        if (music == nullptr) {
            std::cout << "Erro ao carregar musica: ";
            std::cout << Mix_GetError() << std::endl;
        }
        Resources::musicTable.insert({ file, music });
        return music;
    }
    return Resources::musicTable.at(file);
}

Mix_Chunk* Resources::GetSound(const char* file) {
    if (Resources::soundTable.find(file) == Resources::soundTable.end()) {
        Mix_Chunk* chunk = Mix_LoadWAV(file);
        if (chunk == nullptr) {
            std::cout << "Erro ao carregar som: ";
            std::cout << Mix_GetError() << std::endl;
        }
        Resources::soundTable.insert({ file, chunk });
        return chunk;
    }
    return Resources::soundTable.at(file);
}

TTF_Font* Resources::GetFont(const char *file, int size) {
    std::string name = file + std::to_string(size);
    if (Resources::fontTable.find(name) == Resources::fontTable.end()) {
        TTF_Font* font = TTF_OpenFont(file, size);
        if (font == nullptr) {
            std::cout << "Erro ao carregar fonte: ";
            std::cout << TTF_GetError() << std::endl;
        }
        Resources::fontTable.insert({ name, font });
        return font;
    }
    return Resources::fontTable.at(name);
}

void Resources::ClearAll() {
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}

void Resources::ClearImages() {
    for (const std::pair<std::string, SDL_Texture*> &pair_elem: Resources::imageTable) {
        SDL_DestroyTexture(pair_elem.second);
    }
    Resources::imageTable.clear();
}

void Resources::ClearMusics() {
    for (const std::pair<std::string, Mix_Music*> &pair_elem: Resources::musicTable) {
        Mix_FreeMusic(pair_elem.second);
    }
    Resources::musicTable.clear();
}

void Resources::ClearSounds() {
    for (const std::pair<std::string, Mix_Chunk*> &pair_elem: Resources::soundTable) {
        Mix_FreeChunk(pair_elem.second);
    }
    Resources::soundTable.clear();
}

void Resources::ClearFonts() {
    for (const std::pair<std::string, TTF_Font*> &pair_elem: Resources::fontTable) {
        TTF_CloseFont(pair_elem.second);
    }
    Resources::fontTable.clear();
}

