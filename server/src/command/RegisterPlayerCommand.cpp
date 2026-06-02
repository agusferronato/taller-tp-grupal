#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string name, Race race,
                                             std::string playerClass)
    : name(std::move(name)), race(race), playerClass(std::move(playerClass)) {}

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(playerName, race, playerClass, connectionId);
}
