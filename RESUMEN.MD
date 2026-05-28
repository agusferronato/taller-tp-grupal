Resumen del Proyecto — Taller 1
Arquitectura General
Juego multijugador 2D estilo Argentum Online con:
- 
Servidor (server/): game loop, colisiones, persistencia, fórmulas, eventos
- 
Cliente (client/): renderizado SDL2, HUD, manejo de eventos del servidor
- 
Común (common/): DTOs compartidos, protocolo de red, parsers, constantes
La red usa un protocolo binario propio sobre TCP, con opcodes para comandos (cliente→servidor, 0x10–0x15) y eventos (servidor→cliente, 0x80–0x89).
Módulo 1 — Registro con Raza y Clase
Archivos modificados
Archivo	Cambio
common/include/DTO/Commands/RegisterPlayerCommandDTO.h	Se añadió playerClass al DTO
common/src/parser/Commands/RegisterPlayerCommandParser.cpp	Serializa/deserializa playerClass
server/include/command/RegisterPlayerCommand.h	Constructor con 3 parámetros (name, race, playerClass)
server/src/command/RegisterPlayerCommand.cpp	Pasa name, race, playerClass al Game::registerPlayer()
server/src/command/CommandFactory.cpp	Crea RegisterPlayerCommand con 3 args
client/src/pages/CharacterCreationPage.cpp	Cambio "Arquero" → "Clerigo"
Decisión: Se eliminó password del registro (innecesario para el MVP). El servidor recibe name, race, playerClass.
Módulo 2 — Stats Base (Fuerza, Agilidad, Constitución, Inteligencia)
Archivos modificados
Archivo	Cambio
server/include/Game.h (PlayerInfo)	Añadidos campos: level, hp, maxHp, mana, maxMana, experience, gold, strength, agility, constitution, intelligence
server/include/PlayerData.h	Mismos 11 campos nuevos
server/src/PlayerRepository.cpp	writeData() y readData() serializan/deserializan los 11 campos
server/src/Game.cpp	Función initPlayerStats() que calcula stats base según raza + clase
Fórmulas de stats base (hardcodeadas)
Raza → stats base:
Raza	Fuerza	Agilidad	Constitución	Inteligencia
Elfo	6	13	5	16
Enano	13	4	16	7
Gnomo	7	6	14	13
(default)	10	10	10	10
Clase → stats base:
Clase	Fuerza	Agilidad	Constitución	Inteligencia
Mago	3	5	5	15
Clerigo	7	7	9	10
Paladin	10	6	10	8
(default/Guerrero)	10	8	10	3
Stat final = stat_raza + stat_clase. Se calcula al crear/login del personaje.
Módulo 3 — Clase Formulas (server)
Archivos nuevos
Archivo	Propósito
server/include/Formulas.h	Header con todos los métodos estáticos
server/src/Formulas.cpp	Implementación de todas las ecuaciones
Ecuaciones implementadas
Fórmula	Ecuación
calcularVidaMax(con, raza, clase, nivel)	(con + 15) * factor_raza * factor_clase + nivel * 2
calcularManaMax(intel, raza, clase, nivel)	intel * factor_raza * factor_clase + nivel (Guerrero → 0)
calcularRecuperacionVida(con, nivel)	max(1, con / 5 + nivel / 10)
calcularRecuperacionMana(intel, nivel)	max(0, intel / 5 + nivel / 10) (Guerrero → 0)
calcularMeditacion(intel, nivel)	max(0, intel / 2 + nivel) (Guerrero → 0)
calcularOroMax(nivel)	nivel * 1000 + 500
calcularOroExceso(oro, nivel)	max(0, oro - oroMax)
calcularLimiteXP(nivel)	pow(nivel, 1.5) * 1000
calcularExperienciaGanar(nivel_atacante, nivel_victima)	max(1, nivel_victima * 50 - nivel_atacante * 10)
calcularExperienciaPerder(nivel)	max(0, nivel * 200)
calcularDañoFisico(fuerza, agilidad, arma)	(fuerza + agilidad / 2) * factor_clase + arma - defensor_Defensa
calcularEsquivo(agilidad)	agilidad * 0.5 + nivel / 10
calcularCritico(agilidad)	agilidad * 0.3 + nivel / 20
calcularDefensa(constitucion)	constitucion * 2 + nivel
calcularOroDropNPC(nivel)	nivel * (50 + rand() % 151)
Factores hardcodeados por raza
- 
Vida: Elfo 0.9, Enano 1.3, Gnomo 1.1, default 1.0
- 
Mana: Elfo 1.3, Enano 0.7, Gnomo 1.1, default 1.0
Factores hardcodeados por clase
- 
Vida: Mago 0.7, Clerigo 0.9, Paladin 1.2, Guerrero 1.1
- 
Mana: Mago 2.0, Clerigo 1.5, Paladin 0.5, Guerrero 0.0
- 
Daño: Mago 0.7, Clerigo 0.9, Paladin 1.2, Guerrero 1.0
Tests (tests/formulas_tests.cpp)
18 tests que cubren todas las fórmulas, incluyendo:
- 
Guerrero siempre tiene mana 0
- 
Meditación de Guerrero es 0
- 
Oro exceso funciona con/sin exceso
- 
Aleatoriedad recibe valor externo (testeable)
Módulo 4 — Sincronizar Stats al Cliente
Flujo de datos
Servidor (Game.cpp)
  └─ registerPlayer() / loginPlayer()
       ├─ PlayerListEventDTO  → lista completa de jugadores con stats
       └─ PlayerAppearedEventDTO → stats del nuevo jugador
  └─ game loop (cada 60 ticks)
       └─ PlayerInfoEventDTO → actualización periódica de stats
