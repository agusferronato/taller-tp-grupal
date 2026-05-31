#ifndef GLOBAL_CHAT_MESSAGE_COMMAND_DTO_H
#define GLOBAL_CHAT_MESSAGE_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct GlobalChatMessageCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::GlobalChatMessageCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  std::string message;
};

#endif
