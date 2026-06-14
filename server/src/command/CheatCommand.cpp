#include "command/CheatCommand.h"

#include "Game.h"

CheatCommand::CheatCommand(CheatType cheat, uint32_t arg)
    : cheat(cheat), arg(arg) {}

void CheatCommand::execute(Game &game, uint32_t connectionId) {
  game.applyCheat(connectionId, cheat, arg);
}
