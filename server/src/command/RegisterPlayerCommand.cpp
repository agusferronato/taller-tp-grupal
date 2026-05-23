#include "command/RegisterPlayerCommand.h"

#include "Game.h"

void RegisterPlayerCommand::execute(Game& game) {
    game.registerPlayer();
}
