#include "command/CheatCommand.h"

#include "Game.h"

CheatCommand::CheatCommand(CheatType cheat) : cheat(cheat) {}

void CheatCommand::execute(Game &game, uint32_t connectionId) {
  game.applyCheat(connectionId, cheat);
}
