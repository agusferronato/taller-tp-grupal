#include "command/ExitCommand.h"

#include "Game.h"

ExitCommand::ExitCommand(uint32_t playerId) : playerId(playerId) {}

void ExitCommand::execute(Game &game, uint32_t connectionId) {
  if (playerId != 0) {
    game.exitPlayer(playerId);
  } else {
    game.exitPlayerByConnection(connectionId);
  }
}
