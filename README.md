# Configurar (solo la primera vez o si cambias CMakeLists.txt)
cd build && cmake ..
# Compilar todo
cd build && make -j$(nproc)
# Correr server
./build/taller_server
# Correr cliente (otra terminal)
./build/taller_client
# Correr editor
./build/taller_editor
# Correr tests
./build/taller_tests
