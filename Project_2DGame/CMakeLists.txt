cmake_minimum_required(VERSION 3.24)
project(Project)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(SFML)

add_executable(Project main.cpp
        Game.cpp
        Game.h
        Character.cpp
        Character.h
        Collectibles.cpp
        Collectibles.h
        Cup.cpp
        Cup.h
        GameMenu.cpp
        GameMenu.h
        Enemy.cpp
        Enemy.h
        CheshireCat.cpp
        CheshireCat.h
        Queen.cpp
        Queen.h
        CardKnight.cpp
        CardKnight.h
        Mushroom.cpp
        Mushroom.h
)
target_link_libraries(Project sfml-graphics)
target_link_libraries(Project fmt)

