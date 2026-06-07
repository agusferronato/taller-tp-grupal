#include "command/AtackCommand.h"
#include "Game.h"

AtackCommand::AtackCommand(uint32_t playerId, int16_t x, int16_t y)
    : playerId(playerId), x(x), y(y) {}

void AtackCommand::execute(Game &game, uint32_t) { game.atack(playerId, x, y); }
