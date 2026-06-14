#include "command/RejectClanRequestCommand.h"

#include "Game.h"

RejectClanRequestCommand::RejectClanRequestCommand(
    uint32_t founderId, const std::string &playerName)
    : founderId(founderId), playerName(playerName) {}

void RejectClanRequestCommand::execute(Game &game, uint32_t) {
  game.rejectClanRequest(founderId, playerName);
}
