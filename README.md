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
```bash
# Configurar (solo la primera vez o si cambia CMakeLists.txt)
mkdir -p build && cd build && cmake ..
# Compilar
cmake --build . -j$(nproc)
Ejecución
# Servidor (requiere un mapa)
./build/taller_server <ruta_del_mapa>
# Cliente gráfico
./build/taller_client
# Editor de mapas
./build/taller_editor
# Tests unitarios
./build/taller_tests
```
Manuales
La documentación se encuentra en tres archivos PDF:
- 
Manual de Usuario (ManualUsuario.tex): Guía de juego con todos
los controles, comandos de chat, interacción con NPCs, sistema de
clanes, uso del inventario, y explicación de las mecánicas (muerte,
combate, ciudades, etc.).
- 
Manual de Proyecto (ManualProyecto.tex): Documenta el proceso
de desarrollo: organización del equipo, herramientas utilizadas,
evolución semana a semana, dificultades encontradas, lecciones
aprendidas y estado actual de las funcionalidades(que esta completado y que falta).
- 
Documentación Técnica (DocumentacionTecnica.tex): Arquitectura
del cliente y servidor, modelo de hilos, protocolo de comunicación
binario (opcodes y formato de mensajes), formato de persistencia
(index.dat / players.dat), algoritmo de renderizado (Painter's
algorithm), cálculo de colisiones por canal alfa, y descripción del
editor de mapas.
