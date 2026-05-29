#include "command/TakeItemCommand.h"

#include "Game.h"

TakeItemCommand::TakeItemCommand(uint32_t playerId) : playerId(playerId) {}

void TakeItemCommand::execute(Game &, uint32_t) {}
