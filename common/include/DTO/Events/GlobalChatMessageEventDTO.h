#ifndef GLOBAL_CHAT_MESSAGE_EVENT_DTO_H
#define GLOBAL_CHAT_MESSAGE_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct GlobalChatMessageEventDTO {
private:
  static constexpr EventOpcode opCode =
      EventOpcode::GlobalChatMessageEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::string playerName;
  std::string message;
};

#endif
