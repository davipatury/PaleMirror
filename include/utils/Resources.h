#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <unordered_map>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

class Resources
{
public:
    static SDL_Texture* GetImage(const char* file);
    static Mix_Music* GetMusic(const char* file);
    static Mix_Chunk* GetSound(const char* file);
    static TTF_Font* GetFont(const char* file, int size);

    static void ClearAll();
    static void ClearImages();
    static void ClearMusics();
    static void ClearSounds();
    static void ClearFonts();

    static std::unordered_map<std::string, SDL_Texture*> imageTable;
    static std::unordered_map<std::string, Mix_Music*> musicTable;
    static std::unordered_map<std::string, Mix_Chunk*> soundTable;
    static std::unordered_map<std::string, TTF_Font*> fontTable;
};

#endif // RESOURCES_H
