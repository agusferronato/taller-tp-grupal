file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(taller_common
    PRIVATE
        ${SOURCES}
)

target_include_directories(taller_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)