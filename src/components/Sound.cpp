#include "components/Sound.h"

Sound::Sound() {
    chunk = nullptr;
    channel = -1;
}

Sound::Sound(const char *file): Sound() {
    Open(file);
}

Sound::~Sound() {
    if (chunk != nullptr) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Play(int times) {
    if (chunk != nullptr) {
        channel = Mix_PlayChannel(-1, chunk, times);
    }
}

void Sound::Stop() {
    if (chunk != nullptr) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(const char* file) {
    chunk = Resources::GetSound(file);
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}

