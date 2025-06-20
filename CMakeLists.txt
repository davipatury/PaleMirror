cmake_minimum_required(VERSION 3.16)

project(PaleMirror LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
include_directories(include C:/SDL-2.30.8/include/SDL2)
link_directories(C:/SDL-2.30.8/lib)
link_libraries(Mingw32 SDL2main SDL2_image SDL2_ttf SDL2_mixer SDL2)

file(GLOB_RECURSE SOURCES 
    "include/core/*.h"
    "include/physics/*.h"
    "include/math/*.h"
    "include/states/*.h"
    "include/actions/*.h"
    "include/rooms/*.h"
    "include/entities/characters/*.h"
    "include/entities/interactive/*.h"
    "include/entities/projectiles/*.h"
    "include/components/*.h"
    "include/utils/*.h"
    "src/core/*.cpp"
    "src/physics/*.cpp"
    "src/math/*.cpp"
    "src/states/*.cpp"
    "src/actions/*.cpp"
    "src/rooms/*.cpp"
    "src/entities/characters/*.cpp"
    "src/entities/interactive/*.cpp"
    "src/entities/projectiles/*.cpp"
    "src/components/*.cpp"
    "src/utils/*.cpp"
    "src/main.cpp"
)

add_executable(PaleMirror ${SOURCES})

file(COPY ${CMAKE_SOURCE_DIR}/Recursos DESTINATION ${CMAKE_INSTALL_BINDIR}/Recursos)

include(GNUInstallDirs)
install(TARGETS PaleMirror
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
