#include "command/UnequipCommand.h"

#include "Game.h"

UnequipCommand::UnequipCommand(uint32_t playerId, uint8_t equipSlot)
    : playerId(playerId), equipSlot(equipSlot) {}

void UnequipCommand::execute(Game &game) { game.unequipSlot(playerId, equipSlot); }
