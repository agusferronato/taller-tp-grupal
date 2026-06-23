#ifndef PRIVATE_MESSAGE_EVENT_DTO_H
#define PRIVATE_MESSAGE_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct PrivateMessageEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PrivateMessageEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::string senderName;
  std::string targetName;
  std::string message;
};

#endif
