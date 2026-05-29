#include "command/ExitCommand.h"

#include "Game.h"

ExitCommand::ExitCommand(uint32_t playerId) : playerId(playerId) {}

void ExitCommand::execute(Game &game, uint32_t) { game.exitPlayer(playerId); }
