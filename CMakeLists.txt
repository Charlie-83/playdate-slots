cmake_minimum_required(VERSION 3.5)
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


file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=7b354f3a5976c4626c876850c93944e52c83ec59a159ae5de5be7983f0e17a2a
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:DaveGamble/cJSON@1.7.18")
if (cJSON_ADDED)
    add_library(cjson_ INTERFACE)
    target_link_libraries(cjson_ INTERFACE cjson)
    target_include_directories(cjson_ INTERFACE ${cJSON_SOURCE_DIR})
endif()

set(SOURCES
    source/main.c
    source/game.c
    source/inventory.c
    source/slots.c
    source/utils.c
    source/battle.c
)

if (TOOLCHAIN STREQUAL "armgcc")
    message("Compiling for device")
    add_executable(${PLAYDATE_GAME_DEVICE} ${SOURCES})
else()
    message("Compiling for simulator")
    add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCES})
    target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE cjson_)
endif()


include(${SDK}/C_API/buildsupport/playdate_game.cmake)
