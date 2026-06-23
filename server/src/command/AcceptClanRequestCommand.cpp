#include "command/AcceptClanRequestCommand.h"

#include "Game.h"

AcceptClanRequestCommand::AcceptClanRequestCommand(
    uint32_t founderId, const std::string &playerName)
    : founderId(founderId), playerName(playerName) {}

void AcceptClanRequestCommand::execute(Game &game, uint32_t) {
  game.acceptClanRequest(founderId, playerName);
}
