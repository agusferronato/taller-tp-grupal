#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string name)
    : name(std::move(name)) {}

void RegisterPlayerCommand::execute(Game &game) {
  game.registerPlayer(name);
}
