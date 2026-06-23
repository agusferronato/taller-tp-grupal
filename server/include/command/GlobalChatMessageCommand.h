#ifndef GLOBAL_CHAT_MESSAGE_COMMAND_H
#define GLOBAL_CHAT_MESSAGE_COMMAND_H

#include <cstdint>
#include <string>

#include "Command.h"

class GlobalChatMessageCommand : public Command {
private:
  uint32_t playerId;
  std::string message;

public:
  explicit GlobalChatMessageCommand(uint32_t playerId,
                                    const std::string &message);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
