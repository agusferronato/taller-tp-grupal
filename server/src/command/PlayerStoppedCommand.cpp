#include "command/PlayerStoppedCommand.h"

#include "Game.h"

PlayerStoppedCommand::PlayerStoppedCommand(uint32_t playerId)
    : playerId(playerId) {}

void PlayerStoppedCommand::execute(Game &game, uint32_t) {
  game.stopPlayer(playerId);
}
