#include "command/RegisterPlayerCommand.h"

#include "Game.h"

RegisterPlayerCommand::RegisterPlayerCommand(std::string playerName, std::string race, std::string playerClass)
    : playerName(std::move(playerName)), race(std::move(race)), playerClass(std::move(playerClass)) {}

void RegisterPlayerCommand::execute(Game &game, uint32_t connectionId) {
  game.registerPlayer(playerName, race, playerClass, connectionId);
}
