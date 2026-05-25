#ifndef CHAT_MESSAGE_EVENT_DTO_H
#define CHAT_MESSAGE_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct ChatMessageEventDTO {
private:
  static constexpr ServerOpcode opCode = ServerOpcode::ChatMessage;

public:
  ServerOpcode getCode() const { return opCode; }
  std::string sender;
  std::string message;
};

#endif // CHAT_MESSAGE_EVENT_DTO_H
