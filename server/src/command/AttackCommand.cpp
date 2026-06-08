#include "command/AttackCommand.h"
#include "Game.h"

AttackCommand::AttackCommand(uint32_t playerId, int16_t x, int16_t y)
    : playerId(playerId), x(x), y(y) {}

void AttackCommand::execute(Game &game, uint32_t) {
  game.attack(playerId, x, y);
}
