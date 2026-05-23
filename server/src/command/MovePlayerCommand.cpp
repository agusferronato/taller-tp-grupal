#include "command/MovePlayerCommand.h"

#include "Game.h"

MovePlayerCommand::MovePlayerCommand(uint32_t playerId, Direction direction)
    : playerId(playerId), direction(direction) {}

void MovePlayerCommand::execute(Game& game) {
    game.movePlayer(playerId, direction);
}
