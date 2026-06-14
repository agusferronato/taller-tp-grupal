#include "command/ReviewClanCommand.h"

#include "Game.h"

ReviewClanCommand::ReviewClanCommand(uint32_t playerId) : playerId(playerId) {}

void ReviewClanCommand::execute(Game &game, uint32_t) {
  game.reviewClan(playerId);
}
