#include "command/LeaveClanCommand.h"

#include "Game.h"

LeaveClanCommand::LeaveClanCommand(uint32_t playerId) : playerId(playerId) {}

void LeaveClanCommand::execute(Game &game, uint32_t) {
  game.leaveClan(playerId);
}
