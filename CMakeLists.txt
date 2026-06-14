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
  ${CMAKE_CURRENT_SOURCE_DIR}/map.toml
  ${CMAKE_CURRENT_BINARY_DIR}/map.toml
  COPYONLY
)

# Client and editor dependencies are installed by install.sh and resolved here
# as system packages. Keep network/download work out of CMake configuration.
if(TALLER_CLIENT OR TALLER_EDITOR)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
    find_package(SDL2 REQUIRED CONFIG)
    find_package(SDL2_image REQUIRED CONFIG)
    find_package(SDL2_mixer REQUIRED CONFIG)
    find_package(SDL2_ttf REQUIRED CONFIG)
    find_package(SDL2pp REQUIRED CONFIG)
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
