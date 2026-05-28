Plan de Implementación — Inventario
1. Modelo de Items (hardcodeados)
Archivo nuevo: server/include/ItemDef.h
enum class ItemType { Weapon, Armor, Helmet, Shield, Staff, PotionHp, PotionMana };
struct ItemDef {
    std::string name;
    ItemType type;
    int minDamage, maxDamage;       // armas y staves
    int minDefense, maxDefense;     // armaduras, cascos, escudos
    int manaCost;                   // staves (costo del hechizo)
    int healAmount;                 // pociones (vida o maná)
    bool isRange;                   // true para arcos, staves
};
// Tabla global con todos los items del enunciado:
// Espada (2-5), Hacha (4-5), Martillo (1-9), Arco simple (1-4), Arco compuesto (4-16)
// Vara de fresno (2-4, mana 5), Flauta élfica (curar, mana 100)
// Báculo nudoso (4-8, mana 15), Báculo engarzado (8-20, mana 30)
// Armadura cuero (2-6), Armadura placas (15-30), Túnica azul (6-10)
// Capucha (1-4), Casco hierro (4-8), Sombrero mágico (4-12)
// Escudo tortuga (1-2), Escudo hierro (1-4)
// Poción de vida, Poción de maná
2. Inventario + Equipamiento (server-side)
Archivo nuevo: server/include/Inventory.h
class Inventory {
    static constexpr size_t MAX_ITEMS = 20;
    std::vector<const ItemDef*> items;
    
    const ItemDef* weaponSlot{nullptr};   // o staff (mutuamente excluyente)
    const ItemDef* armorSlot{nullptr};
    const ItemDef* helmetSlot{nullptr};
    const ItemDef* shieldSlot{nullptr};
    bool addItem(const ItemDef* item);
    bool removeItem(const ItemDef* item);
    bool equip(size_t inventoryIndex);
    bool unequip(ItemType slotType);
    // ...
};
Reglas del enunciado:
- 
Máximo 20 objetos en inventario
- 
Equipar arma O staff (no ambos a la vez)
- 
Equipar poción = consumirla (efecto inmediato)
3. Persistencia
- 
PlayerData.h: añadir slots serializados (lista de uint16_t itemID + 4 IDs para equipados)
- 
PlayerRepository.cpp: serializar/deserializar
- 
Game.cpp (toPlayerData() / fromPlayerData()): incluir inventario
4. Comandos nuevos (protocolo)
Comandos cliente→servidor:
Opcode	Comando	Args
0x16	EquipCommand	playerId, slotIndex
0x17	UnequipCommand	playerId, equipSlot
0x18	DropItemCommand	playerId, slotIndex
0x19	TakeItemCommand	playerId
Eventos servidor→cliente:
Opcode	Evento	Contenido
0x8A	InventoryUpdateEvent	lista de items + equipados (enviado cada cambio)
5. UI en Cliente
Dibujar un panel de inventario (toggle con tecla I) con:
- 
Grilla de 5×4 slots de inventario (80×80px cada uno)
- 
4 slots de equipamiento a la izquierda: arma, armadura, casco, escudo
- 
Click en slot de inventario → equipar
- 
Click en slot equipado → desequipar
- 
Tooltip con nombre y stats del item
El panel se dibuja sobre el mundo (semitransparente) cuando está abierto.
6. Aplicar stats de equipamiento
- 
Formulas.cpp: calcularDañoFisico() usa arma->minDamage y arma->maxDamage en vez de valores fijos
- 
Formulas.cpp: calcularDefensa() suma defensa de armadura + casco + escudo
Archivos a modificar/crear
Archivo	Acción
server/include/ItemDef.h	Crear — definición de items
server/include/Inventory.h	Crear — clase Inventory
server/src/Inventory.cpp	Crear — implementación
server/include/Game.h	Modificar — añadir Inventory a PlayerInfo
server/include/PlayerData.h	Modificar — añadir campos de inventario
server/src/PlayerRepository.cpp	Modificar — serializar inventario
server/src/Game.cpp	Modificar — comandos + save/load
server/src/Formulas.cpp	Modificar — usar items equipados
common/.../EquipCommandDTO.h	Crear
common/.../UnequipCommandDTO.h	Crear
common/.../DropItemCommandDTO.h	Crear
common/.../TakeItemCommandDTO.h	Crear
common/.../InventoryUpdateEventDTO.h	Crear
common/.../EquipCommandParser.cpp	Crear
common/.../InventoryUpdateEventParser.cpp	Crear
common/.../EventDTO.h	Modificar — añadir al variant
common/.../RegisterAllParsers.cpp	Modificar — registrar parsers
client/include/Player.h	Modificar — añadir inventario local
client/include/GameWindow.h	Modificar — panel de inventario
client/src/GameWindow.cpp	Modificar — renderizar panel
client/src/GameModel.cpp	Modificar — manejar eventos
Resumen: ~18 archivos entre crear y modificar. 



