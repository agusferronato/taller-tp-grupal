#include "command/KickClanMemberCommand.h"

#include "Game.h"

KickClanMemberCommand::KickClanMemberCommand(uint32_t founderId,
                                             const std::string &playerName)
    : founderId(founderId), playerName(playerName) {}

void KickClanMemberCommand::execute(Game &game, uint32_t) {
  game.kickClanMember(founderId, playerName);
}
