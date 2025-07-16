#include "hud/FlashlightHUD.h"
#include "core/Game.h"

//#define DEBUG_FLASHLIGHT

SDL_BlendMode invertAlphaBM = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
SDL_BlendMode objectsBM = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);


FlashlightHUD::FlashlightHUD(GameObject& associated) : Component(associated), backlight("Recursos/img/lighting/backlight_inv.png", 1, 1, true) {
    isDark = true;
    flashlightOn = true;
    angle = 0;

    SDL_BlendMode bm = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
    int bmRet = SDL_SetTextureBlendMode(backlight.texture, bm);
    if (bmRet != 0) std::cout << "[Flashlight] Error on SDL_SetTextureBlendMode: " << SDL_GetError() << std::endl;

    texture = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    supportTexture = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetTextureBlendMode(supportTexture, SDL_BLENDMODE_BLEND);

#ifdef DEBUG_FLASHLIGHT
    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(GAME_RENDERER, &rendererInfo);
    std::cout << "[Flashlight] " << rendererInfo.name << " | " << rendererInfo.flags << std::endl;
    std::cout << "[Flashlight] Texture formats: ";
    for (int i = 0; i <= rendererInfo.num_texture_formats; i++) {
        std::cout << rendererInfo.texture_formats[i] << "; ";
    }
    std::cout << std::endl;
#endif
}

FlashlightHUD::~FlashlightHUD() {}

void FlashlightHUD::Update(float dt) {
    if (Character::player == nullptr ||
        !isDark ||
        CURRENT_STATE.openUI ||
        !INVENTORY->HasItemInHand(ITEM_LANTERNA)
    ) return;

    // Use flashlight (already know flashlight's in hand based on the check above)
    if (USE_CHECK) ToggleFlashlight();

    if (!flashlightOn) return;
    if (INPUT_MANAGER.HasController()) {
        Vec2 rightJoy = INPUT_MANAGER.ControllerAxis(RIGHT_JOYSTICK);
        if (rightJoy.x != 0 || rightJoy.y != 0) angle = rightJoy.Angle() + M_PI * 0.5;
    } else {
        Vec2 origin = Character::player->associated.box.Center() - Camera::pos;
        Vec2 mousePos = Vec2(INPUT_MANAGER.GetMouseX(), INPUT_MANAGER.GetMouseY());
        angle = mousePos.Angle(origin) + M_PI * 0.5;
    }
}

