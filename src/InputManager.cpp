#include "InputManager.h"

#include <iostream>

InputManager::InputManager() {
    for (int i = 0; i < 6; i++) {
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }

    for (int i = 0; i < 416; i++) {
        keyState[i] = false;
        keyUpdate[i] = 0;
    }

    updateCounter = 0;
    quitRequested = false;
    mouseX = 0;
    mouseY = 0;
}

InputManager::~InputManager() { }

void InputManager::Update() {
    updateCounter++;
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            if (event.key.repeat != 1) {
                int key = KeyToIndex(event.key.keysym.sym);
                keyState[key] = true;
                keyUpdate[key] = updateCounter;
            }
            break;
        }
        case SDL_KEYUP: {
            int key = KeyToIndex(event.key.keysym.sym);
            keyState[key] = false;
            keyUpdate[key] = updateCounter;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            mouseState[event.button.button] = true;
            mouseUpdate[event.button.button] = updateCounter;
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            mouseState[event.button.button] = false;
            mouseUpdate[event.button.button] = updateCounter;
            break;
        }
        case SDL_QUIT:
            quitRequested = true;
            break;
        }
    }
}

bool InputManager::KeyPress(int key) {
    int i = KeyToIndex(key);
    return (keyUpdate[i] == updateCounter) ? keyState[i] : false;
}

bool InputManager::KeyRelease(int key) {
    int i = KeyToIndex(key);
    return (keyUpdate[i] == updateCounter) ? !keyState[i] : false;
}

bool InputManager::IsKeyDown(int key) {
    int i = KeyToIndex(key);
    return keyState[i];
}

bool InputManager::MousePress(int button) {
    return (mouseUpdate[button] == updateCounter) ? mouseState[button] : false;
}

bool InputManager::MouseRelease(int button) {
    return (mouseUpdate[button] == updateCounter) ? !mouseState[button] : false;
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::KeyToIndex(int key) {
    return (key >= 0x40000000) ? key - 0x3FFFFF81 : key;
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}

InputManager& InputManager::GetInstance() {
    static InputManager inputManager;
    return inputManager;
}
