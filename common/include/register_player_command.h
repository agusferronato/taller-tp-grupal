#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include "../types.h"
#include "command.h"
#include <string>
#include <utility>

/*
    Register Player Command:
    Permite a un jugador registrarse en el juego.
*/
class RegisterPlayerCommand : public Command {
private:
  std::string player_name;

public:
  explicit RegisterPlayerCommand(std::string name)
      : player_name(std::move(name)) {}

  const std::string &get_name() const { return player_name; }

  virtual void execute([[maybe_unused]] Game &game) override {
    // Placeholder
  }
};

#endif // REGISTER_PLAYER_COMMAND_H