Inventario — MVP completo (server + protocolo + cliente)
Archivos nuevos (22 archivos)
Archivo	Propósito
server/include/ItemDef.h	Enums ItemType, EquipSlot, struct ItemDef, tabla global
server/src/ItemDef.cpp	Los 19 items del enunciado (armas, armaduras, cascos, escudos, staves, pociones)
server/include/Inventory.h	Clase Inventory (20 slots + 4 equipados)
server/src/Inventory.cpp	addItem(), removeItem(), equipItem(), unequipSlot()
common/include/InventoryConstants.h	EMPTY_ITEM, MAX_INVENTORY_SLOTS (compartido server+cliente)
common/include/DTO/Commands/EquipCommandDTO.h	Comando equipar (playerId + inventorySlot)
common/include/DTO/Commands/UnequipCommandDTO.h	Comando desequipar (playerId + equipSlot)
common/include/DTO/Commands/DropItemCommandDTO.h	Comando tirar ítem (playerId + inventorySlot)
common/include/DTO/Commands/TakeItemCommandDTO.h	Comando tomar ítem (playerId)
common/include/DTO/Events/InventoryUpdateEventDTO.h	Evento con array de 20 slots + 4 equipados
4 parsers command + 1 parser event (.h + .cpp)	Serialización/deserialización binaria
4 command classes server (.h + .cpp)	EquipCommand, UnequipCommand, DropItemCommand, TakeItemCommand
Archivos modificados (10 archivos)
Archivo	Cambio
ProtocolCodes.h	Nuevos opcodes: EquipCommand=0x17, UnequipCommand=0x18, DropItemCommand=0x19, TakeItemCommand=0x1A, InventoryUpdateEvent=0x8A
ClientCommandDTO.h	Nuevos 4 DTOs en el variant
EventDTO.h	InventoryUpdateEventDTO en el variant
RegisterAllParsers.cpp	Registrados 4 parsers command + 1 event
CommandFactory.cpp	4 nuevos if para crear comandos
PlayerData.h	inventory[20] + equippedWeapon/Armor/Helmet/Shield
PlayerRepository.cpp	writeData/readData serializa los 24 bytes de inventario
Game.h	Inventory inventory en PlayerInfo + 4 métodos públicos
Game.cpp	to/fromPlayerData incluye inventario + implementación de equipItem(), unequipSlot(), dropItem(), takeItem()
client/include/Player.h	inventory[20] + setters/getters para equipados
client/src/GameModel.cpp	playerInventoryUpdated() actualiza Player desde InventoryUpdateEvent
Reglas implementadas
- 
Máximo 20 ítems en inventario
- 
Al equipar un arma → se mueve del inventario al slot, y si ya había otra, vuelve al inventario
- 
Arma y staff son mutuamente excluyentes (no ambos a la vez, como pide el enunciado)
- 
Pociones se consumen al equipar (efecto inmediato, se eliminan del inventario)
- 
Persistencia completa (se guarda/carga con el personaje)
- 
El cliente recibe y almacena el inventario (UI pendiente para después)


