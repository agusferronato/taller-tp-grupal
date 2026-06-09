#ifndef CHAT_MESSAGE_EVENT_DTO_H
#define CHAT_MESSAGE_EVENT_DTO_H

#include "DTO/Events/ChatMessageCategory.h"
#include "protocol/ProtocolCodes.h"
#include <string>

struct ChatMessageEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::ChatMessageEvent;

public:
  EventOpcode getCode() const { return opCode; }
  ChatMessageCategory category;
  std::string sender;
  std::string message;
};

#endif // CHAT_MESSAGE_EVENT_DTO_H
