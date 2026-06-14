# Argentum Online
Trabajo Práctico de **Taller de Programación I (75.42)** — FIUBA
1er Cuatrimestre 2026

## Integrantes
| Nombre                        | Padrón   |
|-------------------------------|----------|
| Thiago Reller                 | 111067   |
| Federico Tomás Parsons        | 111250   |
| Agustín Atilio Ferronato      | 111991   |
| Sara Lucia Galeano            | 112120   |

## Compilación
Ubicarse en la raiz del proyecto y ejecutar:
```bash
# Instalar dependencias
./install.sh
# Configurar (solo la primera vez o si cambia CMakeLists.txt)
mkdir -p build && cmake -S . -B build/ -G Ninja
# Compilar
cmake --build build -j$(nproc)
```

## Ejecución
Moverse al **directorio de build** y ejecutar
```bash
# Editor de mapas
./taller_editor
# Servidor (requiere un mapa)
./taller_server <puerto> <ruta_del_mapa>
# Cliente gráfico
./taller_client <host> <puerto>
# Tests unitarios
./taller_tests
```
Por ejemplo, desde el directorio build:
`./taller_server 8080 map.toml` para crear el server con el mapa de map.toml.
`./taller_client localhost 8080` para conectarse al server creado.

Si no se hace por default, se recomienda forzar el uso de X11 sobre Wayland, ya que este ultimo presenta leaks y otros problemas relacionados con SDL/Qt. Para ello, se puede forzar escribiendo `QT_QPA_PLATFORM=xcb` para QT y `SDL_VIDEODRIVER=x11` para SDL, antes del comando. Por ejemplo `QT_QPA_PLATFORM=xcb SDL_VIDEODRIVER=x11 ./taller_editor`.

En caso de correr Valgrind, se provee un archivo de supresiones en la raiz del proyecto, llamado `valgrind.supp`. De esta manera se suprimen reportes de memoria conocidos provenientes de Qt, SDL y bibliotecas relacionadas.

Adicionalmente, tambien se provee un mapa de prueba en la raiz del proyecto, hecho con el editor. Para ejecutar el server con este mapa, correr `./taller_server 8080 ../mapa_de_prueba.toml`

## Manuales
La documentación se encuentra en tres archivos PDF:
-  Manual de Usuario (ManualUsuario.tex): Guía de juego con todos los controles, comandos de chat, interacción con NPCs, sistema de clanes, uso del inventario, y explicación de las mecánicas (muerte, combate, ciudades, etc.).

- Manual de Proyecto (ManualProyecto.tex): Documenta el proceso de desarrollo: organización del equipo, herramientas utilizadas, evolución semana a semana, dificultades encontradas, lecciones aprendidas y estado actual de las funcionalidades(que esta completado y que falta).

- Documentación Técnica (DocumentacionTecnica.tex): Arquitectura del cliente y servidor, modelo de hilos, protocolo de comunicación binario (opcodes y formato de mensajes), formato de persistencia (index.dat / players.dat), algoritmo de renderizado (Painter's algorithm), cálculo de colisiones por canal alfa, y descripción del editor de mapas.
