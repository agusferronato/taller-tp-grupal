#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string name, std::string race)
    : name(std::move(name)), race(std::move(race)) {}

void RegisterPlayerCommand::execute(Game &game) {
  game.registerPlayer(name, race);
}
