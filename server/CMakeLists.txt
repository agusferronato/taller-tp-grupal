file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE ALL_INCLUDE_DIRS LIST_DIRECTORIES true
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*"
)
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
foreach(DIR ${ALL_INCLUDE_DIRS})
    if(IS_DIRECTORY ${DIR})
        list(APPEND INCLUDE_DIRS ${DIR})
    endif()
endforeach()

target_sources(taller_server_lib
    PRIVATE
        ${LIB_SOURCES}
)
target_sources(taller_server
    PRIVATE
        main.cpp
)
target_include_directories(taller_server_lib
    PUBLIC
        ${INCLUDE_DIRS}
)
target_include_directories(taller_server
    PUBLIC
        ${INCLUDE_DIRS}
)