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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DTO
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DTO/Commands
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DTO/Events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/include/exceptions
)
