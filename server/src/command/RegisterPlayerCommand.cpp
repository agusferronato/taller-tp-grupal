#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string name, std::string race, std::string playerClass)
    : name(std::move(name)), race(std::move(race)), playerClass(std::move(playerClass)) {}

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(name, race, playerClass, connectionId);
}
