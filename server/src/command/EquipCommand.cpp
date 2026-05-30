#include "command/EquipCommand.h"

#include "Game.h"

EquipCommand::EquipCommand(uint32_t playerId, uint8_t inventorySlot)
    : playerId(playerId), inventorySlot(inventorySlot) {}

void EquipCommand::execute(Game &game, uint32_t) { game.equipItem(playerId, inventorySlot); }
