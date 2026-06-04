#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string name, Race race,
                                             PlayerClass playerClass)
    : name(std::move(name)), race(race), playerClass(playerClass) {}

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(name, race, playerClass, connectionId);
}
