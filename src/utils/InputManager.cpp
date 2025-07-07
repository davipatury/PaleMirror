#include "utils/InputManager.h"

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

    for (int i = 0; i < 6; i++) {
        controllerState[i] = false;
        controllerUpdate[i] = 0;
    }

    std::cout << "[INPUTMANAGER] Total joysticks found: " << SDL_NumJoysticks() << std::endl;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            std::cout << "[INPUTMANAGER] Controller index used: " << i << std::endl;
            break;
        }
    }

    updateCounter = 0;
    quitRequested = false;
    anyKeyPress = false;
    mouseX = 0;
    mouseY = 0;
}

InputManager::~InputManager() { }

void InputManager::Update() {
    updateCounter++;
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    anyKeyPress = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            if (event.key.repeat != 1) {
                int key = KeyToIndex(event.key.keysym.sym);
                keyState[key] = true;
                keyUpdate[key] = updateCounter;
                anyKeyPress = true;
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
        case SDL_CONTROLLERBUTTONDOWN: {
            controllerState[event.cbutton.button] = true;
            controllerUpdate[event.cbutton.button] = updateCounter;
            break;
        }
        case SDL_CONTROLLERBUTTONUP: {
            controllerState[event.cbutton.button] = false;
            controllerUpdate[event.cbutton.button] = updateCounter;
            break;
        }
        case SDL_QUIT:
            quitRequested = true;
            break;
        }
    }
}

/*
 * Keyboard inputs
 */
bool InputManager::AnyKeyPress() {
    return anyKeyPress;
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

void InputManager::ReleaseKey(int key){
    int i = KeyToIndex(key);
    keyState[i] = false;
}

/*
 * Mouse input
 */
bool InputManager::MousePress(int button) {
    return (mouseUpdate[button] == updateCounter) ? mouseState[button] : false;
}

bool InputManager::MouseRelease(int button) {
    return (mouseUpdate[button] == updateCounter) ? !mouseState[button] : false;
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

/*
 * Controller input
 */
bool InputManager::HasController() {
    return controller != nullptr;
}

bool InputManager::CButtonPress(int button) {
    return (controllerUpdate[button] == updateCounter) ? controllerState[button] : false;
}

bool InputManager::CButtonRelease(int button) {
    return (controllerUpdate[button] == updateCounter) ? !controllerState[button] : false;
}

bool InputManager::IsCButtonDown(int button) {
    return controllerState[button];
}

Vec2 InputManager::ControllerAxis(int joystick) {
    if (joystick == LEFT_JOYSTICK) {
        int x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        int y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
        return Vec2(ParseAxis(x), ParseAxis(y));
    } else {
        int x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
        int y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
        return Vec2(ParseAxis(x), ParseAxis(y));
    }
}

float InputManager::ParseAxis(int value) {
    if (abs(value) < JOYSTICK_DEADZONE) return 0.0f;
    float actualValue = value > 0 ? value - JOYSTICK_DEADZONE : value + JOYSTICK_DEADZONE;
    return actualValue / JOYSTICK_MAX_VALUE;
}

/*
 * Others
 */
int InputManager::KeyToIndex(int key) {
    return (key >= 0x40000000) ? key - 0x3FFFFF81 : key;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}

InputManager& InputManager::GetInstance() {
    static InputManager inputManager;
    return inputManager;
}
