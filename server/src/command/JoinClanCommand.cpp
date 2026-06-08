#include "command/JoinClanCommand.h"

#include "Game.h"

JoinClanCommand::JoinClanCommand(uint32_t playerId,
                                 const std::string &clanName)
    : playerId(playerId), clanName(clanName) {}

void JoinClanCommand::execute(Game &game, uint32_t) {
  game.requestJoinClan(playerId, clanName);
}
