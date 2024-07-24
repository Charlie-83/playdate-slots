cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(SDK $ENV{PLAYDATE_SDK_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(PLAYDATE_GAME_NAME slots)
set(PLAYDATE_GAME_DEVICE slots_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

set(SOURCES
    source/main.c
    source/game.c
    source/inventory.c
    source/slots.c
    source/utils.c
)

if (TOOLCHAIN STREQUAL "armgcc")
    add_executable(${PLAYDATE_GAME_DEVICE} ${SOURCES})
else()
    add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCES})
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)
