#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "../direction.h"
#include "../types.h"

/*
    MoveCommand
    Representa el comando de movimiento de un jugador en una direccion.
*/

class MoveCommand : public Command {
private:
  PlayerId caller;
  Direction direction;

public:
  explicit MoveCommand(PlayerId caller, Direction direction)
      : caller(caller), direction(direction) {}

  const PlayerId &get_callerId() const { return caller; }
  const Direction &get_direction() const { return direction; }

  virtual void execute([[maybe_unused]] Game &game) override {
    // Placeholder
  }
};

#endif // MOVE_COMMAND_H
