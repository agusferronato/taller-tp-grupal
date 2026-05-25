target_compile_definitions(taller_tests PRIVATE TESTING)

target_sources(taller_tests
    PRIVATE
        protocol_tests.cpp
        collision_tests.cpp
        persistence_tests.cpp
)
