#ifndef PRIVATE_MESSAGE_COMMAND_H
#define PRIVATE_MESSAGE_COMMAND_H

#include <string>

#include "Command.h"

class PrivateMessageCommand : public Command {
private:
  std::string targetName;
  std::string message;

public:
  explicit PrivateMessageCommand(const std::string &targetName,
                                 const std::string &message);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
