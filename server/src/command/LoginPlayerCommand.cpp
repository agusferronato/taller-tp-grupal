#include "command/LoginPlayerCommand.h"

#include "Game.h"

LoginPlayerCommand::LoginPlayerCommand(std::string name)
    : name(std::move(name)) {}

void LoginPlayerCommand::execute(Game &game, uint32_t) {
  game.loginPlayer(name);
}
