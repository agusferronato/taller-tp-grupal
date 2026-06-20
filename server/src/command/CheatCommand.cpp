#include "command/CheatCommand.h"

#include "Game.h"

CheatCommand::CheatCommand(CheatType cheat, uint32_t arg, std::string itemName)
    : cheat(cheat), arg(arg), itemName(std::move(itemName)) {}

void CheatCommand::execute(Game &game, uint32_t connectionId) {
  game.applyCheat(connectionId, cheat, arg, itemName);
}
