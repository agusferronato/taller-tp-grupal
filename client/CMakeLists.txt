file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(taller_client
    PRIVATE
        ${SOURCES}
    PUBLIC
        main.cpp
)

target_include_directories(taller_client
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)