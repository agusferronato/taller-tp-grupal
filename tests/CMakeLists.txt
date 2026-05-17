target_compile_definitions(taller_tests PRIVATE TESTING)

target_sources(taller_tests
    PRIVATE
        protocol_tests.cpp

        ../client/src/client_protocol.cpp
        ../client/src/client_encoder.cpp
        ../client/src/client_decoder.cpp

        ../server/src/server_protocol.cpp
        ../server/src/server_encoder.cpp
        ../server/src/server_decoder.cpp
)
