cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

option(TALLER_TESTS "Enable / disable tests." ON)
option(TALLER_CLIENT "Enable / disable client program." ON)
option(TALLER_SERVER "Enable / disable server program." ON)
option(TALLER_EDITOR "Enable / disable editor program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

find_package(tomlplusplus REQUIRED)

add_library(taller_common STATIC)

add_subdirectory(common/)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(taller_common ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Public because client, server, editor and tests include headers from the repo root.
target_include_directories(taller_common PUBLIC .)

target_link_libraries(taller_common PUBLIC tomlplusplus::tomlplusplus)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/common/items.toml
  ${CMAKE_CURRENT_BINARY_DIR}/items.toml
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/common/biome_strategy.toml
  ${CMAKE_CURRENT_BINARY_DIR}/biome_strategy.toml
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/common/npc_info.toml
  ${CMAKE_CURRENT_BINARY_DIR}/npc_info.toml
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/common/player_stats.toml
  ${CMAKE_CURRENT_BINARY_DIR}/player_stats.toml
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/common/store_data.toml
  ${CMAKE_CURRENT_BINARY_DIR}/store_data.toml
  COPYONLY
)

# HEY!! TODO XXX: you need to install some runtime and dev libraries *before*
# compiling the client/editor code.
#
# In xubuntu 24.04 these are:
#
# sudo apt-get install libopus-dev libopusfile-dev libxmp-dev libfluidsynth-dev fluidsynth libwavpack1 libwavpack-dev libfreetype-dev wavpack
#

# Install libSDL2 and libSDL2pp (for client or editor)
# HEY!! TODO XXX: add more dependendies (either for client, server or editor)
if(TALLER_CLIENT OR TALLER_EDITOR)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
    include(FetchContent)
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.8
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_Declare(
        libSDL2pp
        OVERRIDE_FIND_PACKAGE
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )

    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.4.0
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(tomlplusplus)

    set(SDL2PP_WITH_IMAGE ON CACHE BOOL "" FORCE)
    set(SDL2PP_WITH_TTF   ON CACHE BOOL "" FORCE)
    set(SDL2PP_WITH_MIXER ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer SDL2_ttf libSDL2pp)
endif()

# Program section
# ---------------
if(TALLER_CLIENT)
    add_executable(taller_client)

    add_dependencies(taller_client taller_common)

    add_subdirectory(client)

    set_project_warnings(taller_client ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_link_libraries(taller_client taller_common SDL2pp::SDL2pp SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf Qt6::Widgets)

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/client/assets
        DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/editor/assets
        DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/client/fonts
        DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(TALLER_SERVER)
    add_library(taller_server_lib STATIC)
    add_executable(taller_server)

    add_dependencies(taller_server taller_common)
    add_dependencies(taller_server_lib taller_common)

    add_subdirectory(server)

    set_project_warnings(taller_server_lib ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
    set_project_warnings(taller_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_compile_definitions(taller_server_lib PRIVATE
        DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    target_compile_definitions(taller_server PRIVATE
        DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

    target_link_libraries(taller_server_lib taller_common)
    target_link_libraries(taller_server taller_server_lib)
endif()

if(TALLER_EDITOR)
    add_executable(taller_editor)

    set_target_properties(taller_editor PROPERTIES AUTOMOC ON)

    add_dependencies(taller_editor taller_common)

    add_subdirectory(editor)

    set_project_warnings(taller_editor ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_link_libraries(taller_editor taller_common SDL2pp::SDL2pp Qt6::Widgets)

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/client/assets
        DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/editor/assets
        DESTINATION ${CMAKE_BINARY_DIR})
endif()


# Testing section
# ---------------

if(TALLER_TESTS)
    if(NOT TALLER_SERVER)
        message(FATAL_ERROR "TALLER_TESTS requires TALLER_SERVER because tests link taller_server_lib.")
    endif()

    enable_testing()

    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_executable(taller_tests)

    add_dependencies(taller_tests taller_common)

    target_include_directories(taller_tests PUBLIC .)

    add_subdirectory(tests/)

    # Tests include helper code from the server and use the same warning policy.
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    target_include_directories(taller_tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/server/include
        )

    target_link_libraries(taller_tests
        taller_common
        taller_server_lib
        GTest::gtest_main
    )
    
    gtest_discover_tests(taller_tests)
endif()
