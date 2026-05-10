target_sources(taller_client
    PRIVATE
        main.cpp
        src/Player.cpp
        src/Display.cpp
        src/Background.cpp
    PUBLIC
        include/Player.h
        include/Display.h
        include/Background.h
)

target_include_directories(taller_client PRIVATE include)
