#ifndef MOVE_PLAYER_COMMAND_H
#define MOVE_PLAYER_COMMAND_H

#include <cstdint>

#include "Command.h"
#include "Direction.h"

class MovePlayerCommand : public Command {
private:
  uint32_t playerId;
  Direction direction;

public:
  MovePlayerCommand(uint32_t playerId, Direction direction);

  void execute(Game &game) override;
};

#endif // MOVE_PLAYER_COMMAND_H