void FlashlightHUD::Render() {
    if (Character::player == nullptr || !isDark) return;

    Vec2 origin = Character::player->associated.box.Center() - Camera::pos;
    int BLoffsetX = backlight.GetWidth() * 0.5;
    int BLoffsetY = backlight.GetHeight() * 0.5;

    // Support texture
    SDL_SetRenderTarget(GAME_RENDERER, supportTexture);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(GAME_RENDERER);

    // Main texture
    SDL_SetRenderTarget(GAME_RENDERER, texture);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(GAME_RENDERER);

    // Backlight
    backlight.Render(origin.x - BLoffsetX, origin.y - BLoffsetY, backlight.GetWidth(), backlight.GetHeight());

    // Flashlight cone
    if (INVENTORY->HasItemInHand(ITEM_LANTERNA) && flashlightOn) {
        int flashlightSize = WINDOW_WIDTH * 0.5;
        float flashlightAngle = M_PI / 8;
        Vec2 flLeft = Vec2(origin.x + flashlightSize * sin(angle - flashlightAngle), origin.y - flashlightSize * cos(angle - flashlightAngle));
        Vec2 flRight = Vec2(origin.x + flashlightSize * sin(angle + flashlightAngle), origin.y - flashlightSize * cos(angle + flashlightAngle));
        std::vector<SDL_Vertex> lightVertices = {
            origin.ToSDLVertex(255, 255, 255, 255, 0.5, 0.5),
            flLeft.ToSDLVertex(255, 255, 255, 255, 1, 0.3),
            flRight.ToSDLVertex(255, 255, 255, 255, 1, 0.7)
        };
        SDL_RenderGeometry(GAME_RENDERER, backlight.texture, lightVertices.data(), lightVertices.size(), nullptr, 0);
    }

    // Render objects above flashlight
    std::vector<GameObject*> objArray = CURRENT_STATE.RenderSort(0);
    for (int i = 0; i < objArray.size(); i++) {
        GameObject* obj = objArray[i];
        if (obj->box.z == 0 && Camera::PosRect().Collides(obj->box)) {
            if (!CURRENT_STATE.dependsOn(&Character::player->associated, obj)) {
                // Player is behind this obj
#ifdef DEBUG_FLASHLIGHT
                std::cout << "[Flashlight] Player is behind " << obj->name << std::endl;
#endif
                IsoCollider* objCol = (IsoCollider*) obj->GetComponent("IsoCollider");
                SpriteRenderer* objSR = (SpriteRenderer*) obj->GetComponent("SpriteRenderer");
                if (objSR && objSR->sprite.IsOpen() && objCol && objCol->blockLight) {
                    IsoCollider* playerCol = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
                    float diffX = abs(playerCol->box.TopLeft().x - objCol->box.TopRight().x);
                    float diffY = abs(playerCol->box.TopLeft().y - objCol->box.BottomLeft().y);
                    float minDiff = std::min(diffX, diffY);

                    int newAlpha = std::min(minDiff / 100, 1.0f) * 255;

                    // Save old values
                    Uint8 r, g, b, a;
                    int getCMRet = SDL_GetTextureColorMod(objSR->sprite.texture, &r, &g, &b);
                    if (getCMRet != 0) std::cout << "[Flashlight] Error on SDL_GetTextureColorMod: " << SDL_GetError() << std::endl;
                    int getAMRet = SDL_GetTextureAlphaMod(objSR->sprite.texture, &a);
                    if (getAMRet != 0) std::cout << "[Flashlight] Error on SDL_GetTextureAlphaMod: " << SDL_GetError() << std::endl;

                    // Render all black sprite
                    int setCMRet = SDL_SetTextureColorMod(objSR->sprite.texture, 0, 0, 0);
                    if (setCMRet != 0) std::cout << "[Flashlight] Error on SDL_SetTextureColorMod: " << SDL_GetError() << std::endl;
                    int setAMRet = SDL_SetTextureAlphaMod(objSR->sprite.texture, newAlpha);
                    if (setAMRet != 0) std::cout << "[Flashlight] Error on SDL_SetTextureAlphaMod: " << SDL_GetError() << std::endl;

                    // Render texture
                    int setBMRet = SDL_SetTextureBlendMode(objSR->sprite.texture, objectsBM);
                    if (setBMRet != 0) std::cout << "[Flashlight] Error on SDL_SetTextureBlendMode objectsBM: " << SDL_GetError() << std::endl;
                    objSR->Render();
                    SDL_SetTextureBlendMode(objSR->sprite.texture, SDL_BLENDMODE_BLEND);

                    // Restore old values
                    SDL_SetTextureColorMod(objSR->sprite.texture, r, g, b);
                    SDL_SetTextureAlphaMod(objSR->sprite.texture, a);
                }
            }

            LightEmitter* lightEmit = (LightEmitter*) obj->GetComponent("LightEmitter");
            if (lightEmit != nullptr) {
                for (int j = 0; j < lightEmit->lightPoints.size(); j++) {
                    if (lightEmit->IsEnabled(j)) {
                        Vec2 lightPos = lightEmit->GetPos(j);
                        Sprite lightSprite = lightEmit->GetSprite(j);
                        lightSprite.Render(lightPos.x - lightSprite.GetWidth() * 0.5, lightPos.y - lightSprite.GetHeight() * 0.5, lightSprite.GetWidth(), lightSprite.GetHeight());
                    }
                }
            }
        }
    }

    // Render custom lighting layer
    SDL_Rect screenRect = WINDOW_RECT;
    SDL_SetRenderTarget(GAME_RENDERER, supportTexture);
    int setBMInv = SDL_SetTextureBlendMode(texture, invertAlphaBM);
    if (setBMInv != 0) std::cout << "[Flashlight] Error on SDL_SetTextureBlendMode invertAlphaBM: " << SDL_GetError() << std::endl;
    SDL_RenderCopy(GAME_RENDERER, texture, &screenRect, &screenRect);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME_RENDERER, nullptr);
    SDL_RenderCopy(GAME_RENDERER, supportTexture, &screenRect, &screenRect);
}

bool FlashlightHUD::Is(std::string type) {
    return type == "FlashlightHUD";
}

bool FlashlightHUD::IsDark() {
    return isDark;
}

bool FlashlightHUD::IsFlashlightOn() {
    return flashlightOn;
}

void FlashlightHUD::SetDark(bool b) {
    isDark = b;
}

void FlashlightHUD::SetFlashlight(bool b) {
    flashlightOn = b;
}

void FlashlightHUD::ToggleDarkMode() {
    isDark = !isDark;
}

void FlashlightHUD::ToggleFlashlight() {
    flashlightOn = !flashlightOn;
}
