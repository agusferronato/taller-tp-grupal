#include "command/CreateClanCommand.h"

#include "Game.h"

CreateClanCommand::CreateClanCommand(uint32_t playerId,
                                     const std::string &clanName)
    : playerId(playerId), clanName(clanName) {}

void CreateClanCommand::execute(Game &game, uint32_t) {
  game.createClan(playerId, clanName);
}
