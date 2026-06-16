#include "command/MeditateCommand.h"

#include "Game.h"

MeditateCommand::MeditateCommand(uint32_t playerId) : playerId(playerId) {}

void MeditateCommand::execute(Game &game, uint32_t connectionId) {
  game.startMeditating(connectionId);
}
