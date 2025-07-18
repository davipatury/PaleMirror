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

    std::cout << "[InputManager] Total joysticks found: " << SDL_NumJoysticks() << std::endl;
    ConfigureController();

    joystickDirTimer = Timer();
    updateCounter = 0;
    quitRequested = false;
    anyKeyPress = false;
    mouseX = 0;
    mouseY = 0;
}

InputManager::~InputManager() {
    if (HasController()) SDL_GameControllerClose(controller);
}

void InputManager::Update(float dt) {
    updateCounter++;
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    anyKeyPress = false;
    mouseWheel = 0;
    joystickDirTimer.Update(dt);

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
        case SDL_MOUSEWHEEL: {
            mouseWheel = event.wheel.y;
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED: {
            std::cout << "[InputManager] Controller removed" << std::endl;
            SDL_GameControllerClose(controller);
            controller = nullptr;
            ConfigureController();
            break;
        }
        case SDL_CONTROLLERDEVICEADDED: {
            if (!HasController()) ConfigureController();
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

Vec2 InputManager::GetMousePos() {
    return Vec2(mouseX, mouseY);
}

int InputManager::GetMouseWheel() {
    return mouseWheel;
}

/*
 * Controller input
 */
void InputManager::ConfigureController() {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            std::cout << "[InputManager] Controller index used: " << i << std::endl;
            break;
        }
    }
}

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

Vec2 InputManager::ControllerAxis(int joystick, float deadzone) {
    if (joystick == LEFT_JOYSTICK) {
        int x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        int y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
        return Vec2(ParseAxis(x, deadzone), ParseAxis(y, deadzone));
    } else {
        int x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
        int y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
        return Vec2(ParseAxis(x, deadzone), ParseAxis(y, deadzone));
    }
}

float InputManager::ParseAxis(int value, float deadzone) {
    if (abs(value) < deadzone) return 0.0f;
    float actualValue = value > 0 ? value - deadzone : value + deadzone;
    return actualValue / JOYSTICK_MAX_VALUE;
}

void InputManager::ReleaseCButton(int cbutton) {
    controllerState[cbutton] = false;
}

/*
 * General
 */
bool InputManager::UpPressed(float delay) {
    bool upPressed = INPUT_MANAGER.KeyPress(UP_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_w) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_UP);
    if (!upPressed && INPUT_MANAGER.HasController() & joystickDirTimer.Get() > delay) {
        Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK, 15000);
        upPressed = leftAxis.y < 0;
        if (upPressed) joystickDirTimer.Restart();
    }
    return upPressed;
}

bool InputManager::DownPressed(float delay) {
    bool downPressed = INPUT_MANAGER.KeyPress(DOWN_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_s) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    if (!downPressed && INPUT_MANAGER.HasController() && joystickDirTimer.Get() > delay) {
        Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK, 15000);
        downPressed = leftAxis.y > 0;
        if (downPressed) joystickDirTimer.Restart();
    }
    return downPressed;
}

bool InputManager::LeftPressed(float delay) {
    bool leftPressed = INPUT_MANAGER.KeyPress(LEFT_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_a) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    if (!leftPressed && INPUT_MANAGER.HasController() && joystickDirTimer.Get() > delay) {
        Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK, 15000);
        leftPressed = leftAxis.x < 0;
        if (leftPressed) joystickDirTimer.Restart();
    }
    return leftPressed;
}

bool InputManager::RightPressed(float delay) {
    bool rightPressed = INPUT_MANAGER.KeyPress(RIGHT_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_d) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    if (!rightPressed && INPUT_MANAGER.HasController() && joystickDirTimer.Get() > delay) {
        Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK, 15000);
        rightPressed = leftAxis.x > 0;
        if (rightPressed) joystickDirTimer.Restart();
    }
    return rightPressed;
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
