Estado actual vs. lo que pide el enunciado
✅ Ya funciona
Feature
Razas (Humano/Elfo/Enano/Gnomo)
Clases (Guerrero/Mago/Arquero/Paladín)
HP, Mana, Oro, Nivel, XP
Persistencia
❌ Falta implementar
Agrupado por módulos:
Módulo 1: Enviar clase al servidor (Register)
El RegisterPlayerCommandDTO solo lleva name + race, falta playerClass. Pasos:
1. 
RegisterPlayerCommandDTO.h → agregar std::string playerClass;
2. 
RegisterPlayerCommandParser.cpp → serializar/deserializar playerClass
3. 
CommandFactory.cpp → pasar request->playerClass al RegisterPlayerCommand
4. 
RegisterPlayerCommand.h/cpp → agregar campo playerClass y pasarlo a game.registerPlayer()
5. 
Game.h/cpp → registerPlayer() recibe playerClass, lo asigna a player->playerClass
6. 
CharacterCreationPage.cpp → la UI manda playerClass (ya lo tiene, solo verificar)
7. 
MainWindow.cpp → el onCharacterCreated emite el dato correctamente (ya lo hace)
Módulo 2: Atributos base (Fuerza, Agilidad, Constitución, Inteligencia)
El enunciado dice: VidaMax = Constitución * FClaseVida * FRazaVida * Nivel. No existen estos stats. Hay que agregarlos:
1. 
PlayerData.h → agregar campos int32_t strength, agility, constitution, intelligence;
2. 
PlayerRepository.cpp → agregar los 4 nuevos campos al binario (writeData/readData)
3. 
Game.h (PlayerInfo) → agregar mismos campos en std::string o uint32_t
4. 
Game.cpp (toPlayerData/fromPlayerData) → incluir los nuevos campos
5. 
Game.cpp (registerPlayer) → inicializar stats según raza y clase
Módulo 3: Módulo de fórmulas (obligatorio por enunciado)
El enunciado exige: "cada ecuación en su propio método/función, todas en una única clase/módulo, valores numéricos de un archivo TOML".
1. 
Nuevo archivo: common/include/Formulas.h y common/src/Formulas.cpp
- 
calcularVidaMax(constitucion, clase, raza, nivel)
- 
calcularManaMax(inteligencia, clase, raza, nivel)
- 
calcularOroMax(nivel)
- 
calcularLimiteXP(nivel)
- 
calcularExperiencia(daño, nivelAtacante, nivelVictima)
- 
calcularRecuperacionVida(raza, segundos)
- 
calcularRecuperacionMana(raza, segundos, meditando, inteligencia, clase)
- 
calcularDaño(fuerza, dañoArmaMin, dañoArmaMax)
- 
calcularDefensa(armadura, escudo, casco)
- 
calcularEsquivo(agilidad) → probabilidad
- 
Factores por raza (FRazaVida, FRazaMana, FRazaRecuperacion)
- 
Factores por clase (FClaseVida, FClaseMana, FClaseMeditacion)
- 
Stats base por raza y clase
2. 
Archivo config/game.toml con todos los valores numéricos
3. 
Integrar parser TOML (ej. toml++ header-only via FetchContent o copia)
Módulo 4: Sincronizar HP/Mana/Oro/Stats al cliente
Hoy el servidor nunca envía estos datos al cliente. Hay que:
1. 
PlayerAppearedEventDTO.h → agregar hp, maxHp, mana, maxMana, gold, level, experience
2. 
PlayerAppearedEventParser.cpp → serializar/deserializar nuevos campos
3. 
PlayerListEventDTO.h → agregar mismos campos al PlayerInfoDTO
4. 
PlayerListEventParser.cpp → serializar/deserializar
5. 
Game.cpp (loginPlayer, registerPlayer) → llenar los nuevos campos al construir los eventos
6. 
client/include/Player.h → agregar campos hp, maxHp, mana, maxMana, gold, level, experience
7. 
client/src/GameModel.cpp → al recibir eventos, almacenar estos valores en los Player
8. 
client/include/GameWindow.h → método para dibujar HUD
Módulo 5: HUD en el cliente (HP bar, mana bar, oro, nivel)
1. 
GameWindow.h/cpp → agregar renderizado de:
- 
Barra de HP (roja)
- 
Barra de Mana (azul)
- 
Texto de oro y nivel
- 
Posición: esquinas de la pantalla
Módulo 6: Recuperación pasiva de HP/Mana
El enunciado dice: Vida = FRazaRecuperacion * segundos y Mana = FRazaRecuperacion * segundos.
1. 
Game::movePlayers() o nuevo método Game::tick() → cada N iteraciones, recuperar HP/Mana según fórmulas
2. 
Enviar actualización al cliente via nuevo evento PlayerInfoEvent (ya existe el opcode 0x86 pero sin DTO ni parser)






Módulo 1 completado. Resumen de cambios:
Archivo
client/src/pages/CharacterCreationPage.cpp
common/include/DTO/Commands/RegisterPlayerCommandDTO.h
common/src/parser/Commands/RegisterPlayerCommandParser.cpp
client/src/Gameloop.cpp
server/src/command/CommandFactory.cpp
server/include/command/RegisterPlayerCommand.h
server/src/command/RegisterPlayerCommand.cpp
server/include/Game.h
server/src/Game.cpp
tests/protocol_tests.cpp
Pipeline completo: UI → DTO → Parser (wire) → Cliente → CommandFactory → Command → Game → PlayerData (persistencia).