Cliente (GameModel.cpp)
  └─ playerAppeared() → crea/actualiza Player con stats
  └─ registerPlayers() → actualiza todos los players existentes
  └─ playerInfoUpdated() → actualiza stats (cada 60 ticks)
Archivos modificados/creados
Archivo	Cambio
common/include/DTO/Events/PlayerAppearedEventDTO.h	Añadidos: hp, maxHp, mana, maxMana, gold, level, experience
common/include/DTO/Events/PlayerListEventDTO.h	PlayerInfoDTO con los mismos 7 campos
common/include/DTO/Events/PlayerInfoEventDTO.h	Nuevo — DTO con opcode 0x86
common/include/DTO/Events/EventDTO.h	PlayerInfoEventDTO añadido al variant
common/src/parser/Events/PlayerAppearedEventParser.cpp	Serializa/deserializa los 7 campos
common/src/parser/Events/PlayerListEventParser.cpp	Serializa/deserializa los 7 campos
common/src/parser/Events/PlayerInfoEventParser.h/.cpp	Nuevos — parser para el evento periódico
common/src/protocol/RegisterAllParsers.cpp	Registra PlayerInfoEventParser
client/include/Player.h	Setters/getters: hp, maxHp, mana, maxMana, gold, level, experience
client/include/GameModel.h	playerInfoUpdated()
client/src/GameModel.cpp	Maneja PlayerAppearedEvent, PlayerListEvent, PlayerInfoEvent
server/src/Game.cpp	Rellena stats en eventos + envía PlayerInfoEvent cada 60 ticks
Protocolo de red
PlayerInfoEvent (0x86, servidor→cliente):
[opcode:1B] [playerId:4B] [hp:4B] [maxHp:4B] [mana:4B] [maxMana:4B]
[gold:4B] [level:4B] [experience:4B]
Decisiones
- 
Los stats viajan en tres tipos de eventos: PlayerAppearedEvent (al aparecer), PlayerListEvent (lista inicial), PlayerInfoEvent (actualización periódica cada 60 ticks)
- 
Guerrero tiene FClaseMana = 0.0 → mana siempre 0
- 
PlayerInfoEvent no se envía al recibir daño/tirar oro (solo loop periódico)
Módulo 5 — HUD (Interfaz de Jugador)
Archivos modificados
Archivo	Cambio
client/include/GameWindow.h	Añadidos renderHUD(), headPathForRace()
client/src/GameWindow.cpp	Renderizado de HUD + carga de recursos + texturas de cabezas
client/src/Gameloop.cpp	Llama a renderHUD() en cada frame
Elementos del HUD
- 
Barra de HP (rojo 200,40,40 / fondo 60,10,10) — arriba izquierda
- 
Barra de Maná (azul 40,80,220 / fondo 10,20,60)
- 
Barra de XP (verde 60,200,60 / fondo 10,50,10) — usa pow(level, 1.5) * 1000 como límite
- 
Texto: nivel, HP/HP, Maná/Maná al lado de cada barra
- 
Oro: esquina superior derecha
Carga de recursos
- 
Assets: ruta relativa ../client/assets/... (se ejecuta desde build/)
- 
Fuente: DejaVuSans del sistema (/usr/share/fonts/truetype/dejavu/) con fallback a ../client/fonts/Vera.ttf
- 
Texturas de cabezas por raza: ../client/assets/cabezas/<raza>.png
Función auxiliar
static std::string assetPath(const std::string &relative) {
    return "../client/" + relative;
}
Módulo 6 — Nombre sobre Jugadores
Archivos modificados
Archivo	Cambio
common/include/DTO/Events/PlayerAppearedEventDTO.h	Añadido playerName
common/include/DTO/Events/PlayerListEventDTO.h	Añadido playerName a PlayerInfoDTO
common/src/parser/Events/PlayerAppearedEventParser.cpp	Serializa/deserializa playerName
common/src/parser/Events/PlayerListEventParser.cpp	Serializa/deserializa playerName
tests/protocol_tests.cpp	Test actualizado con nombres
server/src/Game.cpp	Envía name en ambos eventos
client/include/Player.h	Añadido name + setName()/getName()
client/src/GameModel.cpp	Aplica playerName al Player
client/src/GameWindow.cpp	Renderiza nombre blanco centrado encima de cada cabeza
Renderizado
El nombre se dibuja centrado sobre la cabeza del personaje con fuente blanca (255,255,255), ubicado justo encima de la textura de la cabeza.
Tests
46 tests en total:
Suite	Tests	Descripción
ProtocolTest	14	Serialización/deserialización de todos los DTOs y parsers
CollisionTest	7	Colisiones entre jugadores
PersistenceTest	7	Crear, guardar, cargar personajes
FormulasTest	18	Todas las ecuaciones del juego
Bugs Corregidos
1. 
Segfault al crear personaje (server/src/Game.cpp:212): player->hp después de std::move(player) (puntero nulo). Cambiado a players[newId]->hp.
Issues Conocidos / Decisiones Técnicas
- 
No hay password en el registro (MVP simplificado)
- 
Los assets se leen desde ../client/assets/ porque la ejecución es desde build/
- 
No existe directorio assets/ en raíz — solo client/assets/
- 
El HUD no tiene texturas decorativas ni marco (solo barras de color sólido)
- 
Todos los métodos de Formulas con aleatoriedad reciben el valor random como parámetro (testeable, determinístico)
- 
El envío de PlayerInfoEvent es periódico (60 ticks), no por evento (daño, oro, etc.)