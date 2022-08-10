# CMakeList.txt : CMake project for Margarita, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

set(CMAKE_CXX_STANDARD 17) 

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" )
	set(USE_FLAGS "-fno-rtti -fno-exceptions -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=['png'] -s USE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}  -s ALLOW_MEMORY_GROWTH=1 --preload-file resources/ -lwebsocket.js")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_TTF REQUIRED)
    find_package(SDL2_IMAGE REQUIRED)
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

add_executable (Margarita  Animation.cpp  Bomb.cpp BombAnimation.cpp Coin.cpp Comet.cpp ControlsScreen.cpp Crab.cpp DeathScreen.cpp Enemy.cpp EnergyPowerup.cpp Entity.cpp EntityHealthChangeText.cpp  EventHandler.cpp Game.cpp Graphics.cpp GUI.cpp Item.cpp  Laser.cpp  main.cpp Orb.cpp PauseScreen.cpp Player.cpp Powerup.cpp RegenerationPowerup.cpp Shop.cpp Space.cpp   TextObject.cpp Texture.cpp TitleScreen.cpp Utilities.cpp "Turret.cpp" "Turret.h" "ControlManager.cpp" "ControlManager.h" "Upgrades.h" "Upgrades.cpp" "Block.cpp" "Block.h" "AirStrike.cpp" "AirStrike.h" "Particle.cpp" "Particle.h" "Multiplayer.cpp")

target_link_libraries(Margarita ${SDL2_LIBRARIES})
target_link_libraries(Margarita ${SDL2_TTF_LIBRARIES})
target_link_libraries(Margarita ${SDL2_IMAGE_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" )

else()
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_PROJECT_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()



# TODO: Add tests and install targets if needed.
