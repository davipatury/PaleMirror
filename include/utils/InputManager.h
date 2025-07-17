#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

// Keyboard keys
#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define SPACE_KEY SDLK_SPACE

// Mouse buttons
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT

// Controller
#define LEFT_JOYSTICK 0
#define RIGHT_JOYSTICK 1
#define JOYSTICK_MAX_VALUE 32767.0f
#define JOYSTICK_DEADZONE 2000.0f
#define JOYSTICK_DIR_DELAY 0.15f

// Default values
#define INTERACT_CHECK  (INPUT_MANAGER.KeyPress(SDLK_e)      || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_A))
#define USE_CHECK       (INPUT_MANAGER.KeyPress(SDLK_SPACE)  || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_X))
#define BACK_CHECK      (INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_B))
#define ESCAPE_CHECK    (INPUT_MANAGER.KeyPress(SDLK_ESCAPE) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_START))
#define CONFIRM_CHECK   (INPUT_MANAGER.KeyPress(SDLK_SPACE)  || INPUT_MANAGER.KeyPress(SDLK_RETURN) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_A))
#define UP_CHECK        (INPUT_MANAGER.UpPressed())
#define DOWN_CHECK      (INPUT_MANAGER.DownPressed())
#define LEFT_CHECK      (INPUT_MANAGER.LeftPressed())
#define RIGHT_CHECK     (INPUT_MANAGER.RightPressed())

#define INPUT_MANAGER InputManager::GetInstance()

#include "SDL.h"
#include "math/Vec2.h"
#include "utils/Timer.h"

class InputManager
{
public:
    void Update(float dt);

    // Keyboard
    bool AnyKeyPress();
    bool KeyPress(int key);
    bool KeyRelease(int key);
    bool IsKeyDown(int key);
    void ReleaseKey(int key);

    // Mouse
    bool MousePress(int button);
    bool MouseRelease(int button);
    bool IsMouseDown(int button);
    int GetMouseX();
    int GetMouseY();
    Vec2 GetMousePos();
    int GetMouseWheel();

    // Controller
    bool HasController();
    bool CButtonPress(int button);
    bool CButtonRelease(int button);
    bool IsCButtonDown(int button);
    Vec2 ControllerAxis(int joystick, float deadzone = JOYSTICK_DEADZONE);

    // General
    bool UpPressed(float delay = JOYSTICK_DIR_DELAY);
    bool DownPressed(float delay = JOYSTICK_DIR_DELAY);
    bool LeftPressed(float delay = JOYSTICK_DIR_DELAY);
    bool RightPressed(float delay = JOYSTICK_DIR_DELAY);

    bool QuitRequested();

    static InputManager& GetInstance();
private:
    InputManager();
    ~InputManager();

    int KeyToIndex(int key);
    float ParseAxis(int value, float deadzone);
    void ConfigureController();

    bool mouseState[6];
    int mouseUpdate[6];
    bool keyState[416];
    int keyUpdate[416];
    bool controllerState[SDL_CONTROLLER_BUTTON_MAX];
    int controllerUpdate[SDL_CONTROLLER_BUTTON_MAX];

    bool quitRequested;
    int updateCounter;

    int mouseX;
    int mouseY;
    int mouseWheel;

    SDL_GameController* controller;
    Timer joystickDirTimer;
    bool anyKeyPress;
};

#endif // INPUTMANAGER_H
