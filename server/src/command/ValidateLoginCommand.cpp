#include "command/ValidateLoginCommand.h"

#include "Game.h"

#include <utility>

ValidateLoginCommand::ValidateLoginCommand(std::string playerName)
    : playerName(std::move(playerName)) {}

void ValidateLoginCommand::execute(Game &game, uint32_t connectionId) {
  game.validateLogin(playerName, connectionId);
}
