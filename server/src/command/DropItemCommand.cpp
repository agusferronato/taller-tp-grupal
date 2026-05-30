#include "command/DropItemCommand.h"

#include "Game.h"

DropItemCommand::DropItemCommand(uint32_t playerId, uint8_t inventorySlot)
    : playerId(playerId), inventorySlot(inventorySlot) {}

void DropItemCommand::execute(Game &game, uint32_t) { game.dropItem(playerId, inventorySlot); }
