#include "command/RegisterPlayerCommand.h"

#include "Game.h"

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(connectionId);
}
