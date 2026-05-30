#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(const std::string &playerName)
    : playerName(playerName) {}

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(connectionId, playerName);
}
