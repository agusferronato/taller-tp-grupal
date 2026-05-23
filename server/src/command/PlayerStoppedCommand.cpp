#include "command/PlayerStoppedCommand.h"

#include "Game.h"

PlayerStoppedCommand::PlayerStoppedCommand(uint32_t playerId)
    : playerId(playerId) {}

void PlayerStoppedCommand::execute(Game& game) {
    game.stopPlayer(playerId);
}
