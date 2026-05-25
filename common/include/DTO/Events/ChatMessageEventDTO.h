#ifndef CHAT_MESSAGE_EVENT_DTO_H
#define CHAT_MESSAGE_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct ChatMessageEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::ChatMessageEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::string sender;
  std::string message;
};

#endif // CHAT_MESSAGE_EVENT_DTO_H
