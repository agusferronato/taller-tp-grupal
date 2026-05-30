#include "command/UnequipCommand.h"

#include "Game.h"

UnequipCommand::UnequipCommand(uint32_t playerId, uint8_t equipSlot)
    : playerId(playerId), equipSlot(equipSlot) {}

void UnequipCommand::execute(Game &game, uint32_t) { game.unequipSlot(playerId, equipSlot); }
