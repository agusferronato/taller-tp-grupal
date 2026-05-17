#ifndef PRIVATE_MESSAGE_COMMAND_H
#define PRIVATE_MESSAGE_COMMAND_H

#include "../types.h"
#include "command.h"
#include <string>
#include <utility>

/*
    Private Message Command:
    Permite a un jugador enviar un mensaje privado a otro jugador.
*/

class PrivateMessageCommand : public Command {
private:
  PlayerId caller;
  std::string target_id;
  std::string message;

public:
  explicit PrivateMessageCommand(PlayerId sender_id, std::string target_id,
                                 std::string message)
      : caller(sender_id), target_id(target_id), message(std::move(message)) {}

  const PlayerId &get_callerId() const { return caller; }
  const std::string &get_target() const { return target_id; }
  const std::string &get_message() const { return message; }

  virtual void execute([[maybe_unused]] Game &game) override {
    // Placeholder
  }
};

#endif // PRIVATE_MESSAGE_COMMAND_H
