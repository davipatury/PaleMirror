cmake_minimum_required(VERSION 3.16)

project(PaleMirror LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
include_directories(include C:/SDL-2.30.8/include/SDL2)
link_directories(C:/SDL-2.30.8/lib)
link_libraries(Mingw32 SDL2main SDL2_image SDL2_ttf SDL2_mixer SDL2)

add_executable(PaleMirror src/main.cpp
    include/Game.h src/Game.cpp
    include/State.h src/State.cpp
    include/Music.h src/Music.cpp
    include/Sprite.h src/Sprite.cpp
    include/Vec2.h src/Vec2.cpp
    include/Rect.h src/Rect.cpp
    include/Component.h src/Component.cpp
    include/GameObject.h src/GameObject.cpp
    include/Animation.h src/Animation.cpp
    include/SpriteRenderer.h src/SpriteRenderer.cpp
    include/Zombie.h src/Zombie.cpp
    include/Animator.h src/Animator.cpp
    include/TileSet.h src/TileSet.cpp
    include/TileMap.h src/TileMap.cpp
    include/Sound.h src/Sound.cpp
    include/Resources.h src/Resources.cpp
    include/InputManager.h src/InputManager.cpp
    include/Timer.h src/Timer.cpp
    include/Camera.h src/Camera.cpp
    include/Character.h src/Character.cpp
    include/Gun.h src/Gun.cpp
    include/PlayerController.h src/PlayerController.cpp
    include/Bullet.h src/Bullet.cpp
    include/Collision.h src/Collision.cpp
    include/Collider.h src/Collider.cpp
    include/Wave.h src/Wave.cpp
    include/WaveSpawner.h src/WaveSpawner.cpp
    include/AIController.h src/AIController.cpp
    include/StageState.h src/StageState.cpp
    include/TitleState.h src/TitleState.cpp
    include/Text.h src/Text.cpp
    include/EndState.h src/EndState.cpp
    include/GameData.h src/GameData.cpp
    include/HealthHUD.h src/HealthHUD.cpp
    include/IsoRect.h src/IsoRect.cpp
    include/Line.h src/Line.cpp
    include/IsoCollider.h src/IsoCollider.cpp
    include/Ray.h src/Ray.cpp
    include/ShadowCaster.h src/ShadowCaster.cpp
    include/MirrorPuzzle.h src/MirrorPuzzle.cpp
)

include(GNUInstallDirs)
install(TARGETS PaleMirror
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
