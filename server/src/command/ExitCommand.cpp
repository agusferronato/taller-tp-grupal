#include "command/ExitCommand.h"

#include "Game.h"

void ExitCommand::execute(Game &game, uint32_t connectionId) {
  game.exitPlayer(connectionId);
}
