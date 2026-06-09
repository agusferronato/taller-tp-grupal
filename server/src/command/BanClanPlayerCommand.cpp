#include "command/BanClanPlayerCommand.h"

#include "Game.h"

BanClanPlayerCommand::BanClanPlayerCommand(uint32_t founderId,
                                           const std::string &playerName)
    : founderId(founderId), playerName(playerName) {}

void BanClanPlayerCommand::execute(Game &game, uint32_t) {
  game.banClanPlayer(founderId, playerName);
}